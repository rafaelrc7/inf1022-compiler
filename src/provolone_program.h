#ifndef _PROVOL_PROG_H
#define _PROVOL_PROG_H

#include "llist.h"

enum provol_cmd_t	{ P_WLOOP, P_ASSIGN, P_CALL, };
enum provol_symb_s	{ P_UNDEF, P_VAR_U, P_VAR_I, P_FUN, };
enum provol_var_k	{ P_IN, P_OUT, };

typedef struct provol_program	ProvolProgram;

typedef enum	provol_cmd_t	ProvolCmd_t;
typedef enum	provol_symb_s	ProvolSymbS;
typedef enum	provol_var_k	ProvolVar_k;

typedef struct	provol_var		ProvolVar;
typedef struct 	provol_cmd		ProvolCmd;

typedef struct provol_wloop		ProvolWloop;
typedef struct provol_assign	ProvolAssign;
typedef struct provol_call		ProvolCall;

struct provol_program {
	LinkedList	*in, *out, *loc;
	LinkedList 	*cmds;
	LinkedList	*funs;
};

struct provol_var {
	const char	*id;
	int			is_init;
};

struct provol_wloop {
	const char	*cond_id;
	LinkedList	*body;
};

struct provol_assign {
	const char	*dest, *src;
};

struct provol_call {
	const char	*fun, *arg;
};

struct provol_cmd {
	union {
		ProvolWloop wloop;
		ProvolAssign assign;
		ProvolCall call;
	} val;
	ProvolCmd_t	type;
};

ProvolProgram *provol_prog_create(void);
void provol_prog_free(ProvolProgram *p);

void provol_prog_add_ins(ProvolProgram *p, LinkedList *vars);
void provol_prog_add_outs(ProvolProgram *p, LinkedList *vars);
void provol_prog_add_cmds(ProvolProgram *p, LinkedList *cmds);

LinkedList *provol_var_append(LinkedList *vars, const char *id);
LinkedList *provol_cmds_append(LinkedList *cmds, ProvolCmd *cmd);

ProvolCmd *provol_wloop_new(const ProvolProgram *p, const char *cond_id, LinkedList *body);
ProvolCmd *provol_assign_new(const ProvolProgram *p, const char *dest, const char *src);
ProvolCmd *provol_call_new(const ProvolProgram *p, const char *fun, const char *arg);

#endif

