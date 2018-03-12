// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "fleas.h"
#include <dmc/all.h>
#include "market/Quote.h"
#include "io.h"
#include "Options.h"
#include "Db.h"
#include "Quotes.h"
#include "DEFS.h"
#include "Flea.h"
#include "Trace.h"

#include "unistd.h"

static void help (void) {
  puts(
    "fleas. v201803\n\n"
    "Use:\n"
		"  fleas [s | b | sb] \n"
		"  fleas [s | b | sb] <time_minutes> -> (e.g. fleas 5)\n"
		"  fleas [s | b | sb] trace <flea> -> (e.g. fleas trace 12543)\n"
    "  fleas [s | b | sb] dup (remove duplicate fleas)\n"
		"  fleas stop\n"
		"  fleas force (remove stop lock)\n"
		"  fleas backup <dir> -> (e.g. fleas backup ./)\n"
		"  fleas restore <file> -> (e.g. fleas restore Fleas20181201.zip)\n"
		"  or\n"
		"  fleas help"
  );
}

static void process(size_t minutes, size_t traced) {
  Db *db = db_new();

  Json *ftraced = NULL;
  Arr/*Trace*/ *traces = arr_new();

  Arr/*char */ *qdates = db_qdates(db);
  Quote **quotes = db_quotes(db);
  Nicks *nicks = db_nicks(db);

  Date t1 = date_now();
  for (;;) {
    size_t cycle = db_cycle(db);
    printf("Cycle %d\n", cycle);

    Fleas *fleas = db_fleas(db);
    size_t killed = 0;
    RANGE0(ifl, FLEAS_NUMBER) {
      Flea *f = fleas_get(fleas, ifl);
      if (flea_id(f) == traced) {
        ftraced = flea_serialize(f);
      }
      flea_prepare(f, cycle);
      RANGE0(iq, QUOTES_NUMBER) {
        flea_process(
          f, db, arr_get(qdates, iq), quotes + iq * NICKS_NUMBER, traced, traces
        );
      }_RANGE

      double cash = flea_cash(f);
      EACH(flea_portfolio(f), Pf_entry, e) {
        double q = -1;
        for (int i = QUOTES_NUMBER - 1; i >= 0; --i) {
          double q0 = quote_close(
            quotes[i * NICKS_NUMBER + pf_entry_nick(e)]
          );
          if (q0 > 0) {
            q = q0;
            break;
          } else if (q0 < -1.1) {
            q = 0;
            break;
          }
        }
        if (q < -0.1) {
          THROW
            "%s: Wrong quotes",
            nick_id(nicks_get(nicks, pf_entry_nick(e)))
          _THROW
        }

        cash += sell_income(pf_entry_stocks(e), q);
      }_EACH

      stat_setup(
        flea_stats(f), cycle - flea_cycle(f),
        cash, flea_nbuys(f), flea_nsells(f)
      );
      if (
        cash > (INITIAL_CASH + 1) &&
        stat_buys(flea_stats(f)) + stat_sells(flea_stats(f)) > MIN_OPERATIONS
      ) {
        flea_reset(f);
      } else {
        fleas_kill(fleas, ifl);
        ++killed;
      }
    }_RANGE

    fleas_sort(fleas);

    if (killed < WORSTS_NUMBER) {
      RANGE(ifl, FLEAS_NUMBER - WORSTS_NUMBER, FLEAS_NUMBER) {
        fleas_kill(fleas, ifl);
      }_RANGE
    }

    size_t ix_bests = 0;
    Arr/*Flea*/ *fbests[FAMILIES_END];
    size_t ixs[FAMILIES_END];
    size_t ixs_ix = 0;
    RANGE0(i, FAMILIES_END) {
      fbests[i] = arr_new();
      ixs[i] = 0;
    }_RANGE
    RANGE0(ifl, FLEAS_NUMBER) {
      Flea *f1 = fleas_get(fleas, ifl);
      if (!f1) {
        if (ix_bests < BESTS_NUMBER) {
          Flea *best_flea = fleas_get(fleas, ix_bests++);
          if (best_flea) {
            fleas_set(fleas, ifl, flea_mutate(
              best_flea, db_next_flea_id(db), cycle + 1
            ));
          } else {
            ix_bests = BESTS_NUMBER;
            --ifl;
          }
        } else if (ixs_ix < FAMILIES_END) {
          Arr/*Flea*/ *afm = fbests[ixs_ix];
          if (ixs[ixs_ix] < arr_size(afm)) {
            fleas_set(fleas, ifl, flea_mutate(
              arr_get(afm, ixs[ixs_ix]), db_next_flea_id(db), cycle + 1
            ));
            ++ixs[ixs_ix];
          } else {
            ++ixs_ix;
            --ifl;
          }
        } else {
          Flea *f = flea_new(db_next_flea_id(db), cycle + 1);
          fleas_set(fleas, ifl, f);
        }
      } else if (family_opt(flea_family(f1)) == FAMILIES_ALL) {
        size_t fm = gen_actual(family_gen(flea_family(f1)));
        Arr/*Flea*/ *afm = fbests[fm];
        if (arr_size(afm) < BESTS_FAMILIES_NUMBER) {
          arr_add(afm, f1);
        }
      }
    }_RANGE

    Fleas *bests = fleas_new(BESTS_NUMBER);
    RANGE0(ib, BESTS_NUMBER) {
      fleas_set(bests, ib, fleas_get(fleas, ib));
    }_RANGE

    io_save_bests(cycle, fleas_serialize(bests));
    db_inc_cycle(db);
    db_save(db);

    Date t2 = date_now();
    double min = (t2 - t1) / 60.0;
    printf("Killed: %d\n%.2f minutes\n\n", killed, min);

    if (min >= minutes) {
      break;
    }
  }

  if (traced) {
    Arr /*Json*/ *traces_data = arr_new();

    if (!ftraced) {
      THROW exc_null_pointer("ftraced") _THROW
    }
    arr_add(traces_data, ftraced);

    Arr/*Json*/ *jtraces = arr_new();
    EACH(traces, Trace, tr) {
      arr_add(jtraces, trace_serialize(tr, nicks));
    }_EACH
    jarr_aarray(traces_data, jtraces);

    io_save_traces(json_warray(traces_data));
  }
}

void gc_messages(char *msg, long unsigned int arg) {
  if (str_index(msg, "very large block") == -1) {
    printf(msg, arg);
  }
}

int main (int argc, char **argv) {
//  GC_set_warn_proc(gc_messages);

  void argv_rotation() {
    RANGE(i, 1, argc - 1) {
      argv[i] = argv[i + 1];
    }_RANGE
  }

  sys_init("fleas");

  if (argc > 1) {
    if (!strcmp(argv[1], "s")) {
      options_init(false, true);
      argv_rotation();
      --argc;
    } else if (!strcmp(argv[1], "b")) {
      options_init(true, false);
      argv_rotation();
      --argc;
    } else if (!strcmp(argv[1], "sb")) {
      options_init(true, true);
      argv_rotation();
      --argc;
    } else {
      options_init(false, false);
    }
  } else {
    options_init(false, false);
  }

  io_init();

  if (argc == 1) {
    process(0, 0);
  } else if (argc == 2) {
    if (!strcmp(argv[1], "dup")) {
      io_duplicates();
    } else if (dec_digits(argv[1])) {
      process (atol(argv[1]), 0);
    } else {
      help();
    }
  } else if (argc == 3) {
    if (!strcmp(argv[1], "trace")) {
      size_t id = atol(argv[2]);
      if (!id) {
        THROW "Flea '0' can not be traced" _THROW
      }
      process(0, id);
    } else if (!strcmp(argv[1], "backup")) {
      io_backup(argv[2]);
    } else if (!strcmp(argv[1], "restore")) {
      io_restore(argv[2]);
    } else {
      help();
    }
  } else {
    help();
  }

  return 0;
}
