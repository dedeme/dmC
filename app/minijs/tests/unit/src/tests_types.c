// Copyright 24-Mar-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_types.h"
#include "DEFS.h"
#include "Cpath.h"
#include "lexer/Txpos.h"
#include "lexer/block.h"
#include "types/Type.h"

static Txpos *mk_tx(char *t) {
  Cpath *path = cpath_new("Test1");
  return txpos_new(path, t, t, t + strlen(t), 1, 0);
}

void tests_types() {
  printf("Types test\n");

  Type *tp;
  Arr/*Type*/ *ps;
  Arr/*Type*/ *ps2;
  Arr/*Type*/ *ps3;

  Txpos *tx = mk_tx("* more");
  tx = block_type(&tp, tx);
  assert(type_type(tp) == ANY);
  assert(*txpos_start(tx) == 'm');
  assert(txpos_nline(tx) == 1);
  assert(txpos_nchar(tx) == 2);

  tx = mk_tx("Int   ");
  tx = block_type(&tp, tx);
  assert(type_type(tp) == DATA);
  assert(!strcmp(type_id(tp), "Int"));
  assert(!*txpos_start(tx));
  assert(txpos_nline(tx) == 1);
  assert(txpos_nchar(tx) == 6);

  tx = mk_tx("(Int,*:)   \n m");
  tx = block_type(&tp, tx);
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*txpos_start(tx) == 'm');
  assert(txpos_nline(tx) == 2);
  assert(txpos_nchar(tx) == 1);
  ps = type_params(tp);
  assert(arr_size(ps) == 3);
  assert(type_type(arr_get(ps, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "Int"));
  assert(type_type(arr_get(ps, 1)) == ANY);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));
  assert(!arr_nget(ps, 2));

  tx = mk_tx("(  Int  , \n *  : )    m");
  tx = block_type(&tp, tx);
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*txpos_start(tx) == 'm');
  assert(txpos_nline(tx) == 2);
  ps = type_params(tp);
  assert(arr_size(ps) == 3);
  assert(type_type(arr_get(ps, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "Int"));
  assert(type_type(arr_get(ps, 1)) == ANY);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));
  assert(!arr_nget(ps, 2));

  tx = mk_tx("(Int,*:Str)   \n m");
  tx = block_type(&tp, tx);
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*txpos_start(tx) == 'm');
  assert(txpos_nline(tx) == 2);
  assert(txpos_nchar(tx) == 1);
  ps = type_params(tp);
  assert(arr_size(ps) == 3);
  assert(type_type(arr_get(ps, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "Int"));
  assert(type_type(arr_get(ps, 1)) == ANY);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));
  assert(type_type(arr_get(ps, 2)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 2)), "Str"));

  tx = mk_tx("(  Int  , \n *  : *)    m");
  tx = block_type(&tp, tx);
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*txpos_start(tx) == 'm');
  assert(txpos_nline(tx) == 2);
  ps = type_params(tp);
  assert(arr_size(ps) == 3);
  assert(type_type(arr_get(ps, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "Int"));
  assert(type_type(arr_get(ps, 1)) == ANY);
  assert(!strcmp(type_id(arr_get(ps, 1)), ""));
  assert(type_type(arr_get(ps, 2)) == ANY);

  tx = mk_tx("(MyClass<Int>,*:Other<*,Int>)   \n m");
  tx = block_type(&tp, tx);
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*txpos_start(tx) == 'm');
  assert(txpos_nline(tx) == 2);
  assert(txpos_nchar(tx) == 1);
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

  tx = mk_tx("(MyClass<*>,*:Other<*,*>)   \n m");
  tx = block_type(&tp, tx);
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*txpos_start(tx) == 'm');
  assert(txpos_nline(tx) == 2);
  assert(txpos_nchar(tx) == 1);
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

  tx = mk_tx("{  Int }  m  ");
  tx = block_type(&tp, tx);
  assert(type_type(tp) == MAP);
  assert(!strcmp(type_id(tp), ""));
  assert(*txpos_start(tx) == 'm');
  assert(txpos_nline(tx) == 1);
  ps = type_params(tp);
  assert(arr_size(ps) == 1);
  assert(type_type(arr_get(ps, 0)) ==  DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "Int"));

  tx = mk_tx("[*]m");
  tx = block_type(&tp, tx);
  assert(type_type(tp) == ARR);
  assert(!strcmp(type_id(tp), ""));
  assert(*txpos_start(tx) == 'm');
  assert(txpos_nline(tx) == 1);
  ps = type_params(tp);
  assert(arr_size(ps) == 1);
  assert(type_type(arr_get(ps, 0)) ==  ANY);
  assert(!strcmp(type_id(arr_get(ps, 0)), ""));

  tx = mk_tx("(MyClass<[Int]>,*:Other<{*},[Int]>)   \n m");
  tx = block_type(&tp, tx);
  assert(type_type(tp) == FN);
  assert(!strcmp(type_id(tp), ""));
  assert(*txpos_start(tx) == 'm');
  assert(txpos_nline(tx) == 2);
  assert(txpos_nchar(tx) == 1);
  ps = type_params(tp);
  assert(arr_size(ps) == 3);

  assert(type_type(arr_get(ps, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps, 0)), "MyClass"));
  ps2 = type_params(arr_get(ps, 0));
  assert(arr_size(ps2) == 1);
  assert(type_type(arr_get(ps2, 0)) == ARR);
  assert(!strcmp(type_id(arr_get(ps2, 0)), ""));
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
  assert(type_type(arr_get(ps2, 0)) == MAP);
  assert(!strcmp(type_id(arr_get(ps2, 0)), ""));
  ps3 = type_params(arr_get(ps2, 0));
  assert(arr_size(ps3) == 1);
  assert(type_type(arr_get(ps3, 0)) == ANY);
  assert(!strcmp(type_id(arr_get(ps3, 0)), ""));

  assert(type_type(arr_get(ps2, 1)) == ARR);
  assert(!strcmp(type_id(arr_get(ps2, 1)), ""));
  ps3 = type_params(arr_get(ps2, 1));
  assert(arr_size(ps3) == 1);
  assert(type_type(arr_get(ps3, 0)) == DATA);
  assert(!strcmp(type_id(arr_get(ps3, 0)), "Int"));

  printf( "    Finished\n");
}
