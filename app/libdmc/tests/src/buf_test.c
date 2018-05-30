// Copyright 30-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <assert.h>
#include "dmc/Buf.h"
#include "dmc/str.h"

void buf_test() {
  printf("Buf test\n");

  char *s0 = "";
  char *s1 = "1";
  char *s2 = "abc";
  Buf *bf = buf_new();

  buf_add(bf, s0);
  assert(str_eq(buf_str(bf), ""));
  buf_cadd(bf, '+');
  assert(str_eq(buf_str(bf), "+"));
  buf_add(bf, "0");
  assert(str_eq(buf_str(bf), "+0"));
  buf_add(bf, s1);
  assert(str_eq(buf_str(bf), "+01"));
  buf_add(bf, s2);
  assert(str_eq(buf_str(bf), "+01abc"));
  buf_add(bf, s0);
  assert(str_eq(buf_str(bf), "+01abc"));
  buf_add(bf, s2);
  assert(str_eq(buf_str(bf), "+01abcabc"));
  buf_add(bf, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  assert(str_eq(buf_str(bf), "+01abcabc"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));

  printf("    Finished\n");
}
