// Copyright 28-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Operators

#ifndef AST_OPS_H
  #define AST_OPS_H

#include "stdbool.h"

#define OPS_NOT "!"
#define OPS_BNOT "~"
#define OPS_MINUS "-"

#define OPS_INC "++"
#define OPS_DEC "--"

#define OPS_PLUS "+"
#define OPS_SUB "-"
#define OPS_MUL "*"
#define OPS_DIV "/"
#define OPS_PER "%"
#define OPS_POW "^"
#define OPS_GT ">"
#define OPS_LT "<"
#define OPS_AND "&"
#define OPS_OR "|"

#define OPS_EQ "=="
#define OPS_NEQ "!="
#define OPS_LEQ "<="
#define OPS_GEQ ">="
#define OPS_XOR "^^"
#define OPS_SR3 ">>>"
#define OPS_SL "<<"
#define OPS_SR ">>"
#define OPS_NULL ":?"

#define OPS_ASSIGN "="
#define OPS_PLUS_ASSIGN "+="
#define OPS_MINUS_ASSIGN "-="
#define OPS_MUL_ASSIGN "*="
#define OPS_DIV_ASSIGN "/="
#define OPS_PER_ASSIGN "%="
#define OPS_POW_ASSIGN "^="
#define OPS_AND_ASSIGN "&="
#define OPS_OR_ASSIGN "|="

/// ops_u1 returns "!~"
char *ops_u1(void);

/// ops_u returns "++ --"
char *ops_u2(void);

/// ops_b1 returns "+-*/%^><&|"
char *ops_b1(void);

/// ops_b returns "== != <= >= ^^ >>> << >> ?:"
char *ops_b2(void);

/// ops_b returns "="
char *ops_a1(void);

/// ops_b returns "+= -= *= /= %= ^= &= |= "
char *ops_a2(void);

#endif
