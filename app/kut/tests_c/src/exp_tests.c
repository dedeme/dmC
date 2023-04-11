// Copyright 01-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "exp_tests.h"
#include "kut/DEFS.h"
#include "exp.h"
#include "kut/arr.h"
#include "kut/map.h"
#include "runner/stack.h"

void exp_tests () {
  puts(">>> exp:");

  Exp *empty = exp_empty();
  assert(exp_is_empty(empty));

  Exp *br = exp_break(stack_new());
  assert(exp_is_break(br));
  TESTI(arr_size(stack_to_arr(exp_get_break(br))), 0);
  TEST(exp_to_str(br), "break");
  TEST(exp_to_js(br), "break");
  TEST(exp_type_to_str(br), "break");

  Exp *ct = exp_continue(stack_new());
  assert(exp_is_continue(ct));
  TESTI(arr_size(stack_to_arr(exp_get_continue(ct))), 0);
  TEST(exp_to_str(ct), "continue");
  TEST(exp_to_js(ct), "continue");
  TEST(exp_type_to_str(ct), "continue");

  Exp *b = exp_bool(TRUE);
  assert(exp_is_bool(b));
  TESTI(exp_get_bool(b), TRUE);
  TEST(exp_to_str(b), "true");
  TEST(exp_to_js(b), "true");
  TEST(exp_type_to_str(b), "bool");

  Exp *i = exp_int(23);
  assert(exp_is_int(i));
  TESTI(exp_get_int(i), 23);
  TEST(exp_to_str(i), "23");
  TEST(exp_to_js(i), "23");
  TEST(exp_type_to_str(i), "int");

  Exp *f = exp_float(-12.450);
  assert(exp_is_float(f));
  TESTF(exp_get_float(f), -12.45);
  TEST(exp_to_str(f), "-12.45");
  TEST(exp_to_js(f), "-12.45");
  TEST(exp_type_to_str(f), "float");

  Exp *s = exp_string("ab\\c");
  assert(exp_is_string(s));
  TEST(exp_get_string(s), "ab\\c");
  TEST(exp_to_str(s), "ab\\c");
  TEST(exp_to_js(s), "\"ab\\\\c\"");
  TEST(exp_type_to_str(s), "str");

  Exp *o = exp_object("<nil>", NULL);
  assert(exp_is_object("<nil>", o));
  TEST(exp_to_str(o), "object<nil>:0");
  TEST(exp_to_js(o), "object<nil>:0");
  TEST(exp_type_to_str(o), "object<nil>");

  Arr *ar = arr_new_from("a", "b", "c", NULL);
  Exp *a = exp_array(arr_map(ar, (FMAP)exp_string));
  assert(exp_is_array(a));
  TEST(arr_join(arr_map(exp_get_array(a), (FMAP)exp_get_string), "-"), "a-b-c");
  TEST(exp_to_str(a), "[\"a\", \"b\", \"c\"]");
  TEST(exp_to_js(a), "[\"a\", \"b\", \"c\"]");
  TEST(exp_type_to_str(a), "arr");

  Map *mp = map_new();
  map_put(mp, "one", exp_string("a"));
  map_put(mp, "two", exp_string("b"));
  map_put(mp, "three", exp_string("c"));
  Exp *m = exp_map(mp);
  assert(exp_is_map(m));
  Map *mp2 = exp_get_map(m);
  TEST(exp_get_string(opt_get(map_get(mp2, "one"))), "a");
  TEST(exp_get_string(opt_get(map_get(mp2, "two"))), "b");
  TEST(exp_get_string(opt_get(map_get(mp2, "three"))), "c");
  TEST(exp_to_str(m), "{\"one\": \"a\", \"two\": \"b\", \"three\": \"c\"}");
  TEST(exp_type_to_str(m), "dic");

  Exp *sy = exp_sym("abc");
  assert(exp_is_sym(sy));
  TEST(exp_get_sym(sy), "abc");
  TEST(exp_to_str(sy), "abc");
  TEST(exp_to_js(sy), "abc");
  TEST(exp_type_to_str(sy), "symbol");

  Exp *ran = exp_range(exp_int(11), exp_int(22), exp_empty());
  assert(exp_is_range(ran));
  Tp3 *ran_v = exp_get_range(ran);
  TESTI(exp_get_int(tp3_e1(ran_v)), 11);
  TESTI(exp_get_int(tp3_e2(ran_v)), 22);
  TEST(exp_to_str(ran), "[11:22]");
  TEST(exp_to_js(ran), "[11:22]");
  TEST(exp_type_to_str(ran), "range");

  Exp *pt = exp_pt(exp_sym("class"), exp_sym("func"));
  assert(exp_is_pt(pt));
  Tp *pt_v = exp_get_pt(pt);
  TEST(exp_get_sym(tp_e1(pt_v)), "class");
  TEST(exp_get_sym(tp_e2(pt_v)), "func");
  TEST(exp_to_str(pt), "class.func");
  TEST(exp_to_js(pt), "class.func");
  TEST(exp_type_to_str(pt), "point");

  Exp *sq = exp_sq(exp_sym("map"), exp_string("key"));
  assert(exp_is_sq(sq));
  Tp *sq_v = exp_get_sq(sq);
  TEST(exp_get_sym(tp_e1(sq_v)), "map");
  TEST(exp_get_string(tp_e2(sq_v)), "key");
  TEST(exp_to_str(sq), "map[\"key\"]");
  TEST(exp_to_js(sq), "map[\"key\"]");
  TEST(exp_type_to_str(sq), "square");

  Arr *args0 = arr_new();
  Exp *pr = exp_pr(exp_sym("func"), args0);
  assert(exp_is_pr(pr));
  Tp *pr_v = exp_get_pr(pr);
  TEST(exp_get_sym(tp_e1(pr_v)), "func");
  Arr *args02 = tp_e2(pr_v);
  TESTI(arr_size(args02), 0);
  TEST(exp_to_str(pr), "func()");
  TEST(exp_to_js(pr), "func()");
  TEST(exp_type_to_str(pr), "parenthesis");

  Arr *args1 = arr_new_from(exp_string("a"), NULL);
  pr = exp_pr(exp_sym("func"), args1);
  assert(exp_is_pr(pr));
  pr_v = exp_get_pr(pr);
  TEST(exp_get_sym(tp_e1(pr_v)), "func");
  Arr *args12 = tp_e2(pr_v);
  TESTI(arr_size(args12), 1);
  TEST(exp_get_string(arr_get(args12, 0)), "a")
  TEST(exp_to_str(pr), "func(\"a\")");
  TEST(exp_to_js(pr), "func(\"a\")");
  TEST(exp_type_to_str(pr), "parenthesis");

  Arr *args3 = arr_new_from(exp_string("a"), exp_int(1), exp_string("c"), NULL);
  pr = exp_pr(exp_sym("func"), args3);
  assert(exp_is_pr(pr));
  pr_v = exp_get_pr(pr);
  TEST(exp_get_sym(tp_e1(pr_v)), "func");
  Arr *args32 = tp_e2(pr_v);
  TESTI(arr_size(args32), 3);
  TEST(exp_get_string(arr_get(args32, 0)), "a")
  TESTI(exp_get_int(arr_get(args32, 1)), 1)
  TEST(exp_get_string(arr_get(args32, 2)), "c")
  TEST(exp_to_str(pr), "func(\"a\",1,\"c\")");
  TEST(exp_to_js(pr), "func(\"a\",1,\"c\")");
  TEST(exp_type_to_str(pr), "parenthesis");

  Tp *tp1 = tp_new(exp_int(1), exp_string("a"));
  Tp *tp2 = tp_new(exp_sym("default"), exp_string("b"));
  Arr *swa = arr_new_from(tp1, tp2, NULL);
  Exp *sw = exp_switch(exp_sym("cond"), swa);
  assert(exp_is_switch(sw));
  Tp *sw_v = exp_get_switch(sw);
  TEST(exp_get_sym(tp_e1(sw_v)), "cond");
  Arr *sw_cl = tp_e2(sw_v);
  TESTI(exp_get_int(tp_e1(arr_get(sw_cl, 0))), 1);
  TEST(exp_get_string(tp_e2(arr_get(sw_cl, 0))), "a");
  TESTI(exp_get_sym(tp_e1(arr_get(sw_cl, 1))), "default");
  TEST(exp_get_string(tp_e2(arr_get(sw_cl, 1))), "b");
  TEST(exp_to_str(sw), "switch(cond){1: \"a\";default: \"b\";}");
  TEST(exp_to_js(sw), "switch(cond){1: \"a\";default: \"b\";}");
  TEST(exp_type_to_str(sw), "switch");

  Exp *nt = exp_not(exp_sym("true"));
  assert(exp_is_not(nt));
  TEST(exp_get_sym(exp_get_not(nt)), "true");
  TEST(exp_to_str(nt), "!true");
  TEST(exp_to_js(nt), "!true");
  TEST(exp_type_to_str(nt), "not");

  Exp *mn = exp_minus(exp_float(3.2));
  assert(exp_is_minus(mn));
  TESTF(exp_get_float(exp_get_minus(mn)), 3.2);
  TEST(exp_to_str(mn), "-3.2");
  TEST(exp_to_js(mn), "-3.2");
  TEST(exp_type_to_str(mn), "minus");

  Exp *add = exp_add(exp_int(1), exp_int(2));
  assert(exp_is_add(add));
  Tp *add_v = exp_get_add(add);
  TESTI(exp_get_int(tp_e1(add_v)), 1);
  TESTI(exp_get_int(tp_e2(add_v)), 2);
  TEST(exp_to_str(add), "1+2");
  TEST(exp_to_js(add), "1+2");
  TEST(exp_type_to_str(add), "add");

  Exp *sub = exp_sub(exp_int(1), exp_int(2));
  assert(exp_is_sub(sub));
  Tp *sub_v = exp_get_sub(sub);
  TESTI(exp_get_int(tp_e1(sub_v)), 1);
  TESTI(exp_get_int(tp_e2(sub_v)), 2);
  TEST(exp_to_str(sub), "1-2");
  TEST(exp_to_js(sub), "1-2");
  TEST(exp_type_to_str(sub), "sub");

  Exp *mul = exp_mul(exp_int(1), exp_int(2));
  assert(exp_is_mul(mul));
  Tp *mul_v = exp_get_mul(mul);
  TESTI(exp_get_int(tp_e1(mul_v)), 1);
  TESTI(exp_get_int(tp_e2(mul_v)), 2);
  TEST(exp_to_str(mul), "1*2");
  TEST(exp_to_js(mul), "1*2");
  TEST(exp_type_to_str(mul), "mul");

  Exp *div = exp_div(exp_int(1), exp_int(2));
  assert(exp_is_div(div));
  Tp *div_v = exp_get_div(div);
  TESTI(exp_get_int(tp_e1(div_v)), 1);
  TESTI(exp_get_int(tp_e2(div_v)), 2);
  TEST(exp_to_str(div), "1/2");
  TEST(exp_to_js(div), "1/2");
  TEST(exp_type_to_str(div), "div");

  Exp *mod = exp_mod(exp_int(1), exp_int(2));
  assert(exp_is_mod(mod));
  Tp *mod_v = exp_get_mod(mod);
  TESTI(exp_get_int(tp_e1(mod_v)), 1);
  TESTI(exp_get_int(tp_e2(mod_v)), 2);
  TEST(exp_to_str(mod), "1%2");
  TEST(exp_to_js(mod), "1%2");
  TEST(exp_type_to_str(mod), "mod");

  Exp *and = exp_and(exp_sym("false"), exp_sym("true"));
  assert(exp_is_and(and));
  Tp *and_v = exp_get_and(and);
  TEST(exp_get_sym(tp_e1(and_v)), "false");
  TEST(exp_get_sym(tp_e2(and_v)), "true");
  TEST(exp_to_str(and), "false&true");
  TEST(exp_to_js(and), "false&true");
  TEST(exp_type_to_str(and), "and");

  Exp *or = exp_or(exp_sym("false"), exp_sym("true"));
  assert(exp_is_or(or));
  Tp *or_v = exp_get_or(or);
  TEST(exp_get_sym(tp_e1(or_v)), "false");
  TEST(exp_get_sym(tp_e2(or_v)), "true");
  TEST(exp_to_str(or), "false|true");
  TEST(exp_to_js(or), "false|true");
  TEST(exp_type_to_str(or), "or");

  Exp *greater = exp_greater(exp_int(1), exp_int(2));
  assert(exp_is_greater(greater));
  Tp *greater_v = exp_get_greater(greater);
  TESTI(exp_get_int(tp_e1(greater_v)), 1);
  TESTI(exp_get_int(tp_e2(greater_v)), 2);
  TEST(exp_to_str(greater), "1>2");
  TEST(exp_to_js(greater), "1>2");
  TEST(exp_type_to_str(greater), "greater");

  Exp *greater_eq = exp_greater_eq(exp_int(1), exp_int(2));
  assert(exp_is_greater_eq(greater_eq));
  Tp *greater_eq_v = exp_get_greater_eq(greater_eq);
  TESTI(exp_get_int(tp_e1(greater_eq_v)), 1);
  TESTI(exp_get_int(tp_e2(greater_eq_v)), 2);
  TEST(exp_to_str(greater_eq), "1>=2");
  TEST(exp_to_js(greater_eq), "1>=2");
  TEST(exp_type_to_str(greater_eq), "greater_eq");

  Exp *less = exp_less(exp_int(1), exp_int(2));
  assert(exp_is_less(less));
  Tp *less_v = exp_get_less(less);
  TESTI(exp_get_int(tp_e1(less_v)), 1);
  TESTI(exp_get_int(tp_e2(less_v)), 2);
  TEST(exp_to_str(less), "1<2");
  TEST(exp_to_js(less), "1<2");
  TEST(exp_type_to_str(less), "less");

  Exp *less_eq = exp_less_eq(exp_int(1), exp_int(2));
  assert(exp_is_less_eq(less_eq));
  Tp *less_eq_v = exp_get_less_eq(less_eq);
  TESTI(exp_get_int(tp_e1(less_eq_v)), 1);
  TESTI(exp_get_int(tp_e2(less_eq_v)), 2);
  TEST(exp_to_str(less_eq), "1<=2");
  TEST(exp_to_js(less_eq), "1<=2");
  TEST(exp_type_to_str(less_eq), "less_eq");

  Exp *eq = exp_eq(exp_sym("false"), exp_sym("true"));
  assert(exp_is_eq(eq));
  Tp *eq_v = exp_get_eq(eq);
  TEST(exp_get_sym(tp_e1(eq_v)), "false");
  TEST(exp_get_sym(tp_e2(eq_v)), "true");
  TEST(exp_to_str(eq), "false==true");
  TEST(exp_to_js(eq), "false==true");
  TEST(exp_type_to_str(eq), "eq");

  Exp *neq = exp_neq(exp_sym("false"), exp_sym("true"));
  assert(exp_is_neq(neq));
  Tp *neq_v = exp_get_neq(neq);
  TEST(exp_get_sym(tp_e1(neq_v)), "false");
  TEST(exp_get_sym(tp_e2(neq_v)), "true");
  TEST(exp_to_str(neq), "false!=true");
  TEST(exp_to_js(neq), "false!=true");
  TEST(exp_type_to_str(neq), "neq");

  Exp *ter = exp_ternary(exp_sym("cond"), exp_int(1), exp_int(2));
  assert(exp_is_ternary(ter));
  Tp3 *ter_v = exp_get_ternary(ter);
  TEST(exp_get_sym(tp3_e1(ter_v)), "cond");
  TESTI(exp_get_int(tp3_e2(ter_v)), 1);
  TESTI(exp_get_int(tp3_e3(ter_v)), 2);
  TEST(exp_to_str(ter), "cond?1:2");
  TEST(exp_to_js(ter), "cond?1:2");
  TEST(exp_type_to_str(ter), "ternary");


  puts("  ... Finished");
}
