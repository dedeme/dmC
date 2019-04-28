// Copyright 24-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "process.h"
#include "dmc/date.h"
#include "Rs.h"
#include "io.h"
#include "calcAssets.h"
#include "calcProfits.h"
#include "calcCharts.h"
#include "DEFS.h"

// rss is Arr[Rs]
static void add_old_rss(Model *fmodel, Arr *rss) {
  // Arr[Rs]
  Arr *news = io_rrss_new(model_name(fmodel));
  arr_reverse(news);

  EACH(news, Rs, rs)
    Flea *f = rs_flea(rs);
    arr_insert(rss, 0, calcAssets_new(fmodel, f));
  _EACH

  arr_free(news);
}

int process_run(Model *fmodel) {
  char *date = date_to_str_new(date_now());
  char *mname = model_name(fmodel);
  int nparams = varr_size(model_param_names(fmodel));
  // Arr[Rs]
  Arr *rss = arr_new((FPROC)rs_free);

  REPEAT(FLEAS_PER_GROUP / 2)
      Flea *fnew = flea_new(date, "0", "0", nparams);
      arr_push(rss, rs_new(
        str_new(date), fnew, model_params_new(fmodel, fnew), 0, 0, 0, 0, 0, 0
      ));
  _REPEAT

  int qdays = io_qdays();
  int min_sells = qdays / MIN_SELLS;
  int max_sells = qdays / MAX_SELLS;
  double max_sells_sum = (double)max_sells / 2;
  double max_sells_mul =
    (double)max_sells / (INITIAL_CAPITAL + INITIAL_CAPITAL);

  // Calculate Assets ---------------------------------------------------------

  int max_cycles = INSERTION_CYCLE + CYCLES * nparams;
  RANGE0(cy, max_cycles)
    printf("Model: %s. Cycle: %d / %d\n", mname, cy + 1, max_cycles);

    if (cy == INSERTION_CYCLE || cy == max_cycles - 1) {
      add_old_rss(fmodel, rss);
    }

    char *cyname = str_f_new("%d", cy);
    int nres = arr_size(rss);
    int nresc = 0;
    int c = 0;
    REPEAT(FLEAS_PER_GROUP - nres)
      char *idname = str_f_new("%d", c);
      Flea *fnew = flea_mutate_new(
        rs_flea(arr_get(rss, nresc)), date, cyname, idname
      );
      arr_push(rss, calcAssets_new(fmodel, fnew));

      ++nresc;
      if (nresc == nres) {
        nresc = 0;
      }
      ++c;
      flea_free(fnew);
      free(idname);
    _REPEAT

    double avg = 0;
    do {
      double sum = 0;
      EACH(rss, Rs, rs)
        sum += rs_assets(rs);
      _EACH
      avg = sum / arr_size(rss);

      Varr *rss2 = varr_new();
      EACH(rss, Rs, rs)
        varr_push(rss2, rs);
      _EACH
      int ffilter1(Rs *rs) {
        double assets = rs_assets(rs);
        int sells = rs_sells(rs);
        return assets > avg && sells >= min_sells &&
          sells <= max_sells_sum + assets * max_sells_mul;
      }
      int ffilter2(Rs *rs) {
        return rs_assets(rs) > avg;
      }
      varr_filter(rss2, (FPRED)ffilter1);
      if (varr_size(rss2) == 0) {
        arr_filter(rss, (FPRED)ffilter2);
      } else {
        arr_filter(rss, (FPRED)ffilter1);
      }
      varr_free(rss2);
    } while (arr_size(rss) > FLEAS_PER_GROUP * 0.75);

    double sum = 0;
    EACH(rss, Rs, rs)
      sum += rs_assets(rs);
    _EACH
    avg = sum / arr_size(rss);
    printf("Survivers: %d. Avg: %.2f\n", arr_size(rss), avg);

    free(cyname);
  _RANGE

  // Calculate profits --------------------------------------------------------

  EACH(rss, Rs, rs)
    calcProfits(fmodel, rs);
  _EACH

  double max_assets = 0;
  EACH(rss, Rs, rs)
    double assets = rs_assets(rs);
    if (assets > max_assets) {
      max_assets = assets;
    }
  _EACH
  double cut_assets = max_assets * CUT_PROFITS ;
  Rs *sel = arr_get(rss, 0);
  double sel_value = -1000;
  int size = arr_size(rss);
  int sel_counter = 0;
  EACH_IX(rss, Rs, rs, ix)
    double r = rs_profits_sel(rs);
    if (r > sel_value && rs_assets(rs) > cut_assets) {
      Flea *f = rs_flea(rs);
      printf(
        "Selected Flea (%d/%d): %s-%s-%s -> %.4f * (1 - %.4f) = %.4f\n",
        ix, size, flea_date(f), flea_cycle(f), flea_idn(f),
        rs_profits_avg(rs), rs_profits_mdv(rs),
        rs_profits_avg(rs) * (1 - rs_profits_mdv(rs))
      );
      sel = rs;
      sel_value = r;
      ++sel_counter;
    }
  _EACH

  // Calculate charts ---------------------------------------------------------
  char *chartsData = calcCharts(fmodel, sel);
  io_wcharts(mname, chartsData);
  free(chartsData);

  // Arr[Rs]
  Arr *bests_rss = io_rrss_new(mname);
  // Varr[Rs]
  Varr *bests = varr_new();
  EACH(bests_rss, Rs, rs)
    varr_push(bests, rs);
  _EACH
  if (varr_size(bests) > 0 && str_eq(rs_date(varr_get(bests, 0)), date)) {
    varr_set(bests, 0, sel);
  } else {
    varr_insert(bests, 0, sel);
  }
  io_wrss(1, mname, (Arr *)bests);
  varr_free(bests);
  arr_free(bests_rss);

  io_wrss(0, mname, (Arr *)rss);

  io_clean_rss(mname);

  free(date);
  arr_free(rss);

  return sel_counter > 1;
}
