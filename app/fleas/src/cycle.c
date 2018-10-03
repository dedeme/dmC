// Copyright 29-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "cycle.h"
#include "io.h"
#include "Flea.h"
#include "DEFS.h"
#include "Gen.h"
#include "utils/CutCalculator.h"
#include "utils/CutFleaCalculator.h"
#include "data/HistoricEntry.h"

void cycle_run(void) {
  Group **groups;
  int groups_number;
  io_groups_list(&groups, &groups_number);

  double proximity_number = PROXIMITY_NUMBER;

  RANGE0(group_i, groups_number) {
    Group *group = groups[group_i];

    Achar *nicks;
    Achar *dates;
    Quote ***quotes;
    io_read_quotes(&nicks, &dates, &quotes, QUOTES_DATA, group);
    int companies_number = achar_size(nicks);
    int days_number = achar_size(dates);

    cutFleaCalculator_init(RANKING_NUMBER);
    int high_cycle = 0;

    Family **families;
    int families_number;
    flea_families_list(&families, &families_number);
    RANGE0(family_i, families_number) {
      Family *family = families[family_i];

      // Read data ---------------------------------------------------

      int cycle;
      double cut_proximity;
      double inc_proximity;
      Aflea *fleas;
      io_read_family(
        &cycle, &cut_proximity, &inc_proximity, &fleas, group, family
      );
      if (cycle > high_cycle) {
        high_cycle = cycle;
      }

      // Reproducción ------------------------------------------------

      int fsize = aflea_size(fleas);
      if (fsize) {
        int ix = 0;
        RANGE0(i, FLEA_NUMBER_PER_FAMILY - fsize) {
          aflea_add(fleas, flea_mutate(aflea_get(fleas, ix), cycle, i));
          ++ix;
          if (ix == fsize) {
            ix = 0;
          }
        }_RANGE

      } else {
        RANGE0(i, FLEA_NUMBER_PER_FAMILY) {
          aflea_add(fleas, flea_mutate(flea_prototype(family), 0, i));
        }_RANGE
      }
      // Operations --------------------------------------------------

      Aflea *new_fleas = aflea_new();
      EACH(fleas, Flea, f) {
        flea_operate(f, INITIAL_CAPITAL, quotes, days_number, companies_number);
        if (flea_sells(f) > MIN_SELLS) {
          aflea_add(new_fleas, f);
        }
        flea_set_ponderation(f, flea_assets(f));
      }_EACH

      // Selection ---------------------------------------------------

      int new_fleas_size = aflea_size(new_fleas);
/*
      if (cut_proximity == 0) {
        double sum = 0;
        int n = 0;
        int i, j;
        for (i = 0; i < new_fleas_size - 1; ++i) {
          Flea *f1 = aflea_get(new_fleas, i);
          Gen *gens1 = flea_gens(f1);
          for (j = i; j < new_fleas_size; ++j) {
            Flea *f2 = aflea_get(new_fleas, j);
            Gen *gens2 = flea_gens(f2);
            sum += gen_proximity(gens1, gens2);
            ++n;
          }
        }
        cut_proximity = sum / n;
      }
*/
      int current_aproxs = 0;
      int i,j;
      for (i = 0; i < new_fleas_size - 1; ++i) {
        Flea *f1 = aflea_get(new_fleas, i);
        double assets1 = flea_assets(f1);
        double pond1 = flea_ponderation(f1);
        Gen *gens1 = flea_gens(f1);
        for (j = i; j < new_fleas_size; ++j) {
          Flea *f2 = aflea_get(new_fleas, j);
          Gen *gens2 = flea_gens(f2);
          double prox = gen_proximity(gens1, gens2);
//          if (gpond > 0.9999) {
//            flea_set_ponderation(f2, -1);
//          }
//          if (flea_ponderation(f2) > -0.5) {
          if (prox >= cut_proximity) {
            ++current_aproxs;
            double assets2 = flea_assets(f2);
            double pond2 = flea_ponderation(f2);
            double divisor = prox + 1;
            flea_set_ponderation(f1, (pond1 + assets2 * prox) / divisor);
            flea_set_ponderation(f2, (pond2 + assets1 * prox) / divisor);
          }
//          }
        }
      }
      printf(
        "    %4f -> %d. Inc: %4f\n",
        cut_proximity, current_aproxs, inc_proximity
      );
      if (current_aproxs > proximity_number) {
        if (inc_proximity < 0) {
          inc_proximity = -inc_proximity / 2;
        } else {
          inc_proximity *= 1.5;
        }
      } else if (current_aproxs < proximity_number) {
        if (inc_proximity > 0) {
          inc_proximity = -inc_proximity / 2;
        } else {
          inc_proximity *= 1.5;
        }
      }
      cut_proximity += inc_proximity;

      double sum = 0;
      EACH(new_fleas, Flea, f) {
        sum += flea_ponderation(f);
      }_EACH

      double avg = sum / new_fleas_size;
      fleas = aflea_new();
      EACH(new_fleas, Flea, f) {
        double pond = flea_ponderation(f);
        if (pond > avg) {
          aflea_add(fleas, f);
          cutFleaCalculator_add(f);
        }
      }_EACH

      // Write data --------------------------------------------------

      printf(
        "Writing cycle %d of '%s/%s'. Avg: %.2f\n",
        cycle, flea_family_to_str(family), io_group_to_str(group), avg
      );
      io_write_family(
        group, family, cycle + 1, cut_proximity, inc_proximity, fleas
      );

    }_RANGE

    io_write_historic(group, high_cycle, cutFleaCalculator_fleas());

  }_RANGE

}
