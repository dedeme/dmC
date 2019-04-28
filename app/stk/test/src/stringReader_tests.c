// Copyright 16-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "stringReader_tests.h"
#include "assert.h"
#include "readers/StringReader.h"

void stringReader_tests(void) {
  puts("StringReader tests:");

  StringReader *srd = stringReader_new("");
  TkReader *rd = stringReader_reader(srd);
  Token *tk = tkReader_next_new(rd);
  assert(!*token_value(tk));
  token_free(tk);
  stringReader_free(srd);

  srd = stringReader_new(
    " 3 2 +  \n pop \"a\tb\" \"a\nbc \"\"\"ab\nc\"\"\" d"
  );
  rd = stringReader_reader(srd);

  tk = tkReader_next_new(rd);
  assert(str_eq(token_value(tk), "3"));
  assert(token_nline(tk) == 1);
  assert(token_nchar(tk) == 1);
  token_free(tk);

  tk = tkReader_next_new(rd);
  assert(str_eq(token_value(tk), "2"));
  assert(token_nline(tk) == 1);
  assert(token_nchar(tk) == 3);
  token_free(tk);

  tk = tkReader_next_new(rd);
  assert(str_eq(token_value(tk), "+"));
  assert(token_nline(tk) == 1);
  assert(token_nchar(tk) == 5);
  token_free(tk);

  tk = tkReader_next_new(rd);
  assert(str_eq(token_value(tk), "pop"));
  assert(token_nline(tk) == 2);
  assert(token_nchar(tk) == 1);
  token_free(tk);

  tk = tkReader_next_new(rd);
  assert(str_eq(token_value(tk), "\"a\tb\""));
  assert(token_nline(tk) == 2);
  assert(token_nchar(tk) == 5);
  token_free(tk);

  tk = tkReader_next_new(rd);
  assert(str_eq(token_value(tk), "\"a"));
  assert(token_nline(tk) == 2);
  assert(token_nchar(tk) == 11);
  token_free(tk);

  tk = tkReader_next_new(rd);
  assert(str_eq(token_value(tk), "bc"));
  assert(token_nline(tk) == 3);
  assert(token_nchar(tk) == 0);
  token_free(tk);

  tk = tkReader_next_new(rd);
  assert(str_eq(token_value(tk), "\"\"\"ab\nc\"\"\""));
  assert(token_nline(tk) == 3);
  assert(token_nchar(tk) == 3);
  token_free(tk);

  tk = tkReader_next_new(rd);
  assert(str_eq(token_value(tk), "d"));
  assert(token_nline(tk) == 4);
  assert(token_nchar(tk) == 5);
  token_free(tk);

  tk = tkReader_next_new(rd);
  assert(!*token_value(tk));
  token_free(tk);

  tk = tkReader_next_new(rd);
  assert(!*token_value(tk));
  token_free(tk);

  stringReader_free(srd);

  puts("    Finished");
}

