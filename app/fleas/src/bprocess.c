// Copyright 21-Nov-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "bprocess.h"
#include <math.h>
#include "dmc/Darr.h"
#include "io.h"
#include  "DEFS.h"
#include "Fbest.h"

static void print_best(Fbest *bs, int n, int tt) {
  Fresults *rs = fbest_fresults(bs);
  Flea *fl = fresults_flea(rs);
  printf(
      "    Flea %s [%d / %d]\n"
      "    [days: %d, buy: %.4f, sell: %.4f]\n"
      "    [assets: %.2f, nbuys: %d, nsells: %d]\n"
      "    [avg: %.2f, cv: %.4f]\n",
      flea_id(fl), n, tt,
      flea_ndays(fl), flea_buy_strip(fl), flea_sell_strip(fl),
      fresults_assets(rs), fresults_buys(rs), fresults_sells(rs),
      fbest_average(bs), fbest_cv(bs)
  );
}

static Fbest *fdata_new(Fresults *rs, int days, char *date) {
  const double inc_days = 1.0 / (MAX_DAYS - MIN_DAYS - 1);
  const double min_days = 5 * inc_days;
  const double inc_strip = 0.0125;
  const double min_strip = 5 * inc_strip;
  Quote **quotes = io_quotes();
  int qdays = io_qdays();
  int qnicks = io_qnicks();
  int days_end = qdays * qnicks;

  Flea *fl = fresults_flea(rs);
  Gen *g = flea_gen(fl);
  double dfl = gen_get(g, gen_DAYS);
  if (dfl < min_days) dfl = min_days;
  double bfl = gen_get(g, gen_BUY);
  if (bfl < min_strip) bfl = min_strip;
  double sfl = gen_get(g, gen_SELL);
  if (sfl < min_strip) sfl = min_strip;

  Darr *assets = darr_bf_new(1340);

  RANGE(dsi, -5, 6)
  RANGE(bsi, -5, 6)
  RANGE(ssi, -5, 6)
    double ds = dfl + dsi * inc_days;
    double bs = bfl + bsi * inc_strip;
    double ss = sfl + ssi * inc_strip;

    Gen *g = gen_values_new(ds, bs, ss);
    Flea *tmpf = flea_gen_new(str_new("0"), g);

    flea_init(tmpf, qnicks);
    Fresults *rs = flea_process_new(
      tmpf, quotes, days_end - days * qnicks, days_end
    );

    darr_push(assets, fresults_assets(rs));
    fresults_free(rs);
  _RANGE
  _RANGE
  _RANGE

  double sum = 0;
  DEACH(assets, v)
    sum += v;
  _EACH
  double avg = sum / darr_size(assets);
  sum = 0;
  DEACH(assets, v)
    double dif = abs(v - avg);
    sum += dif * dif;
  _EACH
  double cv = sqrt(sum / (avg - 1)) / avg;

  darr_free(assets);

  return fbest_new(str_new(date), fresults_copy_new(rs), avg, cv);
}

static void group (int days) {
  // Arr[Fbest]
  Arr *bests;
  char *date;
  // Arr[Fresult]
  Arr *results;
  io_best_start(&bests, &date, &results, days);

  Fresults *tmp = fbest_fresults(arr_get(bests, 0));
  Fbest *b = fdata_new(tmp, days, date);

  puts("    __________________");
  puts("    Previous selection:\n");

  Quote **quotes = io_quotes();
  int qdays = io_qdays();
  int qnicks = io_qnicks();
  int days_end = qdays * qnicks;

  int c = 0;
  int tt = arr_size(bests);
  EACH(bests, Fbest, fb)
    Flea *fl = flea_copy_new(fresults_flea(fbest_fresults(fb)));
    flea_init(fl, qnicks);

    Fresults *tmp = flea_process_new(
      fl, quotes, days_end - days * qnicks, days_end
    );
    Fbest *newb = fdata_new(tmp, days, date);
    free(fl);
    free(tmp);
    print_best(newb, ++c, tt);
    if (
      fbest_average(newb) / fbest_cv(newb) >=
      fbest_average(b) / fbest_cv(b)
    ) {
      fbest_free(b);
      b = newb;
      puts("    -- Selected");
    } else {
      fbest_free(newb);
    }
  _EACH

  arr_free(bests);

  puts("    ____________________");
  puts("    Last bests selection:\n");

  c = 0;
  tt = arr_size(results);
  EACH(results, Fresults, rs)
    Fbest *newb = fdata_new(rs, days, date);
    print_best(newb, ++c, tt);
    if (
      fbest_average(newb) / fbest_cv(newb) >
      fbest_average(b) / fbest_cv(b)
    ) {
      fbest_free(b);
      b = newb;
      puts("    -- Selected");
    } else {
      fbest_free(newb);
    }
  _EACH

  arr_free(results);
  free(date);

  // Write results ---------------------------------------------------

  io_write_best(days, b);

  fbest_free(b);
}

void bprocess_run() {
  puts ("==================");
  puts ("| Best selection |");
  puts ("==================");

  int qdays = io_qdays();
  int days;
  for (days = DAYS_FIRST_GROUP; days < qdays; days += DAYS_NEXT_GROUP) {
    printf("Group: %d\n", days);
    group(days);
    puts("    Done");
  }
}
