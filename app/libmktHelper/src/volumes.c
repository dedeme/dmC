// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "volumes.h"
#include "kut/DEFS.h"
#include "kut/file.h"
#include "kut/path.h"
#include "kut/js.h"
#include "kut/math.h"
#include "kut/rs.h"
#include "cts.h"

char *volumes_read(Map *rq) {
  char *dpath = js_rs(opt_eget(map_get(rq, "path")));
  int samples = js_ri(opt_eget(map_get(rq, "samples")));
  // <char>
  Arr *cos = arr_map(js_ra(opt_eget(map_get(rq, "Cos"))), (FMAP)js_rs);
  // <char>
  Arr *vols = arr_new();

  EACH(cos, char, c) {
    TRY {
      char *fpath = path_cat(dpath, str_f("%s.tb", c), NULL);
      char *qs_str = str_trim(file_read(fpath));

      int nsamples = 0;
      double sum = 0;
      EACH(str_csplit(qs_str, '\n'), char, l) {
        // <char>
        Arr *ps = str_csplit(l, ':');
        double mx = math_stod(arr_get(ps, 3));
        double mn = math_stod(arr_get(ps, 4));
        double v = math_stoi(arr_get(ps, 5));
        if (mx <= 0 || mn <= 0 || v < 0) continue;
        v = (mx + mn) * v;

        sum += v;
        ++nsamples;
        if (nsamples >= samples) break;
      }_EACH

      if (nsamples == samples) (arr_push(vols, js_wf(sum / (samples * 2), 9)));
      else arr_push(vols, js_wf(0.0, 9));
    } CATCH (e) {
      e = e;
    }_TRY
  }_EACH

  return js_wo(map_from_array(arr_new_from(
    kv_new("Vols", js_wa(vols)),
    NULL
  )));
}
