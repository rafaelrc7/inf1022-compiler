#ifndef _PROVOL_COMPILER_H
#define _PROVOL_COMPILER_H

#include <stdio.h>

#include "provolone_program.h"

int provol_cc(FILE *out, const ProvolProgram *p);
int provol_pc(FILE *out, const ProvolProgram *p);

#endif

