#include "provolone_compiler.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provolone_program.h"

static void provol_cc_cmd(FILE *out, ProvolCmd *cmd, int level);
static void provol_cc_cmds(FILE *out, LinkedList *cmds, int level);
static void provol_pc_cmd(FILE *out, ProvolCmd *cmd, int level);
static void provol_pc_cmds(FILE *out, LinkedList *cmds, int level);

int provol_cc(FILE *out, const ProvolProgram *p) {
	assert(out != NULL);
	assert(p != NULL);

	fprintf(out, "void %s(", p->id);
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
		} else if (strcmp(cmd->val.call.fun, "DEC") == 0) {
			fprintf(out, "--%s;\n", cmd->val.call.arg);
		} else if (strcmp(cmd->val.call.fun, "ZERO") == 0) {
			fprintf(out, "%s = 0;\n", cmd->val.call.arg);
		}
		break;
	default:
		return;
	}
}

int provol_pc(FILE *out, const ProvolProgram *p) {
	fprintf(out, "PROGRAM %s\n", p->id);

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

static void provol_pc_as_expr(FILE *out, ProvolCmd *cmd, int level) {
	int i;

	fprintf(out, "N1N%d=%s\n", level, cmd->val.assign_e.expr->id1);

	for (i = 0; i < level; ++i)
		fputc('\t', out);
	fprintf(out, "N2N%d=%s\n", level, cmd->val.assign_e.expr->id2);

	switch (cmd->val.assign_e.expr->op) {
	case '+':
		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "%s=N1N%d\n", cmd->val.assign_e.dest, level);
		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "REPITA N2N%d VEZES\n", level);
		for (i = 0; i < level+1; ++i)
			fputc('\t', out);
		fprintf(out, "INC(%s)\n", cmd->val.assign_e.dest);
		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "FIM\n");
		break;

	case '-':
		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "%s=N1N%d\n", cmd->val.assign_e.dest, level);
		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "REPITA N2N%d VEZES\n", level);
		for (i = 0; i < level+1; ++i)
			fputc('\t', out);
		fprintf(out, "DEC(%s)\n", cmd->val.assign_e.dest);
		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "FIM\n");
		break;

	case '*':
		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "ZERO(%s)\n", cmd->val.assign_e.dest);
		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "REPITA N2N%d VEZES\n", level);
		for (i = 0; i < level+1; ++i)
			fputc('\t', out);
		fprintf(out, "%s = %s + N1N%d\n", cmd->val.assign_e.dest, cmd->val.assign_e.dest, level);
		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "FIM\n");
		break;

	default:
		fprintf(stderr, "ERROR: undentified op '%c'.\n", cmd->val.assign_e.expr->op);
		exit(EXIT_FAILURE);
		break;
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

	case P_DOLOOP:
		fprintf(out, "DOLOOP%d = %s\n", level, cmd->val.doloop.times);
		fprintf(out, "ENQUANTO DOLOOP%d FACA\n", level);
		provol_pc_cmds(out, cmd->val.doloop.body, level+1);
		fprintf(out, "DEC(DOLOOP%d)\nFIM\n", level);
		break;

	case P_ASSIGN:
		fprintf(out, "%s = %s\n", cmd->val.assign.dest, cmd->val.assign.src);
		break;

	case P_ASSIGN_E:
		provol_pc_as_expr(out, cmd, level+1);
		break;

	case P_CALL:
		fprintf(out, "%s(%s)\n", cmd->val.call.fun, cmd->val.call.arg);
		break;

	case P_IF:
		if (cmd->val.ifelse.else_body != NULL) {
			fprintf(out, "ZERO(ELSE%d) INC(ELSE%d)\n", level, level);
		}

		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "ENQUANTO%d=%s\n", level, cmd->val.ifelse.cond_id);

		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "ENQUANTO ENQUANTO%d FACA\n", level);
		provol_pc_cmds(out, cmd->val.ifelse.if_body, level+1);
		if (cmd->val.ifelse.else_body != NULL) {
			for (i = 0; i < level; ++i)
				fputc('\t', out);
			fprintf(out, "ZERO(ELSE%d)\n", level);
		}
		for (i = 0; i < level; ++i)
			fputc('\t', out);
		fprintf(out, "ZERO(ENQUANTO%d)\n", level);
		fprintf(out, "FIM\n");

		if (cmd->val.ifelse.else_body != NULL) {
			for (i = 0; i < level; ++i)
				fputc('\t', out);
			fprintf(out, "ENQUANTO ELSE%d FACA\n", level);
			provol_pc_cmds(out, cmd->val.ifelse.else_body, level+1);
			for (i = 0; i < level; ++i)
				fputc('\t', out);
			fprintf(out, "ZERO(ELSE%d)\n", level);
			for (i = 0; i < level; ++i)
				fputc('\t', out);
			fprintf(out, "FIM\n");
		}
		break;
	}
}

