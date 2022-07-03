#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provolone_program.h"
#include "llist.h"

static void provol_cmds_free(LinkedList *cmds);
static void provol_funs_free(LinkedList *cmds);
static void provol_vars_free(LinkedList *cmds);

static void provol_cmds_print_tree(const LinkedList *cmds, const int l);

ProvolProgram *provol_prog_create(void) {
	ProvolProgram *p = (ProvolProgram *)malloc(sizeof(ProvolProgram));
	if (p == NULL)
		return NULL;

	p->cmds	= NULL;
	p->funs	= llist_create();
	p->in	= llist_create();
	p->out	= llist_create();
	p->loc	= llist_create();
	p->is_bootstrapped = 1;

	llist_append(p->funs, "INC");
	llist_append(p->funs, "ZERO");

	return p;
}

void provol_prog_free(ProvolProgram *p) {
	assert(p != NULL);
	provol_cmds_free(p->cmds);
	provol_vars_free(p->in);
	provol_vars_free(p->out);
	provol_vars_free(p->loc);
	provol_funs_free(p->funs);
	free(p);
}

void provol_prog_add_ins(ProvolProgram *p, LinkedList *var_ids) {
	assert(p != NULL);

	if (var_ids == NULL)
		return;

	while (!llist_is_empty(var_ids)) {
		const char *var_id = (const char *)llist_pop(var_ids);
		ProvolVar *var = (ProvolVar *)malloc(sizeof(ProvolVar));
		if (var == NULL)
			return;
		var->id = var_id;
		var->is_init = 1;

		llist_append(p->in, (void *)var);
	}
	llist_free(var_ids);
}

void provol_prog_add_outs(ProvolProgram *p, LinkedList *var_ids) {
	assert(p != NULL);

	if (var_ids == NULL)
		return;

	while (!llist_is_empty(var_ids)) {
		const char *var_id = (const char *)llist_pop(var_ids);
		ProvolVar *var = (ProvolVar *)malloc(sizeof(ProvolVar));
		if (var == NULL)
			return;
		var->id = var_id;
		var->is_init = 0;

		llist_append(p->out, (void *)var);
	}
	llist_free(var_ids);
}

void provol_prog_add_cmds(ProvolProgram *p, LinkedList *cmds) {
	assert(cmds != NULL);
	p->cmds = cmds;
}

LinkedList *provol_var_append(LinkedList *vars, const char *id) {
	assert(id != NULL);
	if (vars == NULL)
		vars = llist_create();
	llist_append(vars, (void *)id);
	return vars;
}

LinkedList *provol_cmds_append(LinkedList *cmds, ProvolCmd *cmd) {
	assert(cmd != NULL);
	if (cmds == NULL)
		cmds = llist_create();
	llist_append(cmds, (void *)cmd);
	return cmds;
}

ProvolCmd *provol_wloop_new(const ProvolProgram *p, const char *cond_id, LinkedList *body) {
	ProvolCmd *cmd;

	assert(p != NULL);
	assert(cond_id != NULL);
	assert(body != NULL);

	cmd = (ProvolCmd *)malloc(sizeof(ProvolCmd));
	if (cmd == NULL)
		return NULL;
	cmd->type = P_WLOOP;
	cmd->val.wloop.cond_id = cond_id;
	cmd->val.wloop.body = body;

	return cmd;
}

ProvolCmd *provol_doloop_new(const ProvolProgram *p, const char *times, LinkedList *body) {
	ProvolCmd *cmd;

	assert(p != NULL);
	assert(times != NULL);
	assert(body != NULL);

	cmd = (ProvolCmd *)malloc(sizeof(ProvolCmd));
	if (cmd == NULL)
		return NULL;
	cmd->type = P_DOLOOP;
	cmd->val.doloop.times = times;
	cmd->val.doloop.body = body;

	return cmd;
}

ProvolCmd *provol_assign_new(const ProvolProgram *p, const char *dest, const char *src) {
	ProvolCmd *cmd;

	assert(p != NULL);
	assert(dest != NULL);
	assert(src != NULL);

	// TODO: Check if src is initialised

	cmd = (ProvolCmd *)malloc(sizeof(ProvolCmd));
	if (cmd == NULL)
		return NULL;
	cmd->type = P_ASSIGN;
	cmd->val.assign.dest = dest;
	cmd->val.assign.src = src;

	return cmd;
}

ProvolCmd *provol_call_new(const ProvolProgram *p, const char *fun, const char *arg) {
	ProvolCmd *cmd;

	assert(p != NULL);
	assert(fun != NULL);
	assert(arg != NULL);

	// TODO: check if fun exists and if arg is initialised

	cmd = (ProvolCmd *)malloc(sizeof(ProvolCmd));
	if (cmd == NULL)
		return NULL;
	cmd->type = P_CALL;
	cmd->val.call.fun = fun;
	cmd->val.call.arg = arg;

	return cmd;
}

ProvolCmd *provol_if_new(const ProvolProgram *p, const char *cond_id, LinkedList *if_body, LinkedList *else_body) {
	ProvolCmd *cmd;

	assert(p != NULL);
	assert(cond_id != NULL);
	assert(if_body != NULL);

	cmd = (ProvolCmd *)malloc(sizeof(ProvolCmd));
	if (cmd == NULL)
		return NULL;

	cmd->type = P_IF;
	cmd->val.ifelse.cond_id = cond_id;
	cmd->val.ifelse.if_body = if_body;
	cmd->val.ifelse.else_body = else_body;

	return cmd;
}

static void provol_cmds_free(LinkedList *cmds) {
	ProvolCmd *cmd;
	while (!llist_is_empty(cmds)) {
		cmd = llist_pop(cmds);
		switch (cmd->type) {
		case P_ASSIGN:
			free((void *)cmd->val.assign.dest);
			free((void *)cmd->val.assign.src);
			break;

		case P_CALL:
			free((void *)cmd->val.call.fun);
			free((void *)cmd->val.call.arg);
			break;

		case P_WLOOP:
			free((void *)cmd->val.wloop.cond_id);
			provol_cmds_free(cmd->val.wloop.body);
			break;

		case P_DOLOOP:
			free((void *)cmd->val.doloop.times);
			provol_cmds_free(cmd->val.doloop.body);
			break;

		case P_IF:
			free((void *)cmd->val.ifelse.cond_id);
			provol_cmds_free(cmd->val.ifelse.if_body);
			if (cmd->val.ifelse.else_body != NULL)
				provol_cmds_free(cmd->val.ifelse.else_body);
			break;
		}
		free(cmd);
	}
	llist_free(cmds);
}

static void provol_funs_free(LinkedList *cmds) {
	llist_free(cmds);
}

static void provol_vars_free(LinkedList *vars) {
	ProvolVar *var;

	if (vars == NULL)
		return;

	while (!llist_is_empty(vars)) {
		var = llist_pop(vars);
		free((void *)var->id);
		free(var);
	}

	llist_free(vars);
}

static int cmp_var(const void *v, const void *id) {
	return strcmp(((const ProvolVar *)v)->id, ((const char *)id));
}

static int cmp_fun(const void *a, const void *b) {
	return strcmp(((const char *)a), ((const char *)b));
}

ProvolSymbS provol_program_check_symbol(const ProvolProgram *p, const char *sym) {
	union {
		ProvolVar *v;
		const char *f;
	} n;

	assert(p != NULL);
	assert(sym != NULL);

	/* Search in vars */
	n.v = (ProvolVar *)llist_search(p->loc, sym, &cmp_var);
	if (n.v != NULL)
		return P_VAR_I;

	n.v = (ProvolVar *)llist_search(p->in, sym, &cmp_var);
	if (n.v != NULL)
		return P_VAR_I;

	n.v = (ProvolVar *)llist_search(p->out, sym, &cmp_var);
	if (n.v != NULL) {
		if (n.v->is_init)
			return P_VAR_I;
		else
			return P_VAR_U;
	}

	/* Search for function symbols */
	n.f = (const char *)llist_search(p->funs, sym, &cmp_fun);
	if (n.f != NULL)
		return P_FUN;

	return P_UNDEF;
}

static void provol_cmd_print(const ProvolCmd *c, const int l) {
	switch (c->type) {
	case P_WLOOP:
		printf("WHILE %s do:\n", c->val.wloop.cond_id);
		provol_cmds_print_tree(c->val.wloop.body, l+1);
		break;

	case P_DOLOOP:
		printf("DO %s TIMES:\n", c->val.doloop.times);
		provol_cmds_print_tree(c->val.doloop.body, l+1);
		break;

	case P_ASSIGN:
		printf("%s = %s", c->val.assign.dest, c->val.assign.src);
		break;

	case P_CALL:
		printf("%s(%s)", c->val.call.fun, c->val.call.arg);
		break;

	case P_IF:
		printf("IF %s do:\n", c->val.ifelse.cond_id);
		provol_cmds_print_tree(c->val.ifelse.if_body, l+1);
		if (c->val.ifelse.else_body != NULL) {
			printf("ELSE:\n");
			provol_cmds_print_tree(c->val.ifelse.else_body, l+1);
		}
	}
}

static void provol_cmds_print_tree(const LinkedList *cmds, const int l) {
	LLNode *n;

	for (n = cmds->head; n != NULL; n = n->next) {
		int i;
		for (i = 0; i < l; ++i)
			putchar('\t');

		if (n->next == NULL)
			printf("└── ");
		else
			printf("├── ");

		provol_cmd_print((ProvolCmd*)n->val, l);
		putchar('\n');
	}
}

void provol_prog_print_tree(const ProvolProgram *p) {
	LLNode *n;

	printf("------ PROGRAM TREE -----\n");
	printf("IN Vars: ");
	for (n = p->in->head; n != NULL; n = n->next) {
		printf("%s, ", ((ProvolVar *)n->val)->id);
	}
	putchar('\n');

	printf("OUT Vars: ");
	for (n = p->out->head; n != NULL; n = n->next) {
		printf("%s, ", ((ProvolVar *)n->val)->id);
	}
	putchar('\n');

	printf("LOC Vars: ");
	for (n = p->loc->head; n != NULL; n = n->next) {
		printf("%s, ", ((ProvolVar *)n->val)->id);
	}
	putchar('\n');

	printf("FUNS: ");
	for (n = p->funs->head; n != NULL; n = n->next) {
		printf("%s, ", ((const char *)n->val));
	}
	putchar('\n');

	puts("CMDS:");

	provol_cmds_print_tree(p->cmds, 0);
}

