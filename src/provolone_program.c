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

	while (p->cmds) {
		ProvolCmd *c = p->cmds;
		p->cmds = c->next;

		provol_cmd_free(c);
	}
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

int provol_program_cmd_new(ProvolProgram *p, const ProvolCmd *cmd);

ProvolCmd *provol_cmd_new(const ProvolCmd_t type, const void *val);
void provol_cmd_free(ProvolCmd *cmd);

ProvolWloop *provol_wloop_new(const ProvolId cond, const ProvolCmd *body);
void provol_wloop_free(ProvolWloop *wl);

ProvolAssign *provol_assign_new(const ProvolId src, const ProvolId dest);
void provol_assign_free(ProvolAssign *as);

ProvolCall *provol_call_new(const ProvolId fun, const ProvolId arg);
void provol_call_free(ProvolCall *c);

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

