// Copyright 05-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "quotes.h"
#include "kut/DEFS.h"
#include "kut/arr.h"
#include "kut/path.h"
#include "kut/file.h"
#include "kut/math.h"
#include "cquotes.h"
#include "DEFS.h"

static Exp *quotes_to_exp(Quotes *this) {
  // <Exp>
  Arr *cos = arr_new();
  for (int i = 0; i < this->ncos; ++i) arr_push(cos, exp_string(this->cos[i]));
  // <Exp>
  Arr *dates = arr_new();
  for (int i = 0; i < libmkt_HISTORIC_QUOTES; ++i)
    arr_push(dates, exp_string(this->dates[i]));
  // <Exp>
  Arr *opens = arr_new();
  // <Exp>
  Arr *closes = arr_new();
  // <Exp>
  Arr *maxs = arr_new();
  // <Exp>
  Arr *mins = arr_new();
  for (int i = 0; i < libmkt_HISTORIC_QUOTES; ++i) {
    // <Exp>
    Arr *orow = arr_new();
    // <Exp>
    Arr *crow = arr_new();
    // <Exp>
    Arr *xrow = arr_new();
    // <Exp>
    Arr *nrow = arr_new();
    for (int j = 0; j < this->ncos; ++j) {
      arr_push(orow, exp_float(this->opens[i][j]));
      arr_push(crow, exp_float(this->closes[i][j]));
      arr_push(xrow, exp_float(this->maxs[i][j]));
      arr_push(nrow, exp_float(this->mins[i][j]));
    }
    arr_push(opens, exp_array(orow));
    arr_push(closes, exp_array(crow));
    arr_push(maxs, exp_array(xrow));
    arr_push(mins, exp_array(nrow));
  }

  return exp_array(arr_new_from(
    exp_array(cos),
    exp_array(dates),
    exp_array(opens),
    exp_array(closes),
    exp_array(maxs),
    exp_array(mins),
    NULL
  ));
}

Exp *libmkt_quotes_from_cquotes(Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  return quotes_to_exp(exp_get_object("cquotes", aparams[0]));
}

Exp *libmkt_quotes_read (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  char *dpath = exp_get_string(aparams[0]);
  // <char>
  Arr *cos = arr_map(exp_get_array(aparams[1]), (FMAP)exp_get_string);

  return quotes_to_exp(
    cquotes_read(dpath, arr_size(cos), (char **)arr_begin(cos))
  );
}

