// Copyright 28-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/ops.h"
#include "dmc/str.h"
#include "string.h"

static char *u1;
static char *u;
static char *b1;
static char *b;
static char *logic;

void ops_init(void) {
  u1 = OPS_NOT;
  u = str_cat(OPS_INC, " ", OPS_DEC, NULL);
  b1 = str_cat(
    OPS_PLUS, OPS_MINUS, OPS_MUL, OPS_DIV, OPS_PER, OPS_POW,
    OPS_GT, OPS_LT, OPS_AND1, OPS_OR1, NULL
  );
  b = str_cat(
    OPS_EQ, " ", OPS_NEQ, " ", OPS_LEQ, " ", OPS_GEQ, " ",
    OPS_ADN2, " ", OPS_OR2, " ", OPS_XOR, " ", OPS_RR3, " ", OPS_RL, " ",
    OPS_RR, " ", OPS_NULL, NULL
  );
  logic = str_cat(OPS_ADN2, " ", OPS_OR2, " ", OPS_XOR, NULL);
}

inline
char *ops_u1(void) {
  return u1;
}

inline
char *ops_u(void) {
  return u;
}

inline
char *ops_b1(void) {
  return b1;
}

char *ops_b(void) {
  return b;
}

inline
bool ops_is_not(char *op) {
  return op[0] == *OPS_NOT && !op[1];
}

inline
bool ops_is_plus(char *op) {
  return op[0] == *OPS_PLUS && !op[1];
}

inline
bool ops_is_null(char *op) {
  return op[0] == *OPS_NULL && !op[1];
}

inline
bool ops_is_logic(char *op) {
  char ch = op[1];
  return ch == '&' || ch == '|' || ch == '^';
}

inline
bool ops_is_cmp(char *op) {
  char ch = *op;
  return ch == '<' || ch == '>' || op[1] == '=';
}
