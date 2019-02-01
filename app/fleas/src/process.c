// Copyright 28-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "process.h"
#include "DEFS.h"
#include "io.h"
#include "Flea.h"
#include "Fresults.h"

void process_run(void) {
  Quote **quotes = io_quotes();
  int qdays = io_qdays();
  int qnicks = io_qnicks();
  int days_end = qdays * qnicks;

  puts ("-----------------  ___");
  int days;
  for (days = DAYS_FIRST_GROUP; days < qdays; days += DAYS_NEXT_GROUP) {
    char *sdays = str_f_new("%d", days);
    double min_sells = (days - DAYS_STAND_BY) / MIN_SELLS;
    double max_sells = (days - DAYS_STAND_BY) / MAX_SELLS;
    double max_sells_sum = max_sells / 2;
    double max_sells_mul = max_sells / (INITIAL_CAPITAL + INITIAL_CAPITAL);

    // Arr[Fresults]
    Arr *fleas = arr_new((FPROC)fresults_free);
    RANGE0(i, FLEAS_PER_GROUP / 2)
      arr_push(fleas, fresults_new(flea_new(str_f_new("0-%d", i)), 0, 0, 0));
    _RANGE

    RANGE(cycle, 1, CYCLES + 1) {
      printf("Group: %s, cycle: %3d\n", sdays, cycle);

      // Prepare-mutate fleas ----------------------------------------
      int fleas_start_size = arr_size(fleas);
      double sum = 0;
      EACH(fleas, Fresults, r)
        sum += fresults_assets(r);
      _EACH
      int oldc = 0;
      int n = fleas_start_size;

      REPEAT(FLEAS_PER_GROUP - fleas_start_size)
        Flea *oldFlea = fresults_flea(arr_get(fleas, oldc++));
        if (oldc == fleas_start_size) {
          oldc = 0;
        }

        char *id = str_f_new("%d-%d", cycle, n - fleas_start_size);
        Flea *fl = flea_mutate_new(oldFlea, id);
        flea_init(fl, qnicks);

        Fresults *rs = flea_process_new(
          fl, quotes, days_end - days * qnicks, days_end
        );
        double assets = fresults_assets(rs);

        if (
          fresults_sells(rs) >= min_sells &&
          fresults_sells(rs) <= max_sells_sum + assets * max_sells_mul
        ) {
          arr_push(fleas, rs);
          sum += assets;
          ++n;
        } else {
          fresults_free(rs);
        }
      _REPEAT

      // Filter - Annotate -------------------------------------------
      double avg = sum / n;
      int filter(Fresults *f) {
        return fresults_assets(f) >= avg;
      }
      arr_filter(fleas, (FPRED)filter);

      printf(
        "  Survivers: %d of %d. Avg: %.2f\n",
        arr_size(fleas), n, avg
      );

      if (io_stop()) {
        break;
      }
    } _RANGE

    if (io_stop()) {
      free(sdays);
      arr_free(fleas);
      break;
    }

    io_write_results(sdays, fleas);
    printf("----- ___\nGroup %s finished\n----- TTT\n", sdays);

    free(sdays);
    arr_free(fleas);
  }

  if (io_stop()) {
    io_del_stop();
  }
}
