// Copyright 30-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <assert.h>
#include "dmc/Achar.h"
#include "dmc/sys.h"


void arr_test() {
  printf("Arr test\n");
/*
  sys_locale("es_ES.utf8");

  Arr *a1, *a2, *a3, *a4;

  a1 = arr_new();
  a2 = arr_new();

  assert(arr_size(a1) == 0);
  assert(arr_size(a2) == 0);
  assert(it_eq(arr_to_it(a1), arr_to_it(a2), str_eq));
  assert(it_eq_str(arr_to_it(a1), arr_to_it(a2)));
  int i;
  for(i = 0; i < 17; ++i)
    arr_add(a1, str_printf("%d", i));

  for(i = 0; i < 17; ++i) {
    char *s = str_printf("%d", i);
    assert(!strcmp(s, arr_get(a1, i)));
  }
  assert(!it_eq(it_from(a1), it_from(a2), str_eq));
  assert(!it_eq_str(arr_to_it(a1), arr_to_it(a2)));

  arr_reverse(a1);
  for(i = 0; i < 17; ++i) {
    char *s = str_printf("%d", 16 - i);
    assert(!strcmp(s, arr_es(a1)[i]));
  }
  FNE(cb, char, o1, o2) { return atoi(o1) > atoi(o2); }_FN
  arr_sort(a1, cb);

  for(i = 0; i < 17; ++i) {
    char *s = str_printf("%d", i);
    assert(!strcmp(s, arr_get(a1, i)));
  }

  arr_add(a2, "aaa");
  arr_add(a2, "bbb");
  arr_add(a2, "ccc");
  arr_add(a2, "ddd");
  arr_add(a2, "eee");
  assert(!strcmp(arr_es(a2)[0], "aaa"));
  assert(!strcmp(arr_es(a2)[1], "bbb"));
  assert(!strcmp(arr_es(a2)[2], "ccc"));
  assert(!strcmp(arr_es(a2)[3], "ddd"));
  assert(!strcmp(arr_es(a2)[4], "eee"));

  arr_reverse(a2);
  assert(!strcmp(arr_es(a2)[0], "eee"));
  assert(!strcmp(arr_es(a2)[1], "ddd"));
  assert(!strcmp(arr_es(a2)[2], "ccc"));
  assert(!strcmp(arr_es(a2)[3], "bbb"));
  assert(!strcmp(arr_es(a2)[4], "aaa"));
  arr_sort_str(a2);
  assert(!strcmp(arr_es(a2)[0], "aaa"));
  assert(!strcmp(arr_es(a2)[1], "bbb"));
  assert(!strcmp(arr_es(a2)[2], "ccc"));
  assert(!strcmp(arr_es(a2)[3], "ddd"));
  assert(!strcmp(arr_es(a2)[4], "eee"));

  char *lc = setlocale(LC_ALL, NULL);
  if (str_starts(lc, "es") || str_starts(lc, "ES")) {
    arr_set(a2, 0, "ááá");
    arr_reverse(a2);
    arr_sort_locale(a2);
    assert(!strcmp(arr_es(a2)[0], "ááá"));
    assert(!strcmp(arr_es(a2)[1], "bbb"));
    assert(!strcmp(arr_es(a2)[2], "ccc"));
    assert(!strcmp(arr_es(a2)[3], "ddd"));
    assert(!strcmp(arr_es(a2)[4], "eee"));
    arr_set(a2, 0, "aaa");
  }

  arr_insert(a2, 2, "x");
  assert(arr_size(a2) == 6);
  assert(!strcmp(arr_es(a2)[0], "aaa"));
  assert(!strcmp(arr_es(a2)[1], "bbb"));
  assert(!strcmp(arr_es(a2)[2], "x"));
  assert(!strcmp(arr_es(a2)[3], "ccc"));
  assert(!strcmp(arr_es(a2)[4], "ddd"));
  assert(!strcmp(arr_es(a2)[5], "eee"));

  arr_set(a2, 2, "xxx");
  assert(!strcmp(arr_es(a2)[0], "aaa"));
  assert(!strcmp(arr_es(a2)[1], "bbb"));
  assert(!strcmp(arr_es(a2)[2], "xxx"));
  assert(!strcmp(arr_es(a2)[3], "ccc"));
  assert(!strcmp(arr_es(a2)[4], "ddd"));
  assert(!strcmp(arr_es(a2)[5], "eee"));

  arr_set(a2, 0, "a");
  arr_set(a2, 5, "e");
  assert(!strcmp(arr_es(a2)[0], "a"));
  assert(!strcmp(arr_es(a2)[1], "bbb"));
  assert(!strcmp(arr_es(a2)[2], "xxx"));
  assert(!strcmp(arr_es(a2)[3], "ccc"));
  assert(!strcmp(arr_es(a2)[4], "ddd"));
  assert(!strcmp(arr_es(a2)[5], "e"));

  arr_remove(a2, 0);
  arr_remove(a2, 4);
  assert(!strcmp(arr_es(a2)[0], "bbb"));
  assert(!strcmp(arr_es(a2)[1], "xxx"));
  assert(!strcmp(arr_es(a2)[2], "ccc"));
  assert(!strcmp(arr_es(a2)[3], "ddd"));

  arr_remove(a2, 1);
  assert(!strcmp(arr_es(a2)[0], "bbb"));
  assert(!strcmp(arr_es(a2)[1], "ccc"));
  assert(!strcmp(arr_es(a2)[2], "ddd"));

  arr_insert(a2, 0, "aaa");
  arr_insert(a2, arr_size(a2), "eee");
  assert(!strcmp(arr_es(a2)[0], "aaa"));
  assert(!strcmp(arr_es(a2)[1], "bbb"));
  assert(!strcmp(arr_es(a2)[2], "ccc"));
  assert(!strcmp(arr_es(a2)[3], "ddd"));
  assert(!strcmp(arr_es(a2)[4], "eee"));

  a3 = arr_from_it(
    it_drop(
      it_take(
        arr_to_it(a2),
        4),
      1)
    );

  arr_remove_range(a2, 1, 4);

  assert(!strcmp(arr_es(a2)[0], "aaa"));
  assert(!strcmp(arr_es(a2)[1], "eee"));
  assert(arr_size(a2) == 2);

  assert(!strcmp(arr_es(a3)[0], "bbb"));
  assert(!strcmp(arr_es(a3)[1], "ccc"));
  assert(!strcmp(arr_es(a3)[2], "ddd"));
  assert(arr_size(a3) == 3);

  arr_add_arr(a2, a3);
  assert(arr_size(a2) == 5);
  assert(!strcmp(arr_es(a2)[0], "aaa"));
  assert(!strcmp(arr_es(a2)[1], "eee"));
  assert(!strcmp(arr_es(a2)[2], "bbb"));
  assert(!strcmp(arr_es(a2)[3], "ccc"));
  assert(!strcmp(arr_es(a2)[4], "ddd"));

  arr_insert_arr(a2, 0, a3);
  assert(arr_size(a2) == 8);
  assert(!strcmp(arr_es(a3)[0], "bbb"));
  assert(!strcmp(arr_es(a3)[1], "ccc"));
  assert(!strcmp(arr_es(a3)[2], "ddd"));
  assert(!strcmp(arr_es(a2)[3], "aaa"));
  assert(!strcmp(arr_es(a2)[4], "eee"));
  assert(!strcmp(arr_es(a2)[5], "bbb"));
  assert(!strcmp(arr_es(a2)[6], "ccc"));
  assert(!strcmp(arr_es(a2)[7], "ddd"));

  arr_remove_range(a2, 0, 3);
  assert(arr_size(a2) == 5);
  arr_remove_range(a2, 2, 5);
  assert(arr_size(a2) == 2);
  assert(!strcmp(arr_es(a2)[0], "aaa"));
  assert(!strcmp(arr_es(a2)[1], "eee"));


  a4 = arr_new();
  for(i = 0; i < 100; ++i) {
    arr_add(a4, str_printf("%d", i));
  }
  assert(!it_eq(arr_to_it(a1), arr_to_it(a4), str_eq));
  assert(!it_eq_str(arr_to_it(a1), arr_to_it(a4)));
  arr_insert_arr(a1, 11, a4);
  assert(arr_size(a1) == 117);
  assert(!strcmp(arr_es(a1)[116], "16"));

  arr_remove(a1, 0);
  assert(arr_size(a1) == 116);
  assert(!strcmp(arr_es(a1)[0], "1"));
  assert(!strcmp(arr_es(a1)[115], "16"));

  a4 = arr_from_it(
    it_drop(
      it_take(
        arr_to_it(a1),
        110),
      10)
    );

  arr_remove_range(a1, 10, 110);
  assert(arr_size(a1) == 16);
  assert(arr_size(a4) == 100);
  for(i = 0; i < 16; ++i) {
    char *s = str_printf("%d", i + 1);
    assert(!strcmp(s, arr_es(a1)[i]));
  }

  arr_shuffle(a4);
  for(i = 0; i < arr_size(a4); ++i) {
//    printf("%s\n", (char *)arr_es(a4)[i]);
  }
  FNE (sort4, char, e1, e2) { return atoi(e1) > atoi(e2); }_FN
  arr_sort(a4, sort4);
  for(i = 0; i < arr_size(a4); ++i) {
    assert(i == atoi(arr_es(a4)[i]));
  }

  int sort5 (void *e1, void *e2) {
    return atoi((char *)e1) - atoi((char *)e2);
  }
  assert(arr_sindex(a4, "0", sort5) == 0);
  assert(arr_sindex(a4, "24", sort5) == 24);
  assert(arr_sindex(a4, "80", sort5) == 80);
  assert(arr_sindex(a4, "99", sort5) == 99);
  assert(arr_sindex(a4, "100", sort5) == -1);

  i = 0;
  EACH(a1, char, o) {
    assert(!strcmp(o, arr_es(a1)[i++]));
  }_EACH

  EACHR(a1, char, o) {
    assert(!strcmp(o, arr_es(a1)[--i]));
  }_EACH

  assert(it_eq(arr_to_it(a1), arr_to_it(a1), str_eq));
  assert(it_eq_str(arr_to_it(a1), arr_to_it(a1)));

  assert(15 == it_index_str(arr_to_it(a1), "16"));
*/
  printf( "    Finished\n");
}

