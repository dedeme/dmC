// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader/pt_sq_pr_reader.h"
#include "kut/buf.h"
#include "DEFS.h"
#include "reader/ex_reader.h"

Exp *pt_sq_pr_reader_read (Types *tps, Exp *left, Cdr *cdr) {
  if (cdr_next_token_is_point(cdr)) { // . ------------------------------------
    Token *point = cdr_read_token(cdr);
    Exp *exp = ex_reader_read1(tps, cdr);

    if (exp_is_sym(exp)) {
      if (exp_is_sym(left)) {
        char *module = types_find(tps, exp_get_sym(left));
        if (*module) {
          if (cdr_next_token_is_open_parenthesis(cdr)) {
            Token *open_pr = cdr_read_token(cdr);

            // <Exp>
            Arr *exps = arr_new();
            char *ob_sym = exp_get_sym(left);
            arr_push(exps, exp_sym(ob_sym, ob_sym));
            char *left_js = str_f("%s%s%s",
              exp_js_replace(left, module), point->js, exp_get_js(exp)
            );
            left = exp_pt(exp_sym(module, module), exp, left_js);

            Buf *js = buf_new();
            if (cdr_next_token_is_close_parenthesis(cdr)) {
              Token *close_pr = cdr_read_token(cdr);
              buf_add(js, str_f("%s%s%s", open_pr->js, ob_sym, close_pr->js));
            } else {
              buf_add(js, open_pr->js);
              buf_add(js, ob_sym);
              buf_cadd(js, ',');
              for (;;) {
                Exp *exp = ex_reader_read(tps, cdr);
                arr_push(exps, exp);
                buf_add(js, exp_get_js(exp));
                Token *tk = cdr_read_token(cdr);
                if (token_is_close_par(tk)) {
                  buf_add(js, tk->js);
                  break;
                } else if (token_is_comma(tk)) {
                  buf_add(js, tk->js);
                  continue;
                }
                else EXC_GENERIC(cdr_fail_expect(cdr, "')' or ','", token_to_str(tk)));
              }
            }

            return pt_sq_pr_reader_read(
              tps, exp_pr(left, exps, str_f("%s%s", left_js, buf_str(js))), cdr
            );
          }
          if (cdr_next_token_is_hash(cdr)) cdr_read_token(cdr);
          char *insq_js = str_f("%s.%s", module, exp_get_js(exp));
          Exp *insq = exp_pt(exp_sym(module, module), exp, insq_js);
          char *js = str_f("%s%s[%s]",
            exp_get_js(left), str_left(point->js, -1), insq_js
          );
          return pt_sq_pr_reader_read(
            tps, exp_sq(left, insq, js), cdr
          );
        }
      }
      char *js = str_f("%s%s%s",
        exp_get_js(left), point->js, exp_get_js(exp)
      );
      return pt_sq_pr_reader_read(tps, exp_pt(left, exp, js), cdr);
    }
    EXC_GENERIC(cdr_fail_expect(cdr, "symbol", exp_to_str(exp)));
  }

  if (cdr_next_token_is_exclamation(cdr)) { // ! -------------------------------
    Token *exclamation = cdr_read_token(cdr);
    char *js = str_f("%s%s[0]",
      exp_get_js(left), str_left(exclamation->js, -1)
    );
    return pt_sq_pr_reader_read(tps, exp_sq(left, exp_int(0, "0"), js), cdr);
  }

  if (cdr_next_token_is_open_square(cdr)) { // [ -------------------------------
    Token *open_sq = cdr_read_token(cdr);
    if (cdr_next_token_is_colon(cdr)) {
      Token *colon = cdr_read_token(cdr);
      if (cdr_next_token_is_close_square(cdr)) { // [:]
        Token *close_sq = cdr_read_token(cdr);
        char *js = str_f("%s%s%s%s%s%s%s",
          exp_js_insert(left, "sys.$slice("),
          str_left(open_sq->js, -1), ",null",
          str_left(colon->js, -1), ",null",
          str_left(close_sq->js, -1), ")"
        );
        return pt_sq_pr_reader_read(
          tps, exp_slice(left, opt_none(), opt_none(), js), cdr
        );
      }
      Exp *exp2 = ex_reader_read(tps, cdr);
      Token *tk = cdr_read_token(cdr);
      if (token_is_close_square(tk)) { // [:e]
        char *js = str_f("%s%s%s%s%s%s%s%s",
          exp_js_insert(left, "sys.$slice("),
          str_left(open_sq->js, -1), ",null",
          str_left(colon->js, -1), ",",
          exp_get_js(exp2),
          str_left(tk->js, -1), ")"
        );
        return pt_sq_pr_reader_read(
          tps, exp_slice(left, opt_none(), opt_some(exp2), js), cdr
        );
      }
      EXC_GENERIC(cdr_fail_expect(cdr, "']'", token_to_str(tk)));
    }
    Exp *exp1 = ex_reader_read(tps, cdr);
    if (cdr_next_token_is_close_square(cdr)) { // [e]
      Token *close_sq = cdr_read_token(cdr);
        char *js = str_f("%s%s%s%s",
          exp_get_js(left), open_sq->js, exp_get_js(exp1), close_sq->js
        );
      return pt_sq_pr_reader_read(tps, exp_sq(left, exp1, js), cdr);
    }
    if (cdr_next_token_is_colon(cdr)) {
      Token *colon = cdr_read_token(cdr);
      if (cdr_next_token_is_close_square(cdr)) { // [e:]
        Token *close_sq = cdr_read_token(cdr);
        char *js = str_f("%s%s%s%s%s%s%s%s",
          exp_js_insert(left, "sys.$slice("),
          str_left(open_sq->js, -1), ",",
          exp_get_js(exp1),
          str_left(colon->js, -1), ",null",
          str_left(close_sq->js, -1), ")"
        );
        return pt_sq_pr_reader_read(
          tps, exp_slice(left, opt_some(exp1), opt_none(), js), cdr
        );
      }
      Exp *exp2 = ex_reader_read(tps, cdr);
      Token *tk = cdr_read_token(cdr);
      if (token_is_close_square(tk)) { // [e1:e2]
        char *js = str_f("%s%s%s%s%s%s%s%s%s",
          exp_js_insert(left, "sys.$slice("),
          str_left(open_sq->js, -1), ",",
          exp_get_js(exp1),
          str_left(colon->js, -1), ",",
          exp_get_js(exp2),
          str_left(tk->js, -1), ")"
        );
        return pt_sq_pr_reader_read(
          tps, exp_slice(left, opt_some(exp1), opt_some(exp2), js), cdr
        );
      }
      EXC_GENERIC(cdr_fail_expect(cdr, "']'", token_to_str(tk)));
    }
    EXC_GENERIC(cdr_fail_expect(
      cdr, "':' or ']'", token_to_str(cdr_read_token(cdr))
    ));
  }
  if (cdr_next_token_is_open_parenthesis(cdr)) { // ( --------------------------
    Token *open_pr = cdr_read_token(cdr);
    // <Exp>
    Arr *exps = arr_new();
    Buf *js = buf_new();
    buf_add(js, exp_get_js(left));
    if (cdr_next_token_is_close_parenthesis(cdr)) {
      Token *close_pr = cdr_read_token(cdr);
      buf_add(js, str_f("%s%s", open_pr->js, close_pr->js));
    } else {
      buf_add(js, open_pr->js);
      for (;;) {
        Exp *exp = ex_reader_read(tps, cdr);
        arr_push(exps, exp);
        buf_add(js, exp_get_js(exp));
        Token *tk = cdr_read_token(cdr);
        if (token_is_close_par(tk)) {
          buf_add(js, tk->js);
          break;
        } else if (token_is_comma(tk)) {
          buf_add(js, tk->js);
          continue;
        }
        else EXC_GENERIC(cdr_fail_expect(cdr, "')' or ','", token_to_str(tk)));
      }
    }

    return pt_sq_pr_reader_read(tps, exp_pr(left, exps, buf_str(js)), cdr);
  }
  return left;
}
