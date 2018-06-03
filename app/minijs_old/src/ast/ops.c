// Copyright 28-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/ops.h"
#include "dmc/str.h"
#include "string.h"

#define U1 OPS_NOT  OPS_BNOT  OPS_MINUS
#define U2 OPS_INC " " OPS_DEC
#define B1 OPS_PLUS OPS_SUB OPS_MUL OPS_DIV OPS_PER OPS_POW\
 OPS_GT OPS_LT OPS_AND OPS_OR
#define B2 OPS_EQ " " OPS_NEQ " " OPS_LEQ " " OPS_GEQ " " OPS_XOR " " OPS_SR3\
  " " OPS_SL " " OPS_SR " " OPS_LAND " " OPS_LOR " " OPS_NIL
#define A1 OPS_ASSIGN
#define A2 OPS_PLUS_ASSIGN " " OPS_MINUS_ASSIGN " " OPS_MUL_ASSIGN\
  " " OPS_DIV_ASSIGN " " OPS_PER_ASSIGN " " OPS_POW_ASSIGN " " OPS_AND_ASSIGN\
  " " OPS_OR_ASSIGN

inline
char *ops_u1(void) {
  return U1;
}

inline
char *ops_u2(void) {
  return U2;
}

inline
char *ops_b1(void) {
  return B1;
}

inline
char *ops_b2(void) {
  return B2;
}

inline
char *ops_a1(void) {
  return A1;
}

inline
char *ops_a2(void) {
  return A2;
}

int ops_order(char *op) {
  switch(*(op + 1)) {
  case ':': return -1;
  case '=': return 4;
  case '<':
  case '>': return 3;
  case '^': return 6;
  case '&': return 8;
  case '|': return 9;
  }

  switch (*op) {
  case '<':
  case '>': return 4;
  case '+':
  case '-': return 2;
  case '*':
  case '/':
  case '%': return 1;
  case '&': return 5;
  case '|': return 7;
  }
  return 0; // ^
}

inline
bool ops_is_bool(char *op) {
  int order = ops_order(op);
  return order == 4 || order > 7;
}

#undef U1
#undef U2
#undef B1
#undef B2
#undef B1
#undef A2
