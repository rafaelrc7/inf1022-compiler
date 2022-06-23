#include "provolone_compiler.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "provolone_program.h"

static void provol_cc_cmd(FILE *out, ProvolCmd *cmd, int level);
static void provol_cc_cmds(FILE *out, LinkedList *cmds, int level);
static void provol_pc_cmd(FILE *out, ProvolCmd *cmd, int level);
static void provol_pc_cmds(FILE *out, LinkedList *cmds, int level);

int provol_cc(FILE *out, const ProvolProgram *p) {
	assert(out != NULL);
	assert(p != NULL);

	fprintf(out, "void provol_fun(");
	for (LLNode *n = p->in->head; n != NULL; n = n->next) {
		if (n->next != NULL) {
			fprintf(out, "unsigned int %s, ", ((ProvolVar *)n->val)->id);
		} else {
			fprintf(out, "unsigned int %s", ((ProvolVar *)n->val)->id);
		}
	}

	if (!llist_is_empty(p->out)) {
		fprintf(out, ", ");
		for (LLNode *n = p->out->head; n != NULL; n = n->next) {
			if (n->next != NULL) {
				fprintf(out, "unsigned int *%s_ptr, ", ((ProvolVar *)n->val)->id);
			} else {
				fprintf(out, "unsigned int *%s_ptr", ((ProvolVar *)n->val)->id);
			}
		}
	}
	fputs(") {\n", out);

	if (!llist_is_empty(p->out)) {
		fprintf(out, "\t/* Out variable definitions */\n\tunsigned int ");
		for (LLNode *n = p->out->head; n != NULL; n = n->next) {
			if (n->next != NULL) {
				fprintf(out, "%s, ", ((ProvolVar *)n->val)->id);
			} else {
				fprintf(out, "%s;\n", ((ProvolVar *)n->val)->id);
			}
		}
	}

	if (!llist_is_empty(p->loc)) {
		fprintf(out, "\n\t/* Local Variable definitions */\n\tunsigned int ");
		for (LLNode *n = p->loc->head; n != NULL; n = n->next) {
			if (n->next != NULL) {
				fprintf(out, "%s, ", ((ProvolVar *)n->val)->id);
			} else {
				fprintf(out, "%s;\n", ((ProvolVar *)n->val)->id);
			}
		}
	}

	provol_cc_cmds(out, p->cmds, 1);

	if (!llist_is_empty(p->out)) {
		fprintf(out, "\n\t/* Move out variable values to given address */\n");
		for (LLNode *n = p->out->head; n != NULL; n = n->next) {
			fprintf(out, "\t*%s_ptr = %s;\n", ((ProvolVar *)n->val)->id, ((ProvolVar *)n->val)->id);
		}
	}

	fputs("}\n\n", out);
	return 0;
}

static void provol_cc_cmds(FILE *out, LinkedList *cmds, int level) {
	LLNode *n;
	for (n = cmds->head; n != NULL; n = n->next) {
		provol_cc_cmd(out, (ProvolCmd *)n->val, level);
	}
}

static void provol_cc_cmd(FILE *out, ProvolCmd *cmd, int level) {
	int i;

	for (i = 0; i < level; ++i) {
		fputc('\t', out);
	}

	switch (cmd->type) {
	case P_WLOOP:
		fprintf(out, "while (%s) {\n", cmd->val.wloop.cond_id);
		provol_cc_cmds(out, cmd->val.wloop.body, level+1);
		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "}\n");
		break;

	case P_ASSIGN:
		fprintf(out, "%s = %s;\n", cmd->val.assign.dest, cmd->val.assign.src);
		break;

	case P_CALL:
		if (strcmp(cmd->val.call.fun, "INC") == 0) {
			fprintf(out, "++%s;\n", cmd->val.call.arg);
		} else if (strcmp(cmd->val.call.fun, "ZERO") == 0) {
			fprintf(out, "%s = 0;\n", cmd->val.call.arg);
		}
		break;
	default:
		return;
	}
}

int provol_pc(FILE *out, const ProvolProgram *p) {
	fprintf(out, "PROGRAM\n");

	if (!llist_is_empty(p->in)) {
		fprintf(out, "ENTRADA ");
		for (LLNode *n = p->in->head; n != NULL; n = n->next) {
			if (n->next != NULL) {
				fprintf(out, "%s, ", ((ProvolVar *)n->val)->id);
			} else {
				fprintf(out, "%s\n", ((ProvolVar *)n->val)->id);
			}
		}
	}

	if (!llist_is_empty(p->out)) {
		fprintf(out, "SAIDA ");
		for (LLNode *n = p->out->head; n != NULL; n = n->next) {
			if (n->next != NULL) {
				fprintf(out, "%s, ", ((ProvolVar *)n->val)->id);
			} else {
				fprintf(out, "%s\n", ((ProvolVar *)n->val)->id);
			}
		}
	}

	provol_pc_cmds(out, p->cmds, 1);

	fprintf(out, "FIM\n");

	return 0;
}

static void provol_pc_cmds(FILE *out, LinkedList *cmds, int level) {
	LLNode *n;
	for (n = cmds->head; n != NULL; n = n->next) {
		provol_pc_cmd(out, (ProvolCmd *)n->val, level);
	}
}

static void provol_pc_cmd(FILE *out, ProvolCmd *cmd, int level) {
	int i;

	for (i = 0; i < level; ++i) {
		fputc('\t', out);
	}

	switch (cmd->type) {
	case P_WLOOP:
		fprintf(out, "ENQUANTO %s FACA\n", cmd->val.wloop.cond_id);
		provol_pc_cmds(out, cmd->val.wloop.body, level+1);
		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "FIM\n");
		break;

	case P_ASSIGN:
		fprintf(out, "%s = %s\n", cmd->val.assign.dest, cmd->val.assign.src);
		break;

	case P_CALL:
		fprintf(out, "%s(%s)\n", cmd->val.call.fun, cmd->val.call.arg);
		break;

	case P_IF:
		fprintf(out, "ENQUANTO %s FACA\n", cmd->val.ifelse.cond_id);
		provol_pc_cmds(out, cmd->val.ifelse.if_body, level+1);
		fprintf(out, "FIM\n");
		break;
	}
}

