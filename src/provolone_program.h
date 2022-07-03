#ifndef _PROVOL_PROG_H
#define _PROVOL_PROG_H

#include "llist.h"

/* ENUM para os tipos de comando que existem */
enum provol_cmd_t	{ P_WLOOP, P_DOLOOP, P_ASSIGN, P_ASSIGN_E, P_CALL, P_IF };

/* ENUM para os estados dos símbolos possíveis: não definido, variável não
 * inicializada, variável inicializada e função */
enum provol_symb_s	{ P_UNDEF, P_VAR_U, P_VAR_I, P_FUN, };

/* ENUM para tipo da variável */
enum provol_var_k	{ P_IN, P_OUT, };

typedef struct provol_program		ProvolProgram;		/* struct que representa o programa completo */

/* typedefs para os enums acima */
typedef enum	provol_cmd_t		ProvolCmd_t;
typedef enum	provol_symb_s		ProvolSymbS;
typedef enum	provol_var_k		ProvolVar_k;

typedef struct	provol_var			ProvolVar;			/* struct que representa variável */
typedef struct 	provol_cmd			ProvolCmd;			/* struct que representa um comand */

typedef struct provol_wloop			ProvolWloop;		/* while loop */
typedef struct provol_doloop		ProvolDoloop;		/* repita n vezes loop */
typedef struct provol_assign		ProvolAssign;		/* assigment de variável a variável */
typedef struct provol_assign_expr	ProvolAssignExpr;	/* assignment de expressão a variável */
typedef struct provol_call			ProvolCall;			/* call de função */
typedef struct provol_if			ProvolIf;			/* if/else */
typedef struct provol_expr			ProvolExpr;			/* expressão */

struct provol_program {
	LinkedList	*in, *out, *loc;
	LinkedList 	*cmds;
	LinkedList	*funs;
	const char	*id;
	int			is_bootstrapped;
};

struct provol_var {
	const char	*id;
	int			is_init;
};

struct provol_wloop {
	const char	*cond_id;
	LinkedList	*body;
};

struct provol_doloop {
	const char	*times;
	LinkedList	*body;
};

struct provol_assign {
	const char	*dest, *src;
};

struct provol_assign_expr {
	const char	*dest;
	ProvolExpr	*expr;
};

struct provol_call {
	const char	*fun, *arg;
};

struct provol_if {
	const char	*cond_id;
	LinkedList	*if_body, *else_body;
};

struct provol_expr {
	const char	*id1, *id2;
	char		op;
};

struct provol_cmd {
	union {
		ProvolWloop wloop;
		ProvolDoloop doloop;
		ProvolAssign assign;
		ProvolAssignExpr assign_e;
		ProvolCall call;
		ProvolIf ifelse;
	} val;
	ProvolCmd_t	type;
};

ProvolProgram *provol_prog_create(void);
void provol_prog_free(ProvolProgram *p);

void provol_program_set_name(ProvolProgram *p, const char *id);

void provol_prog_add_ins(ProvolProgram *p, LinkedList *vars);
void provol_prog_add_outs(ProvolProgram *p, LinkedList *vars);
void provol_prog_add_cmds(ProvolProgram *p, LinkedList *cmds);

LinkedList *provol_var_append(LinkedList *vars, const char *id);
LinkedList *provol_cmds_append(LinkedList *cmds, ProvolCmd *cmd);

ProvolCmd *provol_wloop_new(const ProvolProgram *p, const char *cond_id, LinkedList *body);
ProvolCmd *provol_doloop_new(const ProvolProgram *p, const char *times, LinkedList *body);
ProvolCmd *provol_assign_new(const ProvolProgram *p, const char *dest, const char *src);
ProvolCmd *provol_assign_expr_new(const ProvolProgram *p, const char *dest, ProvolExpr *expr);
ProvolCmd *provol_call_new(const ProvolProgram *p, const char *fun, const char *arg);
ProvolCmd *provol_if_new(const ProvolProgram *p, const char *cond_id, LinkedList *if_body, LinkedList *else_body);

ProvolExpr *provol_expr_new(const ProvolProgram *p, const char *id1, const char *id2, const char op);

void provol_prog_print_tree(const ProvolProgram *p);

#endif

