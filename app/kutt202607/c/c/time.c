
// time ---------------------------

// i, i -> i
Val __time_addDays (Val tm, Val days) {
  return (Val)time_add(tm.i, (int)days.i);
}

// i -> i
Val __time_day (Val tm) {
  return (Val)(int64_t)time_day(tm.i);
}

// i, i -> i
Val __time_dfDays (Val tm1, Val tm2) {
  return (Val)(int64_t)time_df(tm1.i, tm2.i);
}

// i, i -> b
Val __time_eqDay (Val tm1, Val tm2) {
  return (Val)time_eq(tm1.i, tm2.i);
}

// i, s -> s
Val __time_fmt (Val tm_val, Val template) {
  char *time_fmt (int64_t tm, char *template) {
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

  int64_t tm = tm_val.i;
  char *s = template.s;

  int len = strlen(s);
  int p = 0;
  Buf *r = buf_new();
  for (;;) {
    int i = str_cindex_from(s, '%', p);
    if (i == -1) break;

    buf_add(r, str_sub(s, p, i));
    ++i;
    if (i == len)
      ___built_throw(
        "built-in(time.fmt):0",
        (Val)"Bad '%' sequence at end of string"
      );

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

    ___built_throw("built-in(time.fmt):0", (Val)"Bad '%' sequence");
  }
  buf_add(r, str_right(s, p));
  return (Val)str_new(buf_str(r));
}

// i, s -> [i]
Val __time_fromClock (Val tm, Val template) {
  int to_time_number (char *n, int max) {
    if (strlen(n) != 2) return -1;
    if (!math_digits(n)) return -1;
    int nn = math_stoi(n);
    if (nn >= max) return -1;
    return nn;
  }

  char *tms = template.s;
  // <char>
  Arr *ps = str_csplit(tms, ':');
  if (arr_size(ps) == 3) {
    int h = to_time_number(arr_get(ps, 0), 24);
    if (h != 1) {
      int m = to_time_number(arr_get(ps, 1), 60);
      if (m != -1) {
        int s = to_time_number(arr_get(ps, 2), 60);
        if (s != -1) {
          return __rs_some(__time_new(
            __time_day(tm), __time_month(tm), __time_year(tm),
            (Val)(int64_t)h, (Val)(int64_t)m, (Val)(int64_t)s
          ));
        }
      }
    }
  }
  return __arr_new();
}

Val __time_from_str_op_aux(char *date) {
  if (strlen(date) != 8 && !math_digits(date)) return __arr_new();
  char tpl[5];
  memset (tpl, 0, 5);
  memcpy(tpl, date + 6, 2);
  int d = atoi(tpl);
  memcpy(tpl, date + 4, 2);
  int m = atoi(tpl);
  memcpy(tpl, date, 4);
  int y = atoi(tpl);
  return __rs_some(__time_newDate(
    (Val)(int64_t)d, (Val)(int64_t)m, (Val)(int64_t)y
  ));
}
Val __time_from_iso_en_op (int is_iso, char *date, char sep) {
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
      return __time_from_str_op_aux(str_f("%s%s%s", y, m, d));
  }
  return __arr_new();
}
// s, s -> [i]
Val __time_fromEn (Val s, Val sep) {
  return __time_from_iso_en_op(FALSE, s.s, *sep.s);
}

// s, s -> [i]
Val __time_fromIso (Val s, Val sep) {
  return __time_from_iso_en_op(TRUE, s.s, *sep.s);
}

// s -> [i]
Val __time_fromStr (Val s) {
  return __time_from_str_op_aux(s.s);
}

// i -> i
Val __time_hour (Val tm) {
  return (Val)(int64_t)time_hour(tm.i);
}

// i -> i
Val __time_millisecond (Val tm) {
  return (Val)(int64_t)time_millisecond(tm.i);
}

// i -> i
Val __time_minute (Val tm) {
  return (Val)(int64_t)time_minute(tm.i);
}

// i -> i
Val __time_month (Val tm) {
  return (Val)(int64_t)(time_month(tm.i) + 1);
}

// i, i, i, i, i, i -> i
Val __time_new (Val day, Val month, Val year, Val hour, Val minute, Val second) {
  return (Val)time_new_time(
    (int)day.i, (int)(month.i - 1), (int)year.i,
    (int)hour.i, (int)minute.i, (int)second.i
  );
}

// i, i, i -> i
Val __time_newDate (Val day, Val month, Val year) {
  return (Val)time_new((int)day.i, (int)(month.i - 1), (int)year.i);
}

// -> i
Val __time_now (void) {
  return (Val)time_now();
}

// i -> i
Val __time_second (Val tm) {
  return (Val)(int64_t)time_second(tm.i);
}

// i -> s
Val __time_toEn (Val tm) {
  return (Val)time_to_us(tm.i);
}

// i -> s
Val __time_toIso (Val tm) {
    return (Val)time_to_iso(tm.i);
}

// i -> s
Val __time_toStr (Val tm) {
    return (Val)time_to_str(tm.i);
}

// i -> i
Val __time_weekday (Val tm) {
  return (Val)(int64_t)time_week_day(tm.i);
}

// i -> i
Val __time_year (Val tm) {
  return (Val)(int64_t)time_year(tm.i);
}

// i -> i
Val __time_yearDay (Val tm) {
  return (Val)(int64_t)(time_year_day(tm.i) + 1);
}
