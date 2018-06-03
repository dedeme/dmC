// Copyright 2-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rtype.h"
#include "ast/Atype.h"
#include "lexer/rtype.h"

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

void tests_rtype() {
  puts("Reader: type");

  Type *tp;
  Atype *ps;
  Atype *ps2;
  Atype *ps3;

  Tx *tx = mk_tx(":(Int:) more");
  tx = rtype(&tp, tx);
  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(":* more");
  tx = rtype(&tp, tx);
  assert(!strcmp("*", type_to_str(tp)));
  assert(type_type(tp) == ANY);
  assert(*tx_start(tx) == 'm');
  assert(tx_nline(tx) == 1);
  assert(tx_nchar(tx) == 3);
  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(":Int   ");
  tx = rtype(&tp, tx);
  assert(!strcmp("Int", type_to_str(tp)));
  assert(type_type(tp) == DATA);
  assert(!strcmp(type_id(tp), "Int"));
  assert(!*tx_start(tx));
  assert(tx_nline(tx) == 1);
  assert(tx_nchar(tx) == 7);
  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(":(Int,*:)   \n m");
  tx = rtype(&tp, tx);
  assert(!strcmp("(Int,*:)", type_to_str(tp)));
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*tx_start(tx) == 'm');
  assert(tx_nline(tx) == 2);
  assert(tx_nchar(tx) == 1);
  ps = type_params(tp);
  assert(arr_size(ps) == 3);
  assert(type_type(arr_get(ps, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "Int"));
  assert(type_type(arr_get(ps, 1)) == ANY);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));
  assert(type_type(arr_get(ps, 2)) == VOID);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));
  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(": (  Int  , \n *  : )    m");
  tx = rtype(&tp, tx);
  assert(!strcmp("(Int,*:)", type_to_str(tp)));
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*tx_start(tx) == 'm');
  assert(tx_nline(tx) == 2);
  ps = type_params(tp);
  assert(arr_size(ps) == 3);
  assert(type_type(arr_get(ps, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "Int"));
  assert(type_type(arr_get(ps, 1)) == ANY);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));
  assert(type_type(arr_get(ps, 2)) == VOID);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));
  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(":(Int,*:Str)   \n m");
  tx = rtype(&tp, tx);
  assert(!strcmp("(Int,*:Str)", type_to_str(tp)));
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*tx_start(tx) == 'm');
  assert(tx_nline(tx) == 2);
  assert(tx_nchar(tx) == 1);
  ps = type_params(tp);
  assert(arr_size(ps) == 3);
  assert(type_type(arr_get(ps, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "Int"));
  assert(type_type(arr_get(ps, 1)) == ANY);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));
  assert(type_type(arr_get(ps, 2)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 2)), "Str"));
  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(":    (  Int  , \n *  : *)    m");
  tx = rtype(&tp, tx);
  assert(!strcmp("(Int,*:*)", type_to_str(tp)));
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*tx_start(tx) == 'm');
  assert(tx_nline(tx) == 2);
  ps = type_params(tp);
  assert(arr_size(ps) == 3);
  assert(type_type(arr_get(ps, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "Int"));
  assert(type_type(arr_get(ps, 1)) == ANY);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));
  assert(type_type(arr_get(ps, 2)) == ANY);
  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(":(MyClass<Int>,*:Other<*,Int>)   \n m");
  tx = rtype(&tp, tx);
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*tx_start(tx) == 'm');
  assert(tx_nline(tx) == 2);
  assert(tx_nchar(tx) == 1);
  ps = type_params(tp);
  assert(arr_size(ps) == 3);

  assert(type_type(arr_get(ps, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "MyClass"));
  ps2 = type_params(arr_get(ps, 0));
  assert(type_type(arr_get(ps2, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps2, 0)), "Int"));

  assert(type_type(arr_get(ps, 1)) == ANY);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));

  assert(type_type(arr_get(ps, 2)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 2)), "Other"));
  ps2 = type_params(arr_get(ps, 2));
  assert(type_type(arr_get(ps2, 0)) == ANY);
  assert(!strcmp(type_id(arr_get(ps2, 0)), ""));
  assert(type_type(arr_get(ps2, 1)) == DATA);
  assert(!strcmp(type_id(arr_get(ps2, 1)), "Int"));

  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(":(MyClass<*>,*:Other<*,*>)   \n m");
  tx = rtype(&tp, tx);
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*tx_start(tx) == 'm');
  assert(tx_nline(tx) == 2);
  assert(tx_nchar(tx) == 1);
  ps = type_params(tp);
  assert(arr_size(ps) == 3);

  assert(type_type(arr_get(ps, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "MyClass"));
  ps2 = type_params(arr_get(ps, 0));
  assert(type_type(arr_get(ps2, 0)) == ANY);
  assert(!strcmp(type_id(arr_get(ps2, 0)), ""));

  assert(type_type(arr_get(ps, 1)) == ANY);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));

  assert(type_type(arr_get(ps, 2)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 2)), "Other"));
  ps2 = type_params(arr_get(ps, 2));
  assert(type_type(arr_get(ps2, 0)) == ANY);
  assert(!strcmp(type_id(arr_get(ps2, 0)), ""));
  assert(type_type(arr_get(ps2, 1)) == ANY);
  assert(!strcmp(type_id(arr_get(ps2, 1)), ""));

  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(":{  Int }  m  ");
  tx = rtype(&tp, tx);
  assert(type_type(tp) == DATA);
  assert(!strcmp(type_id(tp), "Map"));
  assert(*tx_start(tx) == 'm');
  assert(tx_nline(tx) == 1);
  ps = type_params(tp);
  assert(arr_size(ps) == 1);
  assert(type_type(arr_get(ps, 0)) ==  DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "Int"));
  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(":[*]m");
  tx = rtype(&tp, tx);
  assert(type_type(tp) == DATA);
  assert(!strcmp(type_id(tp), "Arr"));
  assert(*tx_start(tx) == 'm');
  assert(tx_nline(tx) == 1);
  ps = type_params(tp);
  assert(arr_size(ps) == 1);
  assert(type_type(arr_get(ps, 0)) ==  ANY);
  assert(!strcmp(type_id(arr_get(ps, 0)), ""));
  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(":(MyClass<[Int]>,*:Other<{*},[Int]>)   \n m");
  tx = rtype(&tp, tx);
  assert(!strcmp("(MyClass<[Int]>,*:Other<{*},[Int]>)", type_to_str(tp)));
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*tx_start(tx) == 'm');
  assert(tx_nline(tx) == 2);
  assert(tx_nchar(tx) == 1);
  ps = type_params(tp);
  assert(arr_size(ps) == 3);

  assert(type_type(arr_get(ps, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "MyClass"));
  ps2 = type_params(arr_get(ps, 0));
  assert(arr_size(ps2) == 1);
  assert(type_type(arr_get(ps2, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps2, 0)), "Arr"));
  ps3 = type_params(arr_get(ps2, 0));
  assert(arr_size(ps2) == 1);
  assert(type_type(arr_get(ps3, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps3, 0)), "Int"));

  assert(type_type(arr_get(ps, 1)) == ANY);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));

  assert(type_type(arr_get(ps, 2)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 2)), "Other"));
  ps2 = type_params(arr_get(ps, 2));
  assert(arr_size(ps2) == 2);
  assert(type_type(arr_get(ps2, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps2, 0)), "Map"));
  ps3 = type_params(arr_get(ps2, 0));
  assert(arr_size(ps3) == 1);
  assert(type_type(arr_get(ps3, 0)) == ANY);
  assert(!strcmp(type_id(arr_get(ps3, 0)), ""));

  assert(type_type(arr_get(ps2, 1)) == DATA);
  assert(!strcmp(type_id(arr_get(ps2, 1)), "Arr"));
  ps3 = type_params(arr_get(ps2, 1));
  assert(arr_size(ps3) == 1);
  assert(type_type(arr_get(ps3, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps3, 0)), "Int"));

  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(":Dict < K , V >   \n m");
  tx = rtype(&tp, tx);
  assert(!strcmp("Dict<K,V>", type_to_str(tp)));
  assert(type_eq(tp, type_restore(type_serialize(tp))));

  tx = mk_tx(":(Dict <K, V >,   * : Any  )   \n m");
  tx = rtype(&tp, tx);
  assert(!strcmp("(Dict<K,V>,*:Any)", type_to_str(tp)));
  assert(type_eq(tp, type_restore(type_serialize(tp))));

  puts("    Finished");
}
