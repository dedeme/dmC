// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "quotesReader.h"
#include "kut/DEFS.h"
#include "kut/file.h"
#include "kut/path.h"
#include "kut/js.h"
#include "kut/math.h"
#include "kut/rs.h"
#include "cts.h"

// <char>
static Arr *read_dates(char *co, char *qs) {
  // <char>
  Arr *r = arr_new();
  EACH(str_split_trim(qs, "\n"), char, q) {
    char *qdate = str_left(q, 8);
    if (!math_digits(qdate))
      EXC_ILLEGAL_ARGUMENT(str_f("Bad date in %s", co), "A valid date", qdate);
    arr_push(r, qdate);
  }_EACH
  arr_reverse(r);
  return r;
}

static char *check_quote(char *co, char *q) {
  if (!strcmp(q, "-1")) return q;
  // <char>
  Arr *a = str_csplit(q, '.');
  int sz = arr_size(a);
  if (sz == 1 && math_digits(arr_get(a, 0))) return q;
  if (sz == 2 &&
    math_digits(arr_get(a, 0)) &&
    math_digits(arr_get(a, 1))
  ) return q;

  EXC_ILLEGAL_ARGUMENT(str_f("Bad quote in %s", co), "A valid quote", q);
  return q; // Unreachable
}

// 'errs' is Arr<char>.
// Return Rs<Arr<Arr<char>>> with quotes o-c-mx-mn normalized.
static Rs *read_quotes (int nquotes, char *co, char *qs) {
  // <Arr<char>>
  Arr *r = arr_new();
  // <char>
  Arr *okqs = arr_new();
  REPEAT(nquotes) {
    arr_push(r, arr_new());
    arr_push(okqs, "-1");
  }_REPEAT

  // <char>
  Arr *q_strs = str_split_trim(qs, "\n");
  arr_reverse(q_strs);
  EACH(q_strs, char, q) {
    //<char>
    Arr *parts = str_split_trim(q, ":");
    RANGE0(i, nquotes) {
      char *q_str = check_quote(co, arr_get(parts, 1 + i));
      char *okq = arr_get(okqs, i);
      if (*okq == '-') {
        if (*q_str != '-') {
          arr_set(okqs, i, q_str);
          for (int j = _i - 1; j >= 0; --j) {
            if (*((char *)arr_get(arr_get(r, i), j)) == '-')
              arr_set(arr_get(r, i), j, q_str);
            else
              break;
          }
        }
      } else if (*q_str == '-') q_str = okq;
      else arr_set(okqs, i, q_str);

      arr_push(arr_get(r, i), q_str);
    }_RANGE
  }_EACH

  EACH(okqs, char, q) {
    if (*q == '-')
      return rs_fail(str_f(
        "Every %s of %s is '-1'",
        arr_get(arr_new_from("open", "close", "max", "min", NULL), _i), co
      ));
  }_EACH

  return rs_ok(r);
}

char *quotesReader_run(Map *rq) {
  char *dpath = js_rs(opt_eget(map_get(rq, "path")));
  int nquotes = js_ri(opt_eget(map_get(rq, "nquotes")));
  if (nquotes != 2) nquotes = 4;
  // <char>
  Arr *cos = arr_map(js_ra(opt_eget(map_get(rq, "Cos"))), (FMAP)js_rs);
  // <char>
  Arr *dates = arr_new();
  // <char>
  Arr *new_cos = arr_new();
  // <Arr<Arr<char>>
  Arr *ocmm = arr_new();
  REPEAT(nquotes) {
    arr_push(ocmm, arr_new());
  }_REPEAT
  EACH(cos, char, c) {
    char *fpath = path_cat(dpath, str_f("%s.tb", c), NULL);
    char *qs_str = str_trim(file_read(fpath));

    if (_i == 0) dates = read_dates(c, qs_str);
    if (arr_size(dates) != cts_historic_quotes)
      EXC_ILLEGAL_STATE(str_f("Dates of %s != %d", c, cts_historic_quotes));

    // <Arr<Arr<char>>>
    Rs *qsRs = read_quotes(nquotes, c, qs_str);

    // <Arr<char>>
    Arr *qs = rs_get(qsRs);
    RANGE0(i, nquotes) {
      arr_push(arr_get(ocmm, i), arr_get(qs, i));
    }_RANGE

    arr_push(new_cos, c);
  }_EACH

  // <Arr<JS>>
  Arr *all_quotes = arr_new();
  REPEAT(nquotes) {
    arr_push(all_quotes, arr_new());
  }_REPEAT

  RANGE0(row, arr_size(dates)) {
    RANGE0(i, nquotes) {
      Arr *arow = arr_new();
      RANGE0(ico, arr_size(new_cos)) {
        arr_push(arow, arr_get(arr_get(arr_get(ocmm, i), ico), row));
      }_RANGE
      arr_push(arr_get(all_quotes, i), js_wa(arow));
    }_RANGE
  }_RANGE

  //<kv<char, char>>
  Arr *rs = arr_new_from(
    kv_new("Cos", js_wa(arr_map(new_cos, (FMAP)js_ws))),
    kv_new("Dates", js_wa(arr_map(dates, (FMAP)js_ws))),
    kv_new("Opens", js_wa(arr_get(all_quotes, 0))),
    kv_new("Closes", js_wa(arr_get(all_quotes, 1))),
    NULL
  );
  if (nquotes == 4) {
    arr_push(rs, kv_new("Maxs", js_wa(arr_get(all_quotes, 2))));
    arr_push(rs, kv_new("Mins", js_wa(arr_get(all_quotes, 3))));
  }

  return js_wo(map_from_array(rs));
}
