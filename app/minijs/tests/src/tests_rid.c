// Copyright 7-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rid.h"
#include "ast/Value.h"
#include "lexer/rvalue.h"

static char *rm_last(Achar *achar) {
  int ix = arr_size(achar) - 1;
  char *r = arr_get(achar, ix);
  arr_remove(achar, ix);
  return r;
}

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

void tests_rid() {
  puts("Reader: rid");
  Tx *tx;
  char *id;
  Value *v;
  Achar *data;
  Achar *generics;

  tx = mk_tx("a");
  rvalue(&v, tx);
  assert(value_vtype(v) == VID);
  data = value_data(v);
  id = rm_last(data);
  generics = data;
  assert(!strcmp(id, "a"));
  assert(arr_size(generics) == 0);

  tx = mk_tx("a<Str>");
  rvalue(&v, tx);
  assert(value_vtype(v) == VID);
  data = value_data(v);
  id = rm_last(data);
  generics = data;
  assert(!strcmp(id, "a"));
  assert(arr_size(generics) == 1);
  assert(!strcmp(achar_get(generics, 0), "Str"));

  tx = mk_tx("a<Int, A>");
  rvalue(&v, tx);
  assert(value_vtype(v) == VID);
  data = value_data(v);
  id = rm_last(data);
  generics = data;
  assert(!strcmp(id, "a"));
  assert(arr_size(generics) == 2);
  assert(!strcmp(achar_get(generics, 0), "Int"));
  assert(!strcmp(achar_get(generics, 1), "A"));

//  tx = mk_tx("while");
//  rvalue(&v, tx);

//  tx = mk_tx("a<>");
//  rvalue(&v, tx);

//  tx = mk_tx("a<while>");
//  rvalue(&v, tx);

  puts("    Finished");
}
