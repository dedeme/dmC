// Copyright 06-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "volumes.h"
#include "kut/DEFS.h"
#include "kut/arr.h"
#include "kut/path.h"
#include "kut/file.h"
#include "kut/math.h"

Exp *libmkt_volumes_read (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  Exp *dpath_e = aparams[0];
  Exp *samples_e = aparams[1];
  Exp *cos_e = aparams[2];

  char *dpath = exp_get_string(dpath_e);
  int samples = exp_get_int(samples_e);
  // <char>
  Arr *cos = arr_map(exp_get_array(cos_e), (FMAP)exp_get_string);

  int ncos = arr_size(cos);
  // <Exp>
  Arr *vols = arr_new();
  for (int ico = 0; ico < ncos; ++ico) {
    char *c = arr_get(cos, ico);
    char *fpath = path_cat(dpath, str_f("%s.tb", c), NULL);
    //<char>
    Arr *qs = str_csplit_trim(str_trim(file_read(fpath)), '\n');

    int nsamples = 0;
    double sum = 0;
    EACH(qs, char, l) {
      //<char>
      Arr *ps = str_csplit(l, ':');
      double mx = math_stod(arr_get(ps, 3));
      double mn = math_stod(arr_get(ps, 4));
      double v = math_stod(arr_get(ps, 5));
      if (mx <= 0 || mn <= 0 || v < 0) continue;

      sum += (mx + mn) * v;
      nsamples += 1;
      if (nsamples >= samples) break;
    }_EACH

    arr_push(vols, exp_float(sum / (samples * 2)));
  }

  return exp_array(vols);
}
