// Copyright 19-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modtime.h"
#include <time.h>
#include <limits.h>
#include "tk.h"
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
static Opt *from (char *tpl, char *time) {
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

  return opt_new(token_new_int(0, mktime(&tp)));
}

static void new (Machine *m) {
  Int second = tk_pop_int(m);
  Int minute = tk_pop_int(m);
  Int hour = tk_pop_int(m);
  Int day = tk_pop_int(m);
  Int month = tk_pop_int(m);
  Int year = tk_pop_int(m);

  struct tm tp;
  memset(&tp, 0, sizeof(struct tm));
  tp.tm_year = year - 1900;
  tp.tm_mon = month - 1;
  tp.tm_mday = day;
  tp.tm_hour = hour;
  tp.tm_min = minute;
  tp.tm_sec = second;

  machine_push(m, token_new_int(0, mktime(&tp)));
}

static void newdate (Machine *m) {
  int day = tk_pop_int(m);
  int year = tk_pop_int(m);
  int month = tk_pop_int(m);

  struct tm tp;
  memset(&tp, 0, sizeof(struct tm));
  tp.tm_year = year - 1900;
  tp.tm_mon = month - 1;
  tp.tm_mday = day;
  tp.tm_hour = 12;

  machine_push(m, token_new_int(0, mktime(&tp)));
}

static void fromstr (Machine *m) {
  char *time = tk_pop_string(m);
  char *tpl = tk_pop_string(m);
  Token *r = opt_nget(from(tpl, time));
  if (r) machine_push(m, token_new_list(0, arr_new_from(r, NULL)));
  else machine_push(m, token_new_list(0, arr_new()));
}

static void fromdate (Machine *m) {
  char *time = tk_pop_string(m);
  time = str_f("%s:12", time);
  Token *r = opt_nget(from("%Y%m%d%H", time));
  if (r) machine_push(m, token_new_list(0, arr_new_from(r, NULL)));
  else machine_push(m, token_new_list(0, arr_new()));
}

static void todate (Machine *m) {
  time_t tm = tk_pop_int(m);
  machine_push(m, to("%Y%m%d", localtime(&tm)));
}

static void totime (Machine *m) {
  time_t tm = tk_pop_int(m);
  machine_push(m, to("%H:%M:%S", localtime(&tm)));
}

static void todatetime (Machine *m) {
  time_t tm = tk_pop_int(m);
  machine_push(m, to("%Y%m%d%H%M%S", localtime(&tm)));
}

static void format (Machine *m) {
  char *tpl = tk_pop_string(m);
  time_t tm = tk_pop_int(m);
  machine_push(m, to(tpl, localtime(&tm)));
}

static void now (Machine *m) {
  machine_push(m, token_new_int(0, time(NULL)));
}

static void broke (Machine *m) {
  time_t t = tk_pop_int(m);
  struct tm *tm = localtime(&t);

  // Arr<Token>
  Arr *a = arr_new();

  void add (char *id, int v) {
    arr_push(a, token_new_string(0, id));
    arr_push(a, token_new_int(0, v));
  }

  add("year", tm->tm_year + 1900);
  add("month", tm->tm_mon + 1);
  add("day", tm->tm_mday);
  add("hour", tm->tm_hour);
  add("minute", tm->tm_min);
  add("second", tm->tm_sec);

  machine_push(m, token_new_list(0, a));
}

static void tadd (Machine *m) {
  Int n = tk_pop_int(m);
  time_t t = tk_pop_int(m);

  machine_push(m, token_new_int(0, t + n));
}

static void df (Machine *m) {
  time_t t2 = tk_pop_int(m);
  time_t t1 = tk_pop_int(m);

  machine_push(m, token_new_int(0, t1 - t2));
}

static void addd (Machine *m) {
  Int n = tk_pop_int(m);
  time_t t = tk_pop_int(m);

  machine_push(m, token_new_int(0, t + n * 86400));
}

static void dfd (Machine *m) {
  time_t t2 = tk_pop_int(m);
  time_t t1 = tk_pop_int(m);

  double tmp = ((double)t1 - t2) / 86400.;
  Int r  = (Int)(tmp >= 0 ? tmp + 0.5 : tmp - 0.5);
  machine_push(m, token_new_int(0, r));
}

Pmodule *modtime_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("new", new); // [INT, INT, INT, INT, INT, INT] - INT
                          // [year, month, day, hour, min, sec] - date
                          // Note: hour is without daylight saving
  add("newDate", newdate); // [INT, INT, INT] - INT
                                  // [year, month, day] - date
  add("fromStr", fromstr); // [STRING, STRING] - OPT<INT>
                                  // [template, date] - option of date
  add("fromDate", fromdate); // STRING - INT
  add("toDate", todate); // INT - STRING
  add("toTime", totime); // INT - STRING
  add("toDateTime", todatetime); // INT - STRING
  add("format", format); // [INT - STRING] - STRING
                               // [date, template] - date
  add("now", now); // [] - INT
  add("broke", broke); // INT - OBJ
  add("add", tadd); // [INT - INT] - INT  - In seconds (date + secs.)
  add("df", df); // [INT - INT] - INT  - In seconds (date - date) = secs.
  add("addDays", addd); // [INT - INT] - INT (date + days)
  add("dfDays", dfd); // [INT - INT] - INT (date - date) = days

  return r;
}





