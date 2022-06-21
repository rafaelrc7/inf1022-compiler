#include <stdlib.h>
#include <assert.h>

#include "provolone_program.h"

static int	provol_prog_var_new(const ProvolId id, const int is_init, ProvolVar **next);
static void	provol_prog_vars_free(ProvolVar *vs);

ProvolProgram *provol_program_create(void) {
	return (ProvolProgram *)calloc(1, sizeof(ProvolProgram));
}

void provol_program_free(ProvolProgram *p) {
	assert(p != NULL);
	provol_prog_vars_free(p->in);
	provol_prog_vars_free(p->out);
	provol_cmd_free(p->cmds);
}

int provol_program_in_new(ProvolProgram *p, const ProvolId id) {
	assert(p != NULL);
	assert(id != NULL);
	return provol_prog_var_new(id, 1, &p->in);
}

int provol_program_out_new(ProvolProgram *p, const ProvolId id) {
	assert(p != NULL);
	assert(id != NULL);
	return provol_prog_var_new(id, 0, &p->out);
}

int provol_program_cmd_new(ProvolProgram *p, ProvolCmd *cmd) {
	assert(p != NULL);
	assert(cmd != NULL);

	cmd->next = p->cmds;
	p->cmds = cmd;

	return 0;
}

ProvolCmd *provol_cmd_new(const ProvolCmd_t type, void *val) {
	ProvolCmd *cmd;

	assert(val != NULL);

	cmd = (ProvolCmd *)malloc(sizeof(ProvolCmd));
	if (cmd == NULL)
		return NULL;

	cmd->type = type;
	cmd->val = val;
	cmd->next = NULL;

	return cmd;
}

void provol_cmd_free(ProvolCmd *cmd) {
	ProvolCmd *next;

	if (cmd == NULL)
		return;

	switch(cmd->type) {
	case P_WLOOP:
		provol_wloop_free((ProvolWloop *)cmd->val);
		break;

	case P_ASSIGN:
		provol_assign_free((ProvolAssign *)cmd->val);
		break;

	case P_CALL:
		provol_call_free((ProvolCall *)cmd->val);
		break;
	} /* switch(cmd->type) */

	next = cmd->next;
	free(cmd);
	provol_cmd_free(next);
}

ProvolWloop *provol_wloop_new(const ProvolId cond, ProvolCmd *body) {
	ProvolWloop *wl;

	assert(cond != NULL);
	assert(body != NULL);

	wl = (ProvolWloop *)malloc(sizeof(ProvolWloop));
	if (wl == NULL)
		return NULL;

	wl->cond = cond;
	wl->body = body;

	return wl;
}

void provol_wloop_free(ProvolWloop *wl) {
	assert(wl != NULL);
	free(wl->cond);
	free(wl->body);
	free(wl);
}

ProvolAssign *provol_assign_new(const ProvolId src, const ProvolId dest) {
	ProvolAssign *as;

	assert(src != NULL);
	assert(dest != NULL);

	as = (ProvolAssign *)malloc(sizeof(ProvolAssign));
	if (as == NULL)
		return NULL;

	as->src = src;
	as->dest = dest;

	return as;
}

void provol_assign_free(ProvolAssign *as) {
	assert(as != NULL);
	free(as->dest);
	free(as->src);
	free(as);
}

ProvolCall *provol_call_new(const ProvolId fun, const ProvolId arg) {
	ProvolCall *c;

	assert(fun != NULL);
	assert(arg != NULL);

	c = (ProvolCall *)malloc(sizeof(ProvolCall));
	if (c == NULL)
		return NULL;

	c->fun = fun;
	c->arg = arg;

	return c;
}

void provol_call_free(ProvolCall *c) {
	assert(c != NULL);
	free(c->fun);
	free(c->arg);
}

ProvolSymbS provol_program_check_symbol(const ProvolProgram *p, const ProvolId sym);

static int provol_prog_var_new(const ProvolId id, const int is_init, ProvolVar **next) {
	ProvolVar *var = (ProvolVar *)malloc(sizeof(ProvolVar));
	if (var == NULL)
		return 1;

	var->name = id;
	var->is_init = is_init;
	var->next = *next;
	*next = var;

	return 0;
}

static void	provol_prog_vars_free(ProvolVar *vs) {
	while (vs != NULL) {
		ProvolVar *v = vs;
		vs = v->next;

		free(v->name);
		free(v);
	}
}

