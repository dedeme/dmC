static char *_time_fmt (int64_t tm, char *template) {
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
static int64_t time_add_days (int64_t this, int64_t days) {
  return this + days * 86400000;
}
static int64_t time_day (int64_t this) {
  time_t t = time_to_time(this);
  return localtime(&t)->tm_mday;
}
static int time_day_light_correction (int day, int month, int year) {
  struct tm t;
  memset(&t, 0, sizeof(struct tm));
  t.tm_year = year - 1900;
  t.tm_mon = month;
  t.tm_mday = day;
  t.tm_hour = 12;
  time_t t2 = mktime(&t);
  int h = localtime(&t2)->tm_hour;
  return 12 - h;
}
static int64_t time_df_day (int64_t this, int64_t other) {
  return (this / 86400000) - (other / 86400000);
}
static int time_eq_day (int64_t this, int64_t other) {
  return time_df_day(this, other) ? 0 : 1;
}
static char *time_fmt (int64_t this, char *template) {
  int len = strlen(template);
  int p = 0;
  Buf *r = buf_new();
  for (;;) {
    int i = str_cindex_from(template, '%', p);
    if (i == -1) break;
    buf_add(r, str_sub(template, p, i));
    ++i;
    if (i == len)
      texc_throw("time.fmt", str_f("Bad '%' sequence at end in %s", template));
    char ch = template[i];
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
          buf_add(r, str_ltrim(_time_fmt(this, "%e")));
          break;
        case 'D':
          buf_add(r, _time_fmt(this, "%d"));
          break;
        case 'm': {
          char *v = _time_fmt(this, "%m");
          if (*v == '0') v = v + 1;
          buf_add(r, v);
          break;
          }
        case 'M':
          buf_add(r, _time_fmt(this, "%m"));
          break;
        case 'y':
          buf_add(r, _time_fmt(this, "%y"));
          break;
        case 'Y':
          buf_add(r, _time_fmt(this, "%Y"));
          break;
        case 't':
          buf_add(r, _time_fmt(this, "%T"));
          break;
        default: { // 'T'
          char *millis = str_f("%ld", this % 1000);
          buf_add(r, str_f("%s.%s", _time_fmt(this, "%T"), millis));
        }
      }
      p = i + 1;
      continue;
    }
    texc_throw("time.fmt", str_f("Bad '%' sequence in %s", template));
  }
  buf_add(r, str_right(template, p));
  return str_new(buf_str(r));
}
static Arri *time_from_clock (int64_t this, char *tms) {
  int to_time_number (char *n, int max) {
    if (strlen(n) != 2) return -1;
    Arri *nnOp = math_stoi(n);
    if (!nnOp) return -1;
    int64_t nn = *tarr_beginI(nnOp);
    if (nn >= max) return -1;
    return nn;
  }
  Arrs *ps = str_csplit(tms, ':');
  if (tarr_sizeS(ps) == 3) {
    int h = to_time_number(tarr_getS(ps, 0), 24);
    if (h != 1) {
      int m = to_time_number(tarr_getS(ps, 1), 60);
      if (m != -1) {
        int s = to_time_number(tarr_getS(ps, 2), 60);
        if (s != -1) {
          int64_t r = time_new_time(
            time_day(this), time_month(this), time_year(this),
            h, m, s
          );
          return tarr_newi(r, 1);
        }
      }
    }
  }
  return tarr_new_i();
}
static Arri *time_from_en (char *date, char *sep) {
  return time_from_iso_en_aux(0, date, *sep);
}
static Arri *time_from_iso (char *date, char *sep) {
  return time_from_iso_en_aux(1, date, *sep);
}
static Arri *time_from_iso_en_aux (int is_iso, char *date, char sep) {
  Arrs *parts = str_csplit(date, sep);
  if (tarr_sizeS(parts) == 3) {
    char *d = tarr_getS(parts, (is_iso) ? 0 : 1);
    char *m = tarr_getS(parts, (is_iso) ? 1 : 0);
    char *y = tarr_getS(parts, 2);

    if (strlen(d) == 1) d = str_f("%s%s", "0", d);
    if (strlen(m) == 1) m = str_f("%s%s", "0", m);
    if (strlen(y) == 2) y = str_f("%s%s", "20", y);

    if (strlen(d) == 2 && strlen(m) == 2 && strlen(y) == 4)
      return time_from_str(str_f("%s%s%s", y, m, d));
  }
  return tarr_new_i();
}
static Arri *time_from_str(char *date) {
  if (strlen(date) != 8 && !math_digits(date)) return tarr_new_i();
  char tpl[5];
  memset (tpl, 0, 5);
  memcpy(tpl, date + 6, 2);
  int d = atoi(tpl);
  memcpy(tpl, date + 4, 2);
  int m = atoi(tpl);
  memcpy(tpl, date, 4);
  int y = atoi(tpl);
  return tarr_newi(time_new(d, m, y), 1);
}
static int64_t time_from_time (time_t tm) {
  return (int64_t)tm * 1000;
}
static int64_t time_hour (int64_t this) {
  time_t t = time_to_time(this);
  return localtime(&t)->tm_hour;
}
static int64_t time_millisecond (int64_t this) {
  return this % 1000;
}
static int64_t time_minute (int64_t this) {
  time_t t = time_to_time(this);
  return localtime(&t)->tm_min;
}
static int64_t time_month (int64_t this) {
  time_t t = time_to_time(this);
  return localtime(&t)->tm_mon + 1;
}
static int64_t time_new (int day, int month, int year) {
  struct tm t;
  memset(&t, 0, sizeof(struct tm));
  t.tm_year = year - 1900;
  t.tm_mon = month - 1;
  t.tm_mday = day;
  t.tm_hour = 12 + time_day_light_correction(day, month, year);

  return time_from_time((time_t) mktime(&t));
}
static int64_t time_new_time (
  int day, int month, int year, int hour, int minute, int second
) {
  struct tm t;
  memset(&t, 0, sizeof(struct tm));
  t.tm_year = year - 1900;
  t.tm_mon = month - 1;
  t.tm_mday = day;
  t.tm_hour = hour + time_day_light_correction(day, month, year);
  t.tm_min = minute;
  t.tm_sec = second;

  return time_from_time((time_t) mktime(&t));
}
static int64_t time_now(void) {
  struct timeval tm;
  gettimeofday(&tm, NULL);
  return tm.tv_sec * 1000 + tm.tv_usec / 1000;
}
static int64_t time_second (int64_t this) {
  time_t t = time_to_time(this);
  return localtime(&t)->tm_sec;
}
static char *time_to_en (int64_t this) {
  return _time_fmt(this, "%m-%d-%Y");
}
static char *time_to_iso (int64_t this) {
  return _time_fmt(this, "%d/%m/%Y");
}
static char *time_to_str (int64_t this) {
  return _time_fmt(this, "%Y%m%d");
}
static time_t time_to_time (int64_t this) {
  return (time_t)(this / 1000);
}
static int64_t time_weekday (int64_t this) {
  time_t t = time_to_time(this);
  return localtime(&t)->tm_wday;
}
static int64_t time_year (int64_t this) {
  time_t t = time_to_time(this);
  return localtime(&t)->tm_year + 1900;
}
static int64_t time_year_day (int64_t this) {
  time_t t = time_to_time(this);
  return localtime(&t)->tm_yday + 1;
}
