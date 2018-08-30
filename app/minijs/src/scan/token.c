// Copyright 27-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "scan/token.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/Opt.h"
#include "dmc/List.h"
#include "dmc/Tuples.h"
#include "dmc/Buf.h"
#include "dmc/ct/Achar.h"
#include "scan/logic.h"
#include "types/ast.h"

//                 __
static ScanResult *ok(void *result, Scanner *sc) {
  return scanResult_new_ok(result, sc);
}

//                 ____
static ScanResult *error(Fail *e) {
  return scanResult_error(fails_new_error(e));
}

//            ________
static Fail *mk_error(Scanner *sc, char *msg) {
  return scanner_mk_fail(sc, msg);
}

ScanResult *token_char(char *s, Scanner *sc) {
  if (!*s || str_len(s) > 2) {
    exc_illegal_argument("s", "string empty o with 3 o more characters",
      "string with one o two characters");
  }
  if (s[1]) {
    char ch2 = s[1];
    char ch_sc = scanner_char(sc);
    if (ch_sc >= *s && ch_sc <= ch2) {
      char *r = str_copy("_");
      r[0] = ch_sc;
      return ok(r, scanner_next(sc));
    }
    return error(mk_error(sc, str_printf(
      "Excected a character between '%c' and '%c'", *s, s[1]
    )));
  }
  if (*s != scanner_char(sc)) {
    return error(mk_error(sc, str_printf("Expected '%c'", *s)));
  }
  return ok(s, scanner_next(sc));
}

ScanResult *token_blank(Opt *none, Scanner *sc) {

  // '\01- '
  AstTp *p0 = ast_tp_new("\x01 ", (AstFn)token_char);

  // '/'
  AstTp *p10 = ast_tp_new("/", (AstFn)token_char);
  // '\n'
  AstMsg *p1100 = ast_msg_new("\n", (AstFn)token_char, "'\n' found");
  // ~'\n'
  AstTp *p110 = ast_tp_new(p1100, (AstFn)logic_not);
  // (~'\n')*
  AstTp *p11 = ast_tp_new(p110, (AstFn)logic_none_or_several);
  AstArr *a1 = ast_arr_new();
  ast_arr_add(a1, p10);
  ast_arr_add(a1, p10);
  ast_arr_add(a1, p11);
  // '/' '/' (~'\n')*
  AstTp *p1 = ast_tp_new(a1, (AstFn)logic_and);

  // '*'
  AstTp *p20 = ast_tp_new("*", (AstFn)token_char);
  AstArr *a2100 = ast_arr_new();
  ast_arr_add(a2100, p20);
  ast_arr_add(a2100, p10);
  // '*' '/'
  AstMsg *p2100 = ast_msg_new(a2100, (AstFn)logic_and, "'*/' found");
  // ~('*' '/')
  AstTp *p210 = ast_tp_new(p2100, (AstFn)logic_not);
  // (~('*' '/')*
  AstTp *p21 = ast_tp_new(p210, (AstFn)logic_none_or_several);
  AstArr *a2 = ast_arr_new();
  ast_arr_add(a2, p10);
  ast_arr_add(a2, p20);
  ast_arr_add(a2, p21);
  ast_arr_add(a2, p20);
  ast_arr_add(a2, p10);
  // '/' '*' (~('*' '/')* '*' '/'
  AstTp *p2 = ast_tp_new(a2, (AstFn)logic_and);

  AstArr *a0 = ast_arr_new();
  ast_arr_add(a0, p0);
  ast_arr_add(a0, p1);
  ast_arr_add(a0, p2);
  // [ '\01- ' | '/' '/' (~'\n')* | '/' '*' (~('*' '/')* '*' '/' ]
  return logic_or(a0, sc);
}

ScanResult *token_blanks(Opt *none, Scanner *sc) {
  AstTp *tp = ast_tp_new(opt_null(), (AstFn)token_blank);
  return logic_none_or_several(tp, sc);
}

ScanResult *token_blank1(Opt *none, Scanner *sc) {
  AstTp *tp = ast_tp_new(opt_null(), (AstFn)token_blank);
  return logic_one_or_more(tp, sc);
}

ScanResult *token_id(Opt *none, Scanner *sc) {
  // ['a-z' | 'A-Z' | '$' | '_']
  ScanResult *first(Opt *none, Scanner *sc) {
    // 'a-z'
    AstTp *p0 = ast_tp_new("az", (AstFn)token_char);
    // 'A-Z'
    AstTp *p1 = ast_tp_new("AZ", (AstFn)token_char);
    // '$'
    AstTp *p2 = ast_tp_new("$", (AstFn)token_char);
    // '_'
    AstTp *p3 = ast_tp_new("_", (AstFn)token_char);
    AstArr *a = ast_arr_new();
    ast_arr_add(a, p0);
    ast_arr_add(a, p1);
    ast_arr_add(a, p2);
    ast_arr_add(a, p3);
    // ['a-z' | 'A-Z' | '$' | '_']
    return logic_or(a, sc);
  }

  // ['a-z' | 'A-Z' | '$' | '_']
  AstTp *p0 = ast_tp_new(opt_null(), (AstFn)first);
  // '0-9'
  AstTp *p101 = ast_tp_new("09", (AstFn)token_char);
  AstArr *a10 = ast_arr_new();
  ast_arr_add(a10, p0);
  ast_arr_add(a10, p101);
  // ['a-z' | 'A-Z' | '$' | '_' | '0-9']
  AstTp *p10 = ast_tp_new(a10, (AstFn)logic_or);
  // ['a-z' | 'A-Z' | '$' | '_' | '0-9']*
  AstTp *p1 = ast_tp_new(p10, (AstFn)logic_one_or_more);
  AstArr *a = ast_arr_new();
  ast_arr_add(a, p0);
  ast_arr_add(a, p1);
  // ['a-z' | 'A-Z' | '$' | '_'] [ 'a-z' | 'A-Z' | '$' | '_' | '0-9' ]*
  ScanResult *scr = logic_and(a, sc);
  if (scanResult_is_error(scr)) {
    return scr;
  }
  Arr *ars = (Arr *)scanResult_result(scr);
  Buf *bf = buf_new();
  buf_add(bf, arr_get(ars, 0));
  buf_add(bf, str_join(achar_to_it((Achar *)arr_get(ars, 1)), ""));
  return scanResult_new_ok(buf_to_str(bf), scanResult_scanner(scr));
}

ScanResult *token_end(Opt *none, Scanner *sc) {
  if (scanner_is_end(sc)) {
    return ok(opt_null(), sc);
  }
  return error(mk_error(sc, "Expected end of text"));
}
