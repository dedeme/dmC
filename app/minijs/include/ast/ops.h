// Copyright 28-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Operators.<p<
/// Binary associativity
///   -1: ?:
///   0: ^
///   1: * / %
///   2: + -
///   3: << >> <<<
///   4: == != < <= > >=
///   5: &
///   6: ^^
///   7: |
///   8: &&
///   9: ||
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
#define OPS_LAND "&&"
#define OPS_LOR "||"
#define OPS_NIL "?:"

#define OPS_NOT2 "not"
#define OPS_BNOT2 "bnot"
#define OPS_MINUS2 "minus"

#define OPS_PREINC "preInc"
#define OPS_PREDEC "preInc"
#define OPS_POSTINC "postInc"
#define OPS_POSTDEC "postInc"

#define OPS_PLUS2 "plus"
#define OPS_SUB2 "sub"
#define OPS_MUL2 "mul"
#define OPS_DIV2 "div"
#define OPS_PER2 "per"
#define OPS_POW2 "pow"
#define OPS_GT2 "gt"
#define OPS_LT2 "lt"
#define OPS_AND2 "and"
#define OPS_OR2 "or"

#define OPS_EQ2 "eq"
#define OPS_NEQ2 "neq"
#define OPS_LEQ2 "leq"
#define OPS_GEQ2 "geq"
#define OPS_XOR2 "xor"
#define OPS_SR32 "sr3"
#define OPS_SL2 "sl"
#define OPS_SR2 "sr"
#define OPS_LAND2 "land"
#define OPS_LOR2 "lor"
#define OPS_NIL2 "nil"

#define OPS_ASSIGN "="
#define OPS_PLUS_ASSIGN "+="
#define OPS_MINUS_ASSIGN "-="
#define OPS_MUL_ASSIGN "*="
#define OPS_DIV_ASSIGN "/="
#define OPS_PER_ASSIGN "%="
#define OPS_POW_ASSIGN "^="
#define OPS_AND_ASSIGN "&="
#define OPS_OR_ASSIGN "|="

/// ops_u1 returns "!~-"
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

///
int ops_order(char *op);

///
bool ops_is_bool(char *op);

#endif
