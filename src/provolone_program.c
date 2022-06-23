#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "provolone_program.h"
#include "llist.h"

static void provol_cmds_free(LinkedList *cmds);
static void provol_funs_free(LinkedList *cmds);
static void provol_vars_free(LinkedList *cmds);

ProvolProgram *provol_prog_create(void) {
	ProvolProgram *p = (ProvolProgram *)malloc(sizeof(ProvolProgram));
	if (p == NULL)
		return NULL;

	p->cmds	= NULL;
	p->funs	= llist_create();
	p->in	= llist_create();
	p->out	= llist_create();
	p->loc	= llist_create();

	// TODO: Add default functions (INC ZERO)

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

// ProvolSymbS provol_program_check_symbol(const ProvolProgram *p, const char *sym) {
// 	union {
// 		ProvolVar *v;
// 		const char *f;
// 	} n;
//
// 	assert(p != NULL);
// 	assert(sym != NULL);
//
// 	/* Search in vars */
// 	for (n.v = p->in; n.v != NULL; n.v = n.v->next) {
// 		if (strcmp(n.v->name, sym) == 0)
// 			return P_VAR_I;
// 	}
//
// 	for (n.v = p->out; n.v != NULL; n.v = n.v->next) {
// 		if (strcmp(n.v->name, sym) == 0) {
// 			if (n.v->is_init)
// 				return P_VAR_I;
// 			else
// 				return P_VAR_U;
// 		}
// 	}
//
// 	/* Search for function symbols */
// 	for (n.f = p->funs; n.f != NULL; n.f = n.f->next) {
// 		if (strcmp(n.f->name, sym) == 0)
// 			return P_FUN;
// 	}
//
// 	return P_UNDEF;
// }

