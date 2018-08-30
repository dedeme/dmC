// Copyright 28-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "types/ast.h"
#include "dmc/Tuples.h"
#include "dmc/Arr.h"

AstTp *ast_tp_new(void *context, AstFn f) {
  return (AstTp *)tp_new(context, f);
}

void *ast_tp_context(AstTp *this) {
  return tp_e1((Tp *)this);
}

AstFn ast_tp_fn(AstTp *this) {
  return tp_e2((Tp *)this);
}

AstArr *ast_arr_new() {
  return (AstArr *)arr_new();
}

AstMsg *ast_msg_new(void *context, AstFn fn, char *msg) {
  return (AstMsg *)tp3_new(context, fn, msg);
}

char *ast_msg_msg(AstMsg *this) {
  return tp3_e3((Tp3 *)this);
}

/// Cast to AstTp which allows call to 'ast_tp_context' and 'ast_tp_fn'.
AstTp *ast_msg_cast(AstMsg *this) {
  return (AstTp *)this;
}

void ast_arr_add(AstArr *this, AstTp *tp) {
  arr_add((Arr *)this, tp);
}

It/*AstTp*/ *ast_arr_cast(AstArr *this) {
  return arr_to_it((Arr *)this);
}
