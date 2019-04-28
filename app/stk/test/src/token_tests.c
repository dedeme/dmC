// Copyright 17-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "token_tests.h"

#include "assert.h"
#include "Token.h"

void token_tests (void) {
  puts("Token tests:");

  Token *tk = token_new(0, 0, str_new(""));
  assert(!token_is_id(tk));
  assert(!token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("a"));
  assert(token_is_id(tk));
  assert(!token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("abc"));
  assert(token_is_id(tk));
  assert(!token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("@a"));
  assert(!token_is_id(tk));
  assert(!token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("\"anv"));
  assert(!token_is_id(tk));
  assert(!token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("-a"));
  assert(!token_is_id(tk));
  assert(!token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("!"));
  assert(!token_is_id(tk));
  assert(!token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("[anv"));
  assert(!token_is_id(tk));
  assert(!token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("{a"));
  assert(!token_is_id(tk));
  assert(!token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("("));
  assert(!token_is_id(tk));
  assert(!token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("'a"));
  assert(!token_is_id(tk));
  assert(token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("'abc"));
  assert(!token_is_id(tk));
  assert(token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("'@a"));
  assert(!token_is_id(tk));
  assert(!token_is_quoted_id(tk));
  token_free(tk);

  tk = token_new(0, 0, str_new("'\"anv"));
  assert(!token_is_id(tk));
  assert(!token_is_quoted_id(tk));
  token_free(tk);

  puts("    Finised");
}
