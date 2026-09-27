// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "mods/md_time.h"
#include <time.h>
#include "kut/time.h"
#include "kut/buf.h"
#include "kut/math.h"
#include "exp.h"
#include "runner/fail.h"

static Exp * from_str_op_aux(char *date) {
  if (strlen(date) != 8 && !math_digits(date)) return exp_array(arr_new());
  char tpl[5];
  memset (tpl, 0, 5);
  memcpy(tpl, date + 6, 2);
  int d = atoi(tpl);
  memcpy(tpl, date + 4, 2);
  int m = atoi(tpl);
  memcpy(tpl, date, 4);
  int y = atoi(tpl);
  return exp_array(arr_new_from(exp_int(time_new(d, m - 1, y)), NULL));
}

static Exp *from_iso_en_op (int is_iso, char *date, char sep) {
  // <char>
  Arr *parts = str_csplit(date, sep);
  if (arr_size(parts) == 3) {
    char *d = arr_get(parts, (is_iso) ? 0 : 1);
    char *m = arr_get(parts, (is_iso) ? 1 : 0);
    char *y = arr_get(parts, 2);

    if (strlen(d) == 1) d = str_f("%s%s", "0", d);
    if (strlen(m) == 1) m = str_f("%s%s", "0", m);
    if (strlen(y) == 2) y = str_f("%s%s", "20", y);

    if (strlen(d) == 2 && strlen(m) == 2 && strlen(y) == 4)
      return from_str_op_aux(str_f("%s%s%s", y, m, d));
  }
  return exp_array(arr_new());
}

static char *time_fmt (int64_t tm, char *template) {
  char *s, *rs;
  time_t tthis = (time_t)(tm / 1000);
  struct tm *t = localtime(&tthis);
  int size = 126;
  while (1) {
    rs = (char *)calloc(size, 1); // calloc ok
    if (strftime (rs, size, template, t)) {
      s = str_new(rs);
      free(rs); // free ok
      break;
    }
    free(rs); // free ok
    size += size;
  }
  return s;
}

// if fail, returns -1. 'max' is exclusive.
static int to_time_number (char *n, int max) {
  if (strlen(n) != 2) return -1;
  if (!math_digits(n)) return -1;
  int nn = math_stoi(n);
  if (nn >= max) return -1;
  return nn;
}

// -----------------------------------------------------------------------------

// \i, i -> i
static Exp *add_days (Arr *exps) {
  CHECK_PARS ("time.addDays", 2, exps);
  return exp_int(time_add(
    exp_get_int(arr_get(exps, 0)),
    exp_get_int(arr_get(exps, 1))
  ));
}

// \i -> i
static Exp *day (Arr *exps) {
  CHECK_PARS ("time.day", 1, exps);
  return exp_int(time_day(exp_get_int(arr_get(exps, 0))));
}

// \i, i -> i
static Exp *df_days (Arr *exps) {
  CHECK_PARS ("time.dfDays", 2, exps);
  return exp_int(time_df(
    exp_get_int(arr_get(exps, 0)),
    exp_get_int(arr_get(exps, 1))
  ));
}

// \i, i -> b
static Exp *eq_day (Arr *exps) {
  CHECK_PARS ("time.eqDay", 2, exps);
  return exp_bool(time_eq(
    exp_get_int(arr_get(exps, 0)),
    exp_get_int(arr_get(exps, 1))
  ));
}

// \i, s -> [i]|[]
static Exp *from_clock (Arr *exps) {
  CHECK_PARS ("time.fromClock", 2, exps);
  int64_t tm = exp_get_int(arr_get(exps, 0));
  char *tms = exp_get_string(arr_get(exps, 1));
  // <char>
  Arr *ps = str_csplit(tms, ':');
  if (arr_size(ps) == 3) {
    int h = to_time_number(arr_get(ps, 0), 24);
    if (h != 1) {
      int m = to_time_number(arr_get(ps, 1), 60);
      if (m != -1) {
        int s = to_time_number(arr_get(ps, 2), 60);
        if (s != -1) {
          Exp *r = exp_int(time_new_time(
            time_day(tm), time_month(tm), time_year(tm),
            h, m, s
          ));
          return exp_array(arr_new_from(r, NULL));
        }
      }
    }
  }
  return exp_array(arr_new());
}

// \s, s -> [i]|[]
static Exp *from_en (Arr *exps) {
  CHECK_PARS ("time.fromEn", 2, exps);
  return from_iso_en_op(
    FALSE,
    exp_get_string(arr_get(exps, 0)),
    *exp_get_string(arr_get(exps, 1))
  );
}

// \s, s -> [i]|[]
static Exp *from_iso (Arr *exps) {
  CHECK_PARS ("time.fromIso", 2, exps);
  return from_iso_en_op(
    TRUE,
    exp_get_string(arr_get(exps, 0)),
    *exp_get_string(arr_get(exps, 1))
  );
}

// \s -> [i]|[]
static Exp *from_str (Arr *exps) {
  CHECK_PARS ("time.fromStr", 1, exps);
  return from_str_op_aux(exp_get_string(arr_get(exps, 0)));
}

// \s, i -> s
static Exp *fmt (Arr *exps) {
  CHECK_PARS ("time.fmt", 2, exps);
  int64_t tm = exp_get_int(arr_get(exps, 0));
  char *s = exp_get_string(arr_get(exps, 1));

  int len = strlen(s);
  int p = 0;
  Buf *r = buf_new();
  for (;;) {
    int i = str_cindex_from(s, '%', p);
    if (i == -1) break;

    buf_add(r, str_sub(s, p, i));
    ++i;
    if (i == len)
      EXC_KUT("Bad '%' sequence at end of string");

    char ch = s[i];
    if (ch == '%') {
      buf_cadd(r, ch);
      p = i + 1;
      continue;
    }

    if (
      ch == 'd' || ch == 'D' || ch == 'm' || ch == 'M' || ch == 'y' ||
      ch == 'Y' || ch == 't' || ch == 'T'
    ) {
      switch (ch) {
        case 'd' :
          buf_add(r, str_ltrim(time_fmt(tm, "%e")));
          break;
        case 'D':
          buf_add(r, time_fmt(tm, "%d"));
          break;
        case 'm': {
          char *v = time_fmt(tm, "%m");
          if (*v == '0') v = v + 1;
          buf_add(r, v);
          break;
          }
        case 'M':
          buf_add(r, time_fmt(tm, "%m"));
          break;
        case 'y':
          buf_add(r, time_fmt(tm, "%y"));
          break;
        case 'Y':
          buf_add(r, time_fmt(tm, "%Y"));
          break;
        case 't':
          buf_add(r, time_fmt(tm, "%T"));
          break;
        default: { // 'T'
          char *millis = str_f("%ld", tm % 1000);
          buf_add(r, str_f("%s.%s", time_fmt(tm, "%T"), millis));
        }
      }

      p = i + 1;
      continue;
    }

    EXC_KUT("Bad '%' sequence");
  }
  buf_add(r, str_right(s, p));
  return exp_string(str_new(buf_str(r)));
}

// \i -> i
static Exp *hour (Arr *exps) {
  CHECK_PARS ("time.hour", 1, exps);
  return exp_int(time_hour(exp_get_int(arr_get(exps, 0))));
}

// \i -> i
static Exp *millisecond (Arr *exps) {
  CHECK_PARS ("time.millisecond", 1, exps);
  return exp_int(time_millisecond(exp_get_int(arr_get(exps, 0))));
}

// \i -> i
static Exp *minute (Arr *exps) {
  CHECK_PARS ("time.minute", 1, exps);
  return exp_int(time_minute(exp_get_int(arr_get(exps, 0))));
}

// \i -> i
static Exp *month (Arr *exps) {
  CHECK_PARS ("time.month", 1, exps);
  return exp_int(time_month(exp_get_int(arr_get(exps, 0))) + 1);
}

// \i, i, i, i, i, i -> i
static Exp *new (Arr *exps) {
  CHECK_PARS ("time.new", 6, exps);
  return exp_int(time_new_time(
    exp_get_int(arr_get(exps, 0)),
    exp_get_int(arr_get(exps, 1)) - 1,
    exp_get_int(arr_get(exps, 2)),
    exp_get_int(arr_get(exps, 3)),
    exp_get_int(arr_get(exps, 4)),
    exp_get_int(arr_get(exps, 5))
  ));
}

// \i, i, i, -> i
static Exp *new_date (Arr *exps) {
  CHECK_PARS ("time.newDate", 3, exps);
  return exp_int(time_new(
    exp_get_int(arr_get(exps, 0)),
    exp_get_int(arr_get(exps, 1)) - 1,
    exp_get_int(arr_get(exps, 2))
  ));
}

// \-> i
static Exp *now (Arr *exps) {
  CHECK_PARS ("time.now", 0, exps);
  return exp_int(time_now());
}

// \i -> i
static Exp *second (Arr *exps) {
  CHECK_PARS ("time.second", 1, exps);
  return exp_int(time_second(exp_get_int(arr_get(exps, 0))));
}

// \i -> s
static Exp *to_en (Arr *exps) {
  CHECK_PARS ("time.toEn", 1, exps);
  return exp_string(time_to_us(exp_get_int(arr_get(exps, 0))));
}

// \i -> s
static Exp *to_iso (Arr *exps) {
  CHECK_PARS ("time.toIso", 1, exps);
  return exp_string(time_to_iso(exp_get_int(arr_get(exps, 0))));
}

// \i -> s
static Exp *to_str (Arr *exps) {
  CHECK_PARS ("time.toStr", 1, exps);
  return exp_string(time_to_str(exp_get_int(arr_get(exps, 0))));
}

// \i -> i
static Exp *weekday (Arr *exps) {
  CHECK_PARS ("time.weekday", 1, exps);
  return exp_int(time_week_day(exp_get_int(arr_get(exps, 0))));
}

// \i -> i
static Exp *year (Arr *exps) {
  CHECK_PARS ("time.year", 1, exps);
  return exp_int(time_year(exp_get_int(arr_get(exps, 0))));
}

// \i -> i
static Exp *year_day (Arr *exps) {
  CHECK_PARS ("time.yearDay", 1, exps);
  return exp_int(time_year_day(exp_get_int(arr_get(exps, 0))) + 1);
}

Bfunction md_time_get (char *fname) {
  if (!strcmp(fname, "addDays")) return add_days;
  if (!strcmp(fname, "day")) return day;
  if (!strcmp(fname, "dfDays")) return df_days;
  if (!strcmp(fname, "eqDay")) return eq_day;
  if (!strcmp(fname, "fromClock")) return from_clock;
  if (!strcmp(fname, "fromEn")) return from_en;
  if (!strcmp(fname, "fromIso")) return from_iso;
  if (!strcmp(fname, "fromStr")) return from_str;
  if (!strcmp(fname, "fmt")) return fmt;
  if (!strcmp(fname, "hour")) return hour;
  if (!strcmp(fname, "millisecond")) return millisecond;
  if (!strcmp(fname, "minute")) return minute;
  if (!strcmp(fname, "month")) return month;
  if (!strcmp(fname, "new")) return new;
  if (!strcmp(fname, "newDate")) return new_date;
  if (!strcmp(fname, "now")) return now;
  if (!strcmp(fname, "second")) return second;
  if (!strcmp(fname, "toEn")) return to_en;
  if (!strcmp(fname, "toIso")) return to_iso;
  if (!strcmp(fname, "toStr")) return to_str;
  if (!strcmp(fname, "weekday")) return weekday;
  if (!strcmp(fname, "year")) return year;
  if (!strcmp(fname, "yearDay")) return year_day;

  EXC_KUT(fail_bfunction("time", fname));
  return NULL; // Unreachable
}
