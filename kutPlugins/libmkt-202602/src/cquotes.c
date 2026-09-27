// Copyright 05-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "cquotes.h"
#include "kut/DEFS.h"
#include "kut/arr.h"
#include "kut/path.h"
#include "kut/file.h"
#include "kut/math.h"
#include "DEFS.h"

static double **mk_matrix (int n_cos) {
  Arr *mx = arr_new_bf(libmkt_HISTORIC_QUOTES);
  for (int i = 0; i < libmkt_HISTORIC_QUOTES; ++i) {
    double *row = ATOMIC(sizeof(double) * n_cos);
    for (int j = 0; j < n_cos; ++j) row[j] = -1;
    arr_push(mx, row);
  }
  return (double **)arr_begin(mx);
}

Quotes *quotes_new(int n_cos) {
  Quotes *this = MALLOC(Quotes);
  Arr *cos = arr_new_fill("", n_cos);
  Arr *dates = arr_new_fill("", libmkt_HISTORIC_QUOTES);
  this->ncos = n_cos;
  this->cos = (char **)arr_begin(cos);
  this->dates = (char **)arr_begin(dates);
  this->opens = mk_matrix(n_cos);
  this->closes = mk_matrix(n_cos);
  this->maxs = mk_matrix(n_cos);
  this->mins = mk_matrix(n_cos);
  return this;
}

Quotes *cquotes_read (char *dpath, int ncos, char **cos) {
  Quotes *r = quotes_new(ncos);
  r->cos = cos;

  for (int ico = 0; ico < ncos; ++ico) {
    char *co = cos[ico];
    char *fpath = path_cat(dpath, str_f("%s.tb", co), NULL);

    // <char>
    Arr *qs = str_csplit_trim(str_trim(file_read(fpath)), '\n');
    if (arr_size(qs) != libmkt_HISTORIC_QUOTES)
      EXC_KUT(str_f(
        "Dates of %s(%d) are different from %d",
        co, arr_size(qs), libmkt_HISTORIC_QUOTES
      ));
    arr_reverse(qs);

    if (ico == 0) { // readDates
      char **dates = r->dates;
      for (int i = 0; i < libmkt_HISTORIC_QUOTES; ++i) {
        char *qdate = str_left(arr_get(qs, i), 8);
        if (!math_digits(qdate))
          EXC_KUT(str_f("'%s'. Bad date in %s", qdate, co));
        dates[i] = qdate;
      }
    }

    int nqtypes = 4;
    double **qqs[nqtypes];
    qqs[0] = r->opens;
    qqs[1] = r->closes;
    qqs[2] = r->maxs;
    qqs[3] = r->mins;

    for (int idate = 0; idate < libmkt_HISTORIC_QUOTES; ++idate) {
      char *qstr = arr_get(qs, idate);
      //<char>
      Arr *es = str_csplit_trim(qstr, ':');
      if (arr_size(es) != 7)
        EXC_KUT(str_f("Quote %s of %s has not 7 fields.", qstr, co));

      for (int ifield = 0; ifield < nqtypes; ++ifield) {
        double **qq = qqs[ifield];
        char *q = arr_get(es, ifield + 1);
        double n;
        TRY {
          n = math_round(math_stod(q), 4);
        } CATCH (e) {
          EXC_KUT(str_f("Bad quote %s (%s) in %s of %s.", q, e, qstr, co));
        }_TRY
        if (n < 0.0 && idate > 0) {
          if (qq[idate - 1][ico] >= 0.0)
            qq[idate][ico] = qq[idate - 1][ico];
          else
            qq[idate][ico] = -1.0;
        } else {
          if (idate > 0 && qq[idate - 1][ico] < 0.0) {
            for (int i = idate; i >= 0; --i)
              qq[i][ico] = n;
          } else {
            qq[idate][ico] = n;
          }
        }
      }
    }
  }
  return r;
}

Exp *libmkt_cquotes_read (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  char *dpath = exp_get_string(aparams[0]);
  // <char>
  Arr *cos = arr_map(exp_get_array(aparams[1]), (FMAP)exp_get_string);

  return exp_object(
    "cquotes",
    cquotes_read(dpath, arr_size(cos), (char **)arr_begin(cos))
  );
}

Exp *libmkt_cquotes_company_index (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  Quotes *qts = exp_get_object("cquotes", aparams[0]);
  char *nick = exp_get_string(aparams[1]);
  char **cos = qts->cos;
  for (int i = 0; i < qts->ncos; ++i) if (!strcmp(nick, cos[i]))
    return exp_int(i);
  return exp_int(-1);
}

/// Extracts data of the company with index 'coIx' to be used in 'strategy'.
/// Returns Exp<<cquotes>>
///   params: Arr<<cquotes>, s>. Fields are:
///             cqts: C quotes.
///             co_ix: Index in cqts of the company to extract.
Exp *libmkt_cquotes_get_single (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  Quotes *qts = exp_get_object("cquotes", aparams[0]);
  int ix = exp_get_int(aparams[1]);
  EXC_RANGE(ix, 0, qts->ncos - 1);

  Quotes *r = quotes_new(1);
  r->cos[0] = qts->cos[ix];
  char **rdates = r->dates;
  char **qdates = qts->dates;
  double **ropens = r->opens;
  double **qopens = qts->opens;
  double **rcloses = r->closes;
  double **qcloses = qts->closes;
  double **rmaxs = r->maxs;
  double **qmaxs = qts->maxs;
  double **rmins = r->mins;
  double **qmins = qts->mins;
  for (int idate = 0; idate < libmkt_HISTORIC_QUOTES; ++idate) {
    rdates[idate] = qdates[idate];
    ropens[idate][0] = qopens[idate][ix];
    rcloses[idate][0] = qcloses[idate][ix];
    rmaxs[idate][0] = qmaxs[idate][ix];
    rmins[idate][0] = qmins[idate][ix];
  }

  return exp_object("cquotes", r);
}
