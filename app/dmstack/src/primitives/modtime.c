// Copyright 19-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modtime.h"
#include <time.h>
#include <limits.h>
#include "Machine.h"
#include "fails.h"

static Token *to (char *tpl, struct tm *tp) {
  char *s, *rs;
  int size = 126;
  while (1) {
    rs = (char *)calloc(size, 1); // calloc ok
    if (strftime (rs, size, tpl, tp)) {
      s = str_new(rs);
      free(rs); // free ok
      break;
    }
    free(rs); // free ok
    size += size;
  }
  return token_new_string(0, s);
}

static char *rdg (int *dg, int n, char *p) {
  Buf *bf = buf_new();
  int c = 0;
  while (*p >= '0' && *p <= '9' && c++ < n) buf_cadd(bf, *p++);
  if (*buf_str(bf)) *dg = atoi (buf_str(bf));
  else *dg = INT_MAX;
  return p;
}

// Admits Y y m (1-12) d (1-31) H M S
// Opt<Token>
static Opt *from (char *tpl, char *time, char *tplout) {
  struct tm tp;
  memset(&tp, 0, sizeof(struct tm));

  while (*tpl && *time) {
    char tplc = *tpl++;
    if (tplc == '%') {
      char tplc = *tpl++;
      if (tplc == '%') {
        if (*time++ != '%') return opt_empty();
      } else {
        int v;
        switch (tplc) {
          case 'Y':
            time = rdg(&v, 4, time);
            if (v == INT_MAX) return opt_empty();
            tp.tm_year = v - 1900;
            break;
          case 'y':
            time = rdg(&v, 2, time);
            if (v == INT_MAX) return opt_empty();
            tp.tm_year = 100 + v;
            break;
          case 'm':
            time = rdg(&v, 2, time);
            if (v == INT_MAX) return opt_empty();
            tp.tm_mon = v - 1;
            break;
          case 'd':
            time = rdg(&v, 2, time);
            if (v == INT_MAX) return opt_empty();
            tp.tm_mday = v;
            break;
          case 'H':
            time = rdg(&v, 2, time);
            if (v == INT_MAX) return opt_empty();
            tp.tm_hour = v;
            break;
          case 'M':
            time = rdg(&v, 2, time);
            if (v == INT_MAX) return opt_empty();
            tp.tm_min = v;
            break;
          case 'S':
            time = rdg(&v, 2, time);
            if (v == INT_MAX) return opt_empty();
            tp.tm_sec = v;
            break;
          default: return opt_empty();
        }
      }
    } else if (tplc != *time++) {
      return opt_empty();
    }
  }

  return opt_new(to(tplout, &tp));
}

static void new (Machine *m) {
  int second = token_int(machine_pop_exc(m, token_INT));
  int minute = token_int(machine_pop_exc(m, token_INT));
  int hour = token_int(machine_pop_exc(m, token_INT));
  int day = token_int(machine_pop_exc(m, token_INT));
  int month = token_int(machine_pop_exc(m, token_INT));
  int year = token_int(machine_pop_exc(m, token_INT));

  struct tm tp;
  memset(&tp, 0, sizeof(struct tm));
  tp.tm_year = year - 1900;
  tp.tm_mon = month - 1;
  tp.tm_mday = day;
  tp.tm_hour = hour;
  tp.tm_min = minute;
  tp.tm_sec = second;

  machine_push(m, to("%Y%m%d%H%M%S", &tp));
}

static void newdate (Machine *m) {
  int day = token_int(machine_pop_exc(m, token_INT));
  int year = token_int(machine_pop_exc(m, token_INT));
  int month = token_int(machine_pop_exc(m, token_INT));

  struct tm tp;
  memset(&tp, 0, sizeof(struct tm));
  tp.tm_year = year - 1900;
  tp.tm_mon = month - 1;
  tp.tm_mday = day;
  tp.tm_hour = 12;

  machine_push(m, to("%Y%m%d%H%M%S", &tp));
}

static void fromstr (Machine *m) {
  char *time = token_string(machine_pop_exc(m, token_STRING));
  char *tpl = token_string(machine_pop_exc(m, token_STRING));
  Token *r = opt_nget(from(tpl, time, "%Y%m%d%H%M%S"));
  if (r) machine_push(m, token_new_list(0, arr_new_from(r, NULL)));
  else machine_push(m, token_new_list(0, arr_new()));
}

static void fromdate (Machine *m) {
  char *time = token_string(machine_pop_exc(m, token_STRING));
  time = str_f("%s:12", time);
  Token *r = opt_nget(from("%Y%m%d%H", time, "%Y%m%d%H%M%S"));
  if (r) machine_push(m, token_new_list(0, arr_new_from(r, NULL)));
  else machine_push(m, token_new_list(0, arr_new()));
}

static void todate (Machine *m) {
  machine_push(m, token_new_string(
    0, str_left(token_string(machine_pop_exc(m, token_STRING)),8)
  ));
}

static void totime (Machine *m) {
  char *t = str_right(token_string(machine_pop_exc(m, token_STRING)),8);
  machine_push(m, token_new_string(
    0, str_f("%s:%s:%s", str_left(t, 2), str_sub(t, 2, 4), str_right(t, 4))
  ));
}

static void format (Machine *m) {
  char *time = token_string(machine_pop_exc(m, token_STRING));
  char *tpl = token_string(machine_pop_exc(m, token_STRING));
  Token *tk = opt_nget(from("%Y%m%d%H%M%S", time, tpl));
  if (tk) machine_push(m, tk);
  else machine_fail(m, str_f("Bad time: '%s'", time));
}

static void now (Machine *m) {
  time_t t = time(NULL);
  machine_push(m, to("%Y%m%d%H%M%S", localtime(&t)));
}

static void broke (Machine *m) {
  char *t = token_string(machine_pop_exc(m, token_STRING));

  void fail (void) { machine_fail(m, str_f("Bad time: '%s'", t)); }

  // Arr<Token>
  Arr *a = arr_new();

  void add (char *id, int v) {
    arr_push(a, token_new_string(0, id));
    arr_push(a, token_new_int(0, v));
  }

  int v;
  char *p = t;
  p = rdg(&v, 4, p);
  if (v == INT_MAX) fail();
  add("year", v);
  p = rdg(&v, 2, p);
  if (v == INT_MAX) fail();
  add("month", v);
  p = rdg(&v, 2, p);
  if (v == INT_MAX) fail();
  add("day", v);
  p = rdg(&v, 2, p);
  if (v == INT_MAX) fail();
  add("hour", v);
  p = rdg(&v, 2, p);
  if (v == INT_MAX) fail();
  add("minute", v);
  p = rdg(&v, 2, p);
  if (v == INT_MAX) fail();
  add("second", v);

  machine_push(m, token_new_list(0, a));
}

static struct tm *to_broken_date (Machine *m, char *t) {
  void fail (void) { machine_fail(m, str_f("Bad time: '%s'", t)); }

  struct tm *tp = MALLOC(struct tm);
  memset(tp, 0, sizeof(struct tm));

  int v;
  char *p = t;
  p = rdg(&v, 4, p);
  if (v == INT_MAX) fail();
  tp->tm_year = v - 1900;
  p = rdg(&v, 2, p);
  if (v == INT_MAX) fail();
  tp->tm_mon = v - 1;
  p = rdg(&v, 2, p);
  if (v == INT_MAX) fail();
  tp->tm_mday = v;
  p = rdg(&v, 2, p);
  if (v == INT_MAX) fail();
  tp->tm_hour = v;
  p = rdg(&v, 2, p);
  if (v == INT_MAX) fail();
  tp->tm_min = v;
  p = rdg(&v, 2, p);
  if (v == INT_MAX) fail();
  tp->tm_sec = v;

  return tp;
}

static void add (Machine *m) {
  int n = token_int(machine_pop_exc(m, token_INT));
  char *t = token_string(machine_pop_exc(m, token_STRING));

  time_t tt = mktime(to_broken_date(m, t)) + n * 86400;
  machine_push(m, to("%Y%m%d%H%M%S", localtime(&tt)));
}

static void df (Machine *m) {
  char *t2 = token_string(machine_pop_exc(m, token_STRING));
  char *t1 = token_string(machine_pop_exc(m, token_STRING));

  time_t tt1 = mktime(to_broken_date(m, t1));
  time_t tt2 = mktime(to_broken_date(m, t2));
  double tmp = difftime(tt1, tt2) / 86400;
  int r  = (int)(tmp >= 0 ? tmp + 0.5 : tmp - 0.5);
  machine_push(m, token_new_int(0, r));
}

// Resturns Map<primitives_Fn>
Map *modtime_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "new", new); // [INT, INT, INT, INT, INT, INT] - STRING
                          // [year, month, day, hour, min, sec] - date
  map_put(r, "newDate", newdate); // [INT, INT, INT] - STRING
                                  // [year, month, day] - date
  map_put(r, "fromStr", fromstr); // [STRING, STRING] - OPT<STRING>
                                  // [template, date] - option of date
  map_put(r, "fromDate", fromdate); // STRING - STRING
  map_put(r, "toDate", todate); // STRING - STRING
  map_put(r, "toTime", totime); // STRING - STRING
  map_put(r, "format", format); // [STRING - STRING] - STRING
                               // [template, date] - date
  map_put(r, "now", now); // [] - STRING
  map_put(r, "broke", broke); // STRING - OBJ
  map_put(r, "add", add); // [STRING - INT] - STRING
  map_put(r, "df", df); // [STRING - STRING] - INT

  return r;
}





