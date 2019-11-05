// Copyright 27-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader_tests.h"
#include <assert.h>
#include "tkreader.h"
#include "dmstack.h"

void reader_tests (void) {
  puts("Reader tests:");

  Reader *r = reader_new("*tests*", "1");
  Token *tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  // Arr<Token>
  Arr *a = token_list(tk);
  assert(arr_size(a) == 1);
  tk = arr_get(a, 0);
  assert(token_type(tk) == token_INT);
  assert(token_int(tk) == 1);
  assert(str_eq(token_to_str(tk), "1"));

  r = reader_new("*tests*", "");
  tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  a = token_list(tk);
  assert(arr_size(a) == 0);

  r = reader_new("*tests*", "  -117  ");
  tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  a = token_list(tk);
  assert(arr_size(a) == 1);
  tk = arr_get(a, 0);
  assert(token_type(tk) == token_INT);
  assert(token_int(tk) == -117);
  assert(str_eq(token_to_str(tk), "-117"));

  r = reader_new("*tests*", "  -0.17  ");
  tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  a = token_list(tk);
  assert(arr_size(a) == 1);
  tk = arr_get(a, 0);
  assert(token_type(tk) == token_FLOAT);
  assert(token_float(tk) == -0.17);
  assert(str_eq(token_to_str(tk), "-0.17"));

  r = reader_new("*tests*", "  \"abc\"  ");
  tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  a = token_list(tk);
  assert(arr_size(a) == 1);
  tk = arr_get(a, 0);
  assert(token_type(tk) == token_STRING);
  assert(str_eq(token_string(tk), "abc"));
  assert(str_eq(token_to_str(tk), "abc"));

  r = reader_new("*tests*", "  \" a\\\"bb\\\"\\nc@\\t\"  ");
  tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  a = token_list(tk);
  assert(arr_size(a) == 1);
  tk = arr_get(a, 0);
  assert(token_type(tk) == token_STRING);
  assert(str_eq(token_string(tk), " a\"bb\"\nc@\t"));
  assert(str_eq(token_to_str(tk), " a\"bb\"\nc@\t"));

  r = reader_new("*tests*", "-");
  tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  a = token_list(tk);
  assert(arr_size(a) == 1);
  tk = arr_get(a, 0);
  assert(token_type(tk) == token_SYMBOL);
  assert(str_eq(symbol_to_str(token_symbol(tk)), "-"));
  assert(str_eq(token_to_str(tk), "'-'"));

  r = reader_new("*tests*", "  >=  ");
  tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  a = token_list(tk);
  assert(arr_size(a) == 1);
  tk = arr_get(a, 0);
  assert(token_type(tk) == token_SYMBOL);
  assert(str_eq(symbol_to_str(token_symbol(tk)), ">="));
  assert(str_eq(token_to_str(tk), "'>='"));

  r = reader_new("*tests*", " pop ");
  tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  a = token_list(tk);
  assert(arr_size(a) == 1);
  tk = arr_get(a, 0);
  assert(token_type(tk) == token_SYMBOL);
  assert(str_eq(symbol_to_str(token_symbol(tk)), "pop"));
  assert(str_eq(token_to_str(tk), "'pop'"));

  r = reader_new("*tests*", " () ");
  tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  a = token_list(tk);
  assert(arr_size(a) == 1);
  tk = arr_get(a, 0);
  assert(token_type(tk) == token_LIST);
  assert(arr_size(token_list(tk)) == 0);
  assert(str_eq(token_to_str(tk), "()"));

  r = reader_new("*tests*", " ( a ) ");
  tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  a = token_list(tk);
  assert(arr_size(a) == 1);
  tk = arr_get(a, 0);
  assert(token_type(tk) == token_LIST);
  assert(arr_size(token_list(tk)) == 1);
  assert(str_eq(token_to_str(tk), "('a')"));

  r = reader_new("*tests*", " ( 1,, 2 : \"a\" ) ");
  tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  a = token_list(tk);
  assert(arr_size(a) == 1);
  tk = arr_get(a, 0);
  assert(token_type(tk) == token_LIST);
  assert(arr_size(token_list(tk)) == 3);
  assert(str_eq(token_to_str(tk), "(1, 2, a)"));

  r = reader_new("*tests*", " ( ' (23 , 2 ) : 1.5 ) ");
  tk = reader_process(r);
  assert(token_type(tk) == token_LIST);
  a = token_list(tk);
  assert(arr_size(a) == 1);
  tk = arr_get(a, 0);
  assert(token_type(tk) == token_LIST);
  assert(arr_size(token_list(tk)) == 3);

  assert(str_eq(token_to_str(tk), "(''', (23, 2), 1.5)"));

  puts("    Finished");
}
