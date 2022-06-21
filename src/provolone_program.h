#ifndef _PROVOL_PROG_H
#define _PROVOL_PROG_H

enum provol_cmd_t	{ P_WLOOP, P_ASSIGN, P_CALL, };
enum provol_symb_s	{ P_UNDEF, P_VAR_U, P_VAR_I, P_FUN, };
enum provol_var_k	{ P_IN, P_OUT, };

typedef struct provol_program	ProvolProgram;

typedef char					*ProvolId;
typedef enum	provol_cmd_t	ProvolCmd_t;
typedef enum	provol_symb_s	ProvolSymbS;
typedef enum	provol_var_k	ProvolVar_k;
typedef struct	provol_sym		ProvolSym;

typedef struct	provol_var		ProvolVar;
typedef struct	provol_fun		ProvolFun;
typedef struct 	provol_cmd		ProvolCmd;

typedef struct provol_wloop		ProvolWloop;
typedef struct provol_assign	ProvolAssign;
typedef struct provol_call		ProvolCall;

struct provol_program {
	ProvolVar	*in, *out;
	ProvolCmd 	*cmds;
	ProvolFun	*funs;
};

struct provol_sym {
	ProvolId	id;
	ProvolSym	*next;
};

struct provol_cmd {
	ProvolCmd	*next;
	void		*val;
	ProvolCmd_t	type;
};

struct provol_var {
	ProvolId	name;
	ProvolVar	*next;
	int			is_init;
};

struct provol_fun {
	ProvolFun *next;
	ProvolId name;
};

struct provol_wloop {
	ProvolId	cond;
	ProvolCmd	*body;
};

struct provol_assign {
	ProvolId	src, dest;
};

struct provol_call {
	ProvolId	fun, arg;
};

ProvolProgram *provol_program_create(void);
void provol_program_free(ProvolProgram *p);

int provol_program_in_new(ProvolProgram *p, const ProvolId id);
int provol_program_out_new(ProvolProgram *p, const ProvolId id);
int provol_program_cmd_new(ProvolProgram *p, ProvolCmd *cmd);
int provol_program_fun_new(ProvolProgram *p, ProvolId name);
int provol_program_add_vars(ProvolProgram *p, ProvolSym *s, ProvolVar_k k);

void provol_fun_free(ProvolFun *fun);

ProvolSym *provol_sym_list_new(void);
ProvolSym *provol_sym_list_push(ProvolSym *s, ProvolId sym);
ProvolSym *provol_sym_list_pop(ProvolSym *s, ProvolId *sym);
void provol_sym_list_free(ProvolSym *s);

ProvolCmd *provol_cmd_new(const ProvolCmd_t type, void *val);
void provol_cmd_free(ProvolCmd *cmd);

ProvolWloop *provol_wloop_new(const ProvolId cond, ProvolCmd *body);
void provol_wloop_free(ProvolWloop *wl);

ProvolAssign *provol_assign_new(const ProvolId src, const ProvolId dest);
void provol_assign_free(ProvolAssign *as);

ProvolCall *provol_call_new(const ProvolId fun, const ProvolId arg);
void provol_call_free(ProvolCall *c);

ProvolSymbS provol_program_check_symbol(const ProvolProgram *p, const ProvolId sym);

#endif

