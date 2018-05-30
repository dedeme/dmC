// Copyright 30-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <assert.h>
#include "dmc/str.h"
#include "dmc/Achar.h"
#include "dmc/Buf.h"
#include "dmc/Opt.h"
#include "dmc/sys.h"

void str_test() {
  printf("str test\n");

  printf("    str-starts/ends\n");

  assert(str_starts("ab", ""));
  assert(str_starts("", ""));
  assert(str_starts("ab", "a"));
  assert(str_starts("ab", "ab"));
  assert(!str_starts("", "b"));
  assert(!str_starts("ab", "b"));
  assert(!str_starts("ab", "abc"));

  assert(str_ends("ab", ""));
  assert(str_ends("", ""));
  assert(str_ends("ab", "b"));
  assert(str_ends("ab", "ab"));
  assert(!str_ends("", "b"));
  assert(!str_ends("ab", "a"));
  assert(!str_ends("ab", "cab"));

  printf("    str-index/last_index\n");

  assert(str_cindex("ab", 'a') == 0);
  assert(str_cindex("", 'a') == -1);
  assert(str_cindex("ab", 'b') == 1);
  assert(str_cindex("ab", 'c') == -1);

  assert(str_index("ab", "a") == 0);
  assert(str_index("", "a") == -1);
  assert(str_index("ab", "b") == 1);
  assert(str_index("ab", "c") == -1);
  assert(str_index("ab", "") == 0);
  assert(str_index("", "") == 0);
  assert(str_index("ab", "abc") == -1);
  assert(str_index("abcd", "bc") == 1);
  assert(str_index("abcd", "bcd") == 1);
  assert(str_index("abcd", "d") == 3);
  assert(str_index("abcd", "ad") == -1);

  assert(str_last_cindex("ab", 'a') == 0);
  assert(str_last_cindex("aba", 'a') == 2);
  assert(str_last_cindex("", 'a') == -1);
  assert(str_last_cindex("ab", 'b') == 1);
  assert(str_last_cindex("ab", 'c') == -1);

  assert(str_last_index("ab", "a") == 0);
  assert(str_last_index("abac", "a") == 2);
  assert(str_last_index("", "a") == -1);
  assert(str_last_index("ab", "b") == 1);
  assert(str_last_index("abcb", "b") == 3);
  assert(str_last_index("ab", "c") == -1);
  assert(str_last_index("ab", "") == 0);
  assert(str_last_index("", "") == 0);
  assert(str_last_index("ab", "abc") == -1);
  assert(str_last_index("abcd", "bc") == 1);
  assert(str_last_index("abcdbc", "bc") == 4);
  assert(str_last_index("abcd", "bcd") == 1);
  assert(str_last_index("abcd", "d") == 3);
  assert(str_last_index("abcddd", "d") == 5);
  assert(str_last_index("abcd", "ad") == -1);

  printf("    str-copy/cat\n");

  char *s0 = "";
  char *s1 = "1";

  char *r;

  r = str_copy(s0);
  assert(str_eq(r, ""));
  r = str_copy(s1);
  assert(str_eq(r, "1"));

  r = str_cat(s0, NULL);
  assert(str_eq(r, ""));
  r = str_cat(s1, NULL);
  assert(str_eq(r, "1"));
  r = str_cat(s0, "ab", NULL);
  assert(str_eq(r, "ab"));
  r = str_cat(s1, "ab", NULL);
  assert(str_eq(r, "1ab"));
  r = str_cat(s0, "ab", "", "cd", NULL);
  assert(str_eq(r, "abcd"));
  r = str_cat(s1, "ab", s1, "cd", NULL);
  assert(str_eq(r, "1ab1cd"));

  printf("    str-sub\n");

  r = str_sub("ab", 0, 1);
  assert(str_eq(r, "a"));
  r = str_sub("ab", -2, -1);
  assert(str_eq(r, "a"));
  r = str_sub("ab", 0, 0);
  assert(str_eq(r, ""));
  r = str_sub("ab", -1, 0);
  assert(str_eq(r, ""));
  r = str_sub("ab", 0, 35);
  assert(str_eq(r, ""));
  r = str_sub("ab", 3, 3);
  assert(str_eq(r, ""));
  r = str_sub("ab", 2, 2);
  assert(str_eq(r, ""));
  r = str_sub("ab", 0, 2);
  assert(str_eq(r, "ab"));
  r = str_sub("ab", 1, 2);
  assert(str_eq(r, "b"));
  r = str_sub_end("", 0);
  assert(str_eq(r, ""));
  r = str_sub_end("ab", 32);
  assert(str_eq(r, ""));
  r = str_sub_end("ab", 0);
  assert(str_eq(r, "ab"));
  r = str_sub_end("ab", -1);
  assert(str_eq(r, "b"));

  printf("    str-trim\n");

  r = str_trim("nothing to trim");
  assert(str_eq(r, "nothing to trim"));
  r = str_trim("trim the back     ");
  assert(str_eq(r, "trim the back"));
  r = str_trim(" trim one char front and back ");
  assert(str_eq(r, "trim one char front and back"));
  r = str_trim(" trim one char front");
  assert(str_eq(r, "trim one char front"));
  r = str_trim("trim one char back ");
  assert(str_eq(r, "trim one char back"));
  r = str_trim("                   ");
  assert(str_eq(r, ""));
  r = str_trim(" ");
  assert(str_eq(r, ""));
  r = str_trim("a");
  assert(str_eq(r, "a"));
  r = str_trim("");
  assert(str_eq(r, ""));
  r = str_ltrim("nothing to trim");
  assert(str_eq(r, "nothing to trim"));
  r = str_ltrim("trim the back     ");
  assert(str_eq(r, "trim the back     "));
  r = str_ltrim(" trim one char front and back ");
  assert(str_eq(r, "trim one char front and back "));
  r = str_ltrim(" trim one char front");
  assert(str_eq(r, "trim one char front"));
  r = str_ltrim("trim one char back ");
  assert(str_eq(r, "trim one char back "));
  r = str_ltrim("                   ");
  assert(str_eq(r, ""));
  r = str_ltrim(" ");
  assert(str_eq(r, ""));
  r = str_ltrim("a");
  assert(str_eq(r, "a"));
  r = str_ltrim("");
  assert(str_eq(r, ""));
  r = str_rtrim("nothing to trim");
  assert(str_eq(r, "nothing to trim"));
  r = str_rtrim("trim the back     ");
  assert(str_eq(r, "trim the back"));
  r = str_rtrim(" trim one char front and back ");
  assert(str_eq(r, " trim one char front and back"));
  r = str_rtrim(" trim one char front");
  assert(str_eq(r, " trim one char front"));
  r = str_rtrim("trim one char back ");
  assert(str_eq(r, "trim one char back"));
  r = str_rtrim("                   ");
  assert(str_eq(r, ""));
  r = str_rtrim(" ");
  assert(str_eq(r, ""));
  r = str_rtrim("a");
  assert(str_eq(r, "a"));
  r = str_rtrim("");
  assert(str_eq(r, ""));

  printf("    str-split/join\n");

  Achar *a;
  char *tx0 = "";
  char *tx01 = ";";
  char *tx1 = "ab";
  char *tx2 = "ab;";
  char *tx3 = "ab;c;de";
  char *tx01b = ";--";
  char *tx2b = "ab;--";
  char *tx3b = "ab ;--  c \t ;--  de";
  char *tx3c = "ab;--c;--de";

  a = str_csplit(tx0, ';');
  r = str_cjoin(a, ';');
  assert(str_eq(r, tx0));
  a = str_csplit(tx01, ';');
  r = str_cjoin(a, ';');
  assert(str_eq(r, ""));
  a = str_csplit(tx1, ';');
  r = str_cjoin(a, ';');
  assert(str_eq(r, tx1));
  a = str_csplit(tx2, ';');
  r = str_cjoin(a, ';');
  assert(str_eq(r, tx1));
  a = str_csplit(tx3, ';');
  r = str_cjoin(a, ';');
  assert(str_eq(r, tx3));
  a = str_split(tx0, ";");
  r = str_join(a, ";");
  assert(str_eq(r, tx0));
  a = str_split(tx01, ";");
  r = str_join(a, ";");
  assert(str_eq(r, ""));
  a = str_split(tx1, ";");
  r = str_join(a, ";");
  assert(str_eq(r, tx1));
  a = str_split(tx2, ";");
  r = str_join(a, ";");
  assert(str_eq(r, tx1));
  a = str_split(tx3, ";");
  r = str_join(a, ";");
  assert(str_eq(r, tx3));
  a = str_split(tx0, ";--");
  r = str_join(a, ";--");
  assert(str_eq(r, tx0));
  a = str_split(tx01b, ";--");
  r = str_join(a, ";--");
  assert(str_eq(r, ""));
  a = str_split(tx1, ";--");
  r = str_join(a, ";--");
  assert(str_eq(r, tx1));
  a = str_split(tx2b, ";--");
  r = str_join(a, ";--");
  assert(str_eq(r, tx1));
  a = str_split(tx3b, ";--");
  r = str_join(a, ";--");
  assert(str_eq(r, tx3b));
  a = str_split_trim(tx3b, ";--");
  r = str_join(a, ";--");
  assert(str_eq(r, tx3c));

  printf("    str-replace\n");

  r = str_copy("");
  r = str_creplace(r, '.', ',');
  assert(str_eq(r, ""));
  r = str_copy(".");
  r = str_creplace(r, '.', ',');
  assert(str_eq(r, ","));
  r = str_copy("f.j");
  r = str_creplace(r, '.', ',');
  assert(str_eq(r, "f,j"));
  r = str_copy("f.j.");
  r = str_creplace(r, '.', ',');
  assert(str_eq(r, "f,j,"));

  r = str_replace("", ".", "");
  assert(str_eq(r, ""));
  r = str_replace(".", ".", "");
  assert(str_eq(r, ""));
  r = str_replace("f.j", ".", "");
  assert(str_eq(r, "fj"));
  r = str_replace("f.j.", ".", "");
  assert(str_eq(r, "fj"));

  r = str_replace("", "..", "---");
  assert(str_eq(r, ""));
  r = str_replace("..", "..", "---");
  assert(str_eq(r, "---"));
  r = str_replace("f..j", "..", "---");
  assert(str_eq(r, "f---j"));
  r = str_replace("f.j..", "..", "---");
  assert(str_eq(r, "f.j---"));

  printf("    str-printf\n");

  r = str_printf("");
  assert(str_eq(r, ""));
  r = str_printf("ab");
  assert(str_eq(r, "ab"));
  r = str_printf("Result is %d", 1);
  assert(str_eq(r, "Result is 1"));
  r = str_printf("Your %s number %d", "book", 1);
  assert(str_eq(r, "Your book number 1"));
  r = str_printf("%s%s%s", "1", "ab", "");
  assert(str_eq(r, "1ab"));
  r = str_printf("%s%s%s", "1", "ab", "c");
  assert(str_eq(r, "1abc"));

  printf("    str-utf8\n");

  assert(0 == str_runes(""));
  assert(1 == str_runes("a"));
  assert(1 == str_runes("\n"));
  assert(1 == str_runes("ú"));
  assert(1 == str_runes("ñ"));
  assert(1 == str_runes("€"));
  assert(3 == str_runes("a b"));
  assert(3 == str_runes("a\nb"));
  assert(3 == str_runes("añb"));
  assert(3 == str_runes("a€b"));
  assert(7 == str_runes(" a\tb €\n"));

  assert(str_eq(str_next_rune(""), ""));
  assert(str_eq(str_next_rune("a"), "a"));
  assert(str_eq(str_next_rune("ñ"), "ñ"));

  char *s3 = "| a\tb €\n";
  int pos = 0;
  int i;
  Buf *bf = buf_new();
  for (i = 0; i < str_runes(s3); i++) {
    r = str_next_rune(s3 + pos);
    pos += str_len(r);
    buf_add(bf, r);
  }
  assert(str_eq(buf_str(bf), s3));

  unsigned *u = NULL;
  u = opt_value(str_to_unicode(""));
  r = opt_value(str_from_unicode(u));
  assert(str_eq("", r));
  u = opt_value(str_to_unicode("a"));
  r = opt_value(str_from_unicode(u));
  assert(str_eq("a", r));
  u = opt_value(str_to_unicode("\n"));
  r = opt_value(str_from_unicode(u));
  assert(str_eq("\n", r));
  u = opt_value(str_to_unicode("ú"));
  r = opt_value(str_from_unicode(u));
  assert(str_eq("ú", r));
  u = opt_value(str_to_unicode("ñ"));
  r = opt_value(str_from_unicode(u));
  assert(str_eq("ñ", r));
  u = opt_value(str_to_unicode("€"));
  r = opt_value(str_from_unicode(u));
  assert(str_eq("€", r));
  u = opt_value(str_to_unicode("a b"));
  r = opt_value(str_from_unicode(u));
  assert(str_eq("a b", r));
  u = opt_value(str_to_unicode("a\nb"));
  r = opt_value(str_from_unicode(u));
  assert(str_eq("a\nb", r));
  u = opt_value(str_to_unicode("a€b"));
  r = opt_value(str_from_unicode(u));
  assert(str_eq("a€b", r));
  u = opt_value(str_to_unicode("añb"));
  r = opt_value(str_from_unicode(u));
  assert(str_eq("añb", r));
  u = opt_value(str_to_unicode(" a\tb €\n"));
  r = opt_value(str_from_unicode(u));
  assert(str_eq(" a\tb €\n", r));

  printf("    str-to_upper/to_lower\n");

  sys_locale("es_ES.utf8");
  assert(str_eq("CAÑÓN", str_to_upper("cañón")));
  assert(str_eq("cañón", str_to_lower("CAÑÓN")));
  sys_locale("C");

  printf("    Finished\n");
}
