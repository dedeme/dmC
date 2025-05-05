// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "char.h"
#include "DEFS.h"

int char_is_letter (char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

int char_is_digit (char ch) {
  return (ch >= '0' && ch <= '9');
}

int char_is_letter_or_digit (char ch) {
  return char_is_letter(ch) || char_is_digit(ch);
}

int char_is_id (char *id) {
  if (!*id) return FALSE;
  if (!char_is_letter(*id++)) return FALSE;
  while (*id) if (!char_is_letter_or_digit(*id++)) return FALSE;
  return TRUE;
}

Rs *char_mk_dot_id (char *id) {
  // Rs<char>
  Rs *check (char *id) {
    return char_is_id(id)
      ? rs_ok(id)
      : rs_fail(str_f("'%s' is not a valid identifier", id))
    ;
  }

  int ix = str_cindex(id, '.');
  if (ix == -1) return check(id);
  if (ix == 0) return rs_fail("Identifier start with dot");
  Rs *r1 = check(str_rtrim(str_left(id, ix)));
  char *tx1 = rs_get(r1);
  if (!tx1) return r1;
  Rs *r2 = check(str_ltrim(str_right(id, ix+1)));
  char *tx2 = rs_get(r2);
  if (!tx2) return r2;
  return rs_ok(str_f("%s.%s", tx1, tx2));
}
