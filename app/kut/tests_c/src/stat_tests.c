// Copyright 06-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "stat_tests.h"
#include "kut/DEFS.h"
#include "stat.h"
#include "symix.h"
#include "kut/arr.h"
#include "kut/map.h"

static StatCode *mk_sc (Stat *st) {
  return stat_code_new(0, 1, st);
}

void stat_tests () {
  puts(">>> stat:");

  Stat *empty = stat_empty();
  assert (stat_is_empty(empty));
  TEST(stat_to_str(empty), ";");
  TEST(stat_type_to_str(empty), "empty");

  StatCode *stcode = stat_code_new(1, 23, empty);
  TESTI(stat_code_file_ix(stcode), 1);
  TESTI(stat_code_line(stcode), 23);
  assert(stat_is_empty(stat_code_stat(stcode)));

  Stat *asg = stat_assign(exp_sym(symix_add("a")), exp_int(3));
  assert(stat_is_assign(asg));
  // <<Exp>, <Exp>>
  Tp *asg_v = stat_get_assign(asg);
  TEST(symix_get(exp_get_sym(tp_e1(asg_v))), "a");
  TESTI(exp_get_int(tp_e2(asg_v)), 3);
  TEST(stat_to_str(asg), "a = 3;");
  TEST(stat_type_to_str(asg), "assign");

  Stat *add_as = stat_add_as(exp_sym(symix_add("a")), exp_int(3));
  assert(stat_is_add_as(add_as));
  // <<Exp>, <Exp>>
  Tp *add_as_v = stat_get_add_as(add_as);
  TEST(symix_get(exp_get_sym(tp_e1(add_as_v))), "a");
  TESTI(exp_get_int(tp_e2(add_as_v)), 3);
  TEST(stat_to_str(add_as), "a += 3;");
  TEST(stat_type_to_str(add_as), "add_as");

  Stat *sub_as = stat_sub_as(exp_sym(symix_add("a")), exp_int(3));
  assert(stat_is_sub_as(sub_as));
  // <<Exp>, <Exp>>
  Tp *sub_as_v = stat_get_sub_as(sub_as);
  TEST(symix_get(exp_get_sym(tp_e1(sub_as_v))), "a");
  TESTI(exp_get_int(tp_e2(sub_as_v)), 3);
  TEST(stat_to_str(sub_as), "a -= 3;");
  TEST(stat_type_to_str(sub_as), "sub_as");

  Stat *mul_as = stat_mul_as(exp_sym(symix_add("a")), exp_int(3));
  assert(stat_is_mul_as(mul_as));
  // <<Exp>, <Exp>>
  Tp *mul_as_v = stat_get_mul_as(mul_as);
  TEST(symix_get(exp_get_sym(tp_e1(mul_as_v))), "a");
  TESTI(exp_get_int(tp_e2(mul_as_v)), 3);
  TEST(stat_to_str(mul_as), "a *= 3;");
  TEST(stat_type_to_str(mul_as), "mul_as");

  Stat *div_as = stat_div_as(exp_sym(symix_add("a")), exp_int(3));
  assert(stat_is_div_as(div_as));
  // <<Exp>, <Exp>>
  Tp *div_as_v = stat_get_div_as(div_as);
  TEST(symix_get(exp_get_sym(tp_e1(div_as_v))), "a");
  TESTI(exp_get_int(tp_e2(div_as_v)), 3);
  TEST(stat_to_str(div_as), "a /= 3;");
  TEST(stat_type_to_str(div_as), "div_as");

  Stat *mod_as = stat_mod_as(exp_sym(symix_add("a")), exp_int(3));
  assert(stat_is_mod_as(mod_as));
  // <<Exp>, <Exp>>
  Tp *mod_as_v = stat_get_mod_as(mod_as);
  TEST(symix_get(exp_get_sym(tp_e1(mod_as_v))), "a");
  TESTI(exp_get_int(tp_e2(mod_as_v)), 3);
  TEST(stat_to_str(mod_as), "a %= 3;");
  TEST(stat_type_to_str(mod_as), "mod_as");

  Stat *and_as = stat_and_as(exp_sym(symix_add("a")), exp_int(3));
  assert(stat_is_and_as(and_as));
  // <<Exp>, <Exp>>
  Tp *and_as_v = stat_get_and_as(and_as);
  TEST(symix_get(exp_get_sym(tp_e1(and_as_v))), "a");
  TESTI(exp_get_int(tp_e2(and_as_v)), 3);
  TEST(stat_to_str(and_as), "a &= 3;");
  TEST(stat_type_to_str(and_as), "and_as");

  Stat *or_as = stat_or_as(exp_sym(symix_add("a")), exp_int(3));
  assert(stat_is_or_as(or_as));
  // <<Exp>, <Exp>>
  Tp *or_as_v = stat_get_or_as(or_as);
  TEST(symix_get(exp_get_sym(tp_e1(or_as_v))), "a");
  TESTI(exp_get_int(tp_e2(or_as_v)), 3);
  TEST(stat_to_str(or_as), "a |= 3;");
  TEST(stat_type_to_str(or_as), "or_as");

  Stat *func = stat_func(exp_pr(exp_sym(symix_add("a")), arr_new()));
  assert(stat_is_func(func));
  TEST(exp_to_str(stat_get_func(func)), "a()");
  TEST(stat_to_str(func), "a();");
  TEST(stat_type_to_str(func), "function");

  Stat *block = stat_block(arr_new());
  assert(stat_is_block(block));
  assert(arr_empty(stat_get_block(block)));
  TEST(stat_to_str(block), "{}");
  TEST(stat_type_to_str(block), "block");

  Stat *block2 = stat_block(arr_new_from(stat_code_new(0, 1, stat_empty()), NULL));
  assert(stat_is_block(block2));
  assert(stat_is_empty(stat_code_stat(arr_get(stat_get_block(block2), 0))));
  TEST(stat_to_str(block2), "{;}");
  TEST(stat_type_to_str(block2), "block");

  Stat *block_close = stat_block_close();
  assert (stat_is_block_close(block_close));
  TEST(stat_to_str(block_close), "}");
  TEST(stat_type_to_str(block_close), "block_close");

  Stat *breakv = stat_break();
  assert (stat_is_break(breakv));
  TEST(stat_to_str(breakv), "break;");
  TEST(stat_type_to_str(breakv), "break");

  Stat *continuev = stat_continue();
  assert (stat_is_continue(continuev));
  TEST(stat_to_str(continuev), "continue;");
  TEST(stat_type_to_str(continuev), "continue");

  Stat *trace = stat_trace(FALSE, exp_sym(symix_add("a")));
  assert(stat_is_trace(trace));
  // <int, Exp>
  Tp *trace_tp = stat_get_trace(trace);
  TEST(exp_to_str(tp_e2(trace_tp)), "a");
  TEST(stat_to_str(trace), "trace a;");
  TEST(stat_type_to_str(trace), "trace");

  Stat *returnv = stat_return(exp_sym(symix_add("a")));
  assert(stat_is_return(returnv));
  TEST(exp_to_str(stat_get_return(returnv)), "a");
  TEST(stat_to_str(returnv), "return a;");
  TEST(stat_type_to_str(returnv), "return");

  Stat *returnv2 = stat_return(exp_empty());
  assert(stat_is_return(returnv2));
  TEST(stat_to_str(returnv2), "return;");
  TEST(stat_type_to_str(returnv2), "return");

  Stat *try = stat_try(mk_sc(stat_empty()), symix_add("e"), mk_sc(stat_empty()), opt_none());
  assert(stat_is_try(try));
  // [<Stat>, <int>, <Stat>, <Opt<Stat>>]
  Arr *try_v = stat_get_try(try);
  assert(stat_is_empty(stat_code_stat(arr_get(try_v, 0))));
  TEST(symix_get(*((int *)arr_get(try_v, 1))), "e");
  assert(stat_is_empty(stat_code_stat(arr_get(try_v, 2))));
  assert(!opt_get(arr_get(try_v, 3)));
  TEST(stat_to_str(try), "try ;\ncatch(e) ;");
  TEST(stat_type_to_str(try), "try");

  Stat *try2 = stat_try(
    mk_sc(stat_empty()), symix_add("e"), mk_sc(stat_empty()),
    opt_some(mk_sc(stat_empty()))
  );
  assert(stat_is_try(try2));
  // [<Stat>, <int>, <Stat>, <Opt<Stat>>]
  Arr *try2_v = stat_get_try(try2);
  assert(stat_is_empty(stat_code_stat(arr_get(try2_v, 0))));
  TEST(symix_get(*((int *)arr_get(try2_v, 1))), "e");
  assert(stat_is_empty(stat_code_stat(arr_get(try2_v, 2))));
  assert(stat_is_empty(stat_code_stat(opt_get(arr_get(try2_v, 3)))));
  TEST(stat_to_str(try2), "try ;\ncatch(e) ;\nfinally ;");
  TEST(stat_type_to_str(try2), "try");

  Stat *whilev = stat_while(exp_empty(), mk_sc(stat_empty()));
  assert(stat_is_while(whilev));
  // [<Exp>, <Stat>]
  Arr *while_v = stat_get_while(whilev);
  assert(exp_is_empty(arr_get(while_v, 0)));
  assert(stat_is_empty(stat_code_stat(arr_get(while_v, 1))));
  TEST(stat_to_str(whilev), "while () ;");
  TEST(stat_type_to_str(whilev), "while");

  Stat *whilev2 = stat_while(exp_bool(TRUE), mk_sc(stat_empty()));
  assert(stat_is_while(whilev2));
  // [<Exp>, <Stat>]
  Arr *while2_v = stat_get_while(whilev2);
  TESTI(exp_get_bool(arr_get(while2_v, 0)), TRUE);
  assert(stat_is_empty(stat_code_stat(arr_get(while2_v, 1))));
  TEST(stat_to_str(whilev2), "while (true) ;");
  TEST(stat_type_to_str(whilev2), "while");

  Stat *ifv = stat_if(exp_bool(FALSE), mk_sc(stat_empty()), opt_none());
  assert(stat_is_if(ifv));
  // [<Exp>, <Stat>, <Opt<Stat>>]
  Arr *if_v = stat_get_if(ifv);
  TESTI(exp_get_bool(arr_get(if_v, 0)), FALSE);
  assert(stat_is_empty(stat_code_stat(arr_get(if_v, 1))));
  assert(!opt_get(arr_get(if_v, 2)));
  TEST(stat_to_str(ifv), "if (false) ;");
  TEST(stat_type_to_str(ifv), "if");

  Stat *ifv2 = stat_if(
    exp_bool(FALSE), mk_sc(stat_empty()), opt_some(mk_sc(stat_empty()))
  );
  assert(stat_is_if(ifv2));
  // [<Exp>, <Stat>, <Opt<Stat>>]
  Arr *if2_v = stat_get_if(ifv2);
  TESTI(exp_get_bool(arr_get(if2_v, 0)), FALSE);
  assert(stat_is_empty(stat_code_stat(arr_get(if2_v, 1))));
  assert(stat_is_empty(stat_code_stat(opt_get(arr_get(if2_v, 2)))));
  TEST(stat_to_str(ifv2), "if (false) ;\nelse ;");
  TEST(stat_type_to_str(ifv2), "if");

  Stat *forv = stat_for(symix_add("e"), exp_array(arr_new()), mk_sc(stat_empty()));
  assert(stat_is_for(forv));
  // [<int>, <Exp>, <Stat>]
  Arr *for_v = stat_get_for(forv);
  TEST(symix_get(*((int *)arr_get(for_v, 0))), "e");
  assert(arr_empty(exp_get_array(arr_get(for_v, 1))));
  assert(stat_is_empty(stat_code_stat(arr_get(for_v, 2))));
  TEST(stat_to_str(forv), "for (e = []) ;");
  TEST(stat_type_to_str(forv), "for");

  Stat *for_r = stat_for_r(symix_add("e"), exp_int(0), exp_int(10), mk_sc(stat_empty()));
  assert(stat_is_for_r(for_r));
  // [<char>, <Exp>, <Exp>, <Stat>]
  Arr *for_r_v = stat_get_for_r(for_r);
  TEST(symix_get(*((int *)arr_get(for_r_v, 0))), "e");
  TESTI(exp_get_int(arr_get(for_r_v, 1)), 0);
  TESTI(exp_get_int(arr_get(for_r_v, 2)), 10);
  assert(stat_is_empty(stat_code_stat(arr_get(for_r_v, 3))));
  TEST(stat_to_str(for_r), "for (e = 0 : 10) ;");
  TEST(stat_type_to_str(for_r), "for_r");

  Stat *for_rs = stat_for_rs(
    symix_add("e"), exp_int(0), exp_int(10), exp_int(2), mk_sc(stat_empty())
  );
  assert(stat_is_for_rs(for_rs));
  // [<int>, <Exp>, <Exp>, <Exp>, <Stat>]
  Arr *for_rs_v = stat_get_for_rs(for_rs);
  TEST(symix_get(*((int *)arr_get(for_rs_v, 0))), "e");
  TESTI(exp_get_int(arr_get(for_rs_v, 1)), 0);
  TESTI(exp_get_int(arr_get(for_rs_v, 2)), 10);
  TESTI(exp_get_int(arr_get(for_rs_v, 3)), 2);
  assert(stat_is_empty(stat_code_stat(arr_get(for_rs_v, 4))));
  TEST(stat_to_str(for_rs), "for (e = 0 : 10 : 2) ;");
  TEST(stat_type_to_str(for_rs), "for_rs");

  Stat *switchv = stat_switch(
    exp_int(2),
    arr_new_from(
      tp_new(exp_int(1), stat_empty()),
      tp_new(exp_sym(symix_add("default")), stat_empty()),
      NULL
    )
  );
  assert(stat_is_switch(switchv));
  // [<Exp>, Arr<Tp<Exp, Stat>>]
  Arr *switch_v = stat_get_switch(switchv);
  TESTI(exp_get_int(arr_get(switch_v, 0)), 2);
  Arr *swarr = arr_get(switch_v, 1);
  Tp *tp1 = arr_get(swarr, 0);
  TESTI(exp_get_int(tp_e1(tp1)), 1);
  assert(stat_is_empty(tp_e2(tp1)));
  Tp *tp2 = arr_get(swarr, 1);
  TEST(symix_get(exp_get_sym(tp_e1(tp2))), "default");
  assert(stat_is_empty(tp_e2(tp2)));
  TEST(stat_to_str(switchv), "switch (2) {\n1 : ;\ndefault : ;\n}");
  TEST(stat_type_to_str(switchv), "switch");

  Stat *importv = stat_import(0, symix_add("abc"));
  assert(stat_is_import(importv));
  // [<int>, <int>]
  Arr *import_v = stat_get_import(importv);
  TESTI(*((int*)arr_get(import_v, 0)), 0);
  TEST(symix_get(*((int *)arr_get(import_v, 1))), "abc");
  TEST(stat_to_str(importv), "import \"/home/deme/dm/dmC/app/kut/tests_c/data/main\" : abc;");
  TEST(stat_type_to_str(importv), "import");

  Stat *importv2 = stat_import(0, symix_add(""));
  assert(stat_is_import(importv2));
  // [<int>, <int>]
  Arr *import2_v = stat_get_import(importv2);
  TESTI(*((int*)arr_get(import2_v, 0)), 0);
  TEST(symix_get(*((int *)arr_get(import2_v, 1))), "");
  TEST(stat_to_str(importv2), "import \"/home/deme/dm/dmC/app/kut/tests_c/data/main\";");
  TEST(stat_type_to_str(importv2), "import");

  puts("  ... Finished");
}
