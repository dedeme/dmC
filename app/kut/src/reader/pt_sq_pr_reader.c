// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "symix.h"
#include "reader/pt_sq_pr_reader.h"
#include "reader/ex_reader.h"


Exp *pt_sq_pr_reader_read (Types *tps, Exp *left, Cdr *cdr) {
  if (cdr_next_token_is_point(cdr)) { // . ------------------------------------
    cdr_read_token(cdr);
    Exp *exp = ex_reader_read1(tps, cdr);
    if (exp_is_sym(exp)) {
      if (exp_is_sym(left)) {
        int module = types_find(tps, exp_get_sym(left));
        if (module != -1) {
          if (cdr_next_token_is_open_parenthesis(cdr)) {
            cdr_read_token(cdr);
            Arr *exps = arr_new();
            arr_push(exps, left);
            left = exp_pt(exp_sym(module), exp);

            if (cdr_next_token_is_close_parenthesis(cdr)) {
              cdr_read_token(cdr);
            } else {
              for (;;) {
                Exp *exp = ex_reader_read(tps, cdr);
                arr_push(exps, exp);
                Token *tk = cdr_read_token(cdr);
                if (token_is_close_par(tk)) break;
                else if (token_is_comma(tk)) continue;
                else EXC_KUT(cdr_fail_expect(cdr, "')' or ','", token_to_str(tk)));
              }
            }
            return pt_sq_pr_reader_read(tps, exp_pr(left, exps), cdr);
          }
          if (cdr_next_token_is_hash(cdr)) cdr_read_token(cdr);
          return pt_sq_pr_reader_read(
            tps, exp_sq(left, exp_pt(exp_sym(module), exp)), cdr
          );
        }
      }
      return pt_sq_pr_reader_read(tps, exp_pt(left, exp), cdr);
    }
    EXC_KUT(cdr_fail_expect(cdr, "symbol", exp_to_js(exp)));
  }

  if (cdr_next_token_is_exclamation(cdr)) { // ! -------------------------------
    cdr_read_token(cdr);
    return pt_sq_pr_reader_read(tps, exp_sq(left, exp_int(0)), cdr);
  }

  if (cdr_next_token_is_open_square(cdr)) { // [ -------------------------------
    cdr_read_token(cdr);
    if (cdr_next_token_is_colon(cdr)) {
      cdr_read_token(cdr);
      if (cdr_next_token_is_close_square(cdr)) { // [:]
        cdr_read_token(cdr);
        return pt_sq_pr_reader_read(
          tps, exp_slice(left, exp_empty(), exp_empty()), cdr
        );
      }
      Exp *exp2 = ex_reader_read(tps, cdr);
      Token *tk = cdr_read_token(cdr);
      if (token_is_close_square(tk)) { // [:e]
        return pt_sq_pr_reader_read(
          tps, exp_slice(left, exp_empty(), exp2), cdr
        );
      }
      EXC_KUT(cdr_fail_expect(cdr, "']'", token_to_str(tk)));
    }
    Exp *exp1 = ex_reader_read(tps, cdr);
    if (cdr_next_token_is_close_square(cdr)) { // [e]
      cdr_read_token(cdr);
      return pt_sq_pr_reader_read(tps, exp_sq(left, exp1), cdr);
    }
    if (cdr_next_token_is_colon(cdr)) {
      cdr_read_token(cdr);
      if (cdr_next_token_is_close_square(cdr)) { // [e:]
        cdr_read_token(cdr);
        return pt_sq_pr_reader_read(
          tps, exp_slice(left, exp1, exp_empty()), cdr
        );
      }
      Exp *exp2 = ex_reader_read(tps, cdr);
      Token *tk = cdr_read_token(cdr);
      if (token_is_close_square(tk)) { // [e1:e2]
        return pt_sq_pr_reader_read(tps, exp_slice(left, exp1, exp2), cdr);
      }
      EXC_KUT(cdr_fail_expect(cdr, "']'", token_to_str(tk)));
    }
    EXC_KUT(cdr_fail_expect(
      cdr, "':' or ']'", token_to_str(cdr_read_token(cdr))
    ));
  }
  if (cdr_next_token_is_open_parenthesis(cdr)) { // ( --------------------------
    cdr_read_token(cdr);
    // <Exp>
    Arr *exps = arr_new();
    if (cdr_next_token_is_close_parenthesis(cdr)) {
      cdr_read_token(cdr);
    } else {
      for (;;) {
        Exp *exp = ex_reader_read(tps, cdr);
        arr_push(exps, exp);
        Token *tk = cdr_read_token(cdr);
        if (token_is_close_par(tk)) break;
        else if (token_is_comma(tk)) continue;
        else EXC_KUT(cdr_fail_expect(cdr, "')' or ','", token_to_str(tk)));
      }
    }

    return pt_sq_pr_reader_read(tps, exp_pr(left, exps), cdr);
  }

  return left;
}
