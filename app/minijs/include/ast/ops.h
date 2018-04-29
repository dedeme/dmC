// Copyright 28-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Operators

#ifndef AST_OPS_H
  #define AST_OPS_H

#include "stdbool.h"

#define OPS_NOT "!"

#define OPS_INC "++"
#define OPS_DEC "--"

#define OPS_PLUS "+"
#define OPS_MINUS "-"
#define OPS_MUL "*"
#define OPS_DIV "/"
#define OPS_PER "%"
#define OPS_POW "^"
#define OPS_GT ">"
#define OPS_LT "<"
#define OPS_AND1 "&"
#define OPS_OR1 "|"

#define OPS_EQ "=="
#define OPS_NEQ "!="
#define OPS_LEQ "<="
#define OPS_GEQ ">="
#define OPS_ADN2 "&&"
#define OPS_OR2 "||"
#define OPS_XOR "^^"
#define OPS_RR3 ">>>"
#define OPS_RL "<<"
#define OPS_RR ">>"
#define OPS_NULL ":?"

///
void ops_init(void);

/// ops_u1 returns "!"
char *ops_u1(void);

/// ops_u returns "++ --"
char *ops_u(void);

/// ops_b1 returns "+-*/%^><&|"
char *ops_b1(void);

/// ops_b returns "== != <= >= && || ^^ >>> << >> ?:"
char *ops_b(void);

///
bool ops_is_not(char *op);

///
bool ops_is_plus(char *op);

///
bool ops_is_null(char *op);

///
bool ops_is_logic(char *op);

///
bool ops_is_cmp(char *op);

#endif
