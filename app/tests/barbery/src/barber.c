// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/thread.h"
#include "barber.h"
#include "kut/DEFS.h"
#include "kut/sys.h"
#include "DEFS.h"
#include "shop.h"
#include "clock.h"

static void cut_hair(char *client) {
	puts(str_f("Barber: Cutting hair to %s", client));
	puts(str_f("Shop: %s", shop_sits_to_str()));
	sys_sleep(BARBER_TIME);
}

void barber_run (void) {
	puts("Barber: Go out from home");
	sys_sleep(1000);
	puts("Barber: Open barbery");
	shop_open();

	int end = FALSE;
  int	sleeping = FALSE;
	while (!end) {
		char *cl = "";
      //
      void fn (void) {
        if (clock_is_time_over() && shop_is_open()) {
          puts("Barber: Close barbery");
          shop_close();
        }

        if (shop_is_empty()) {
          if (!shop_is_open()) {
            end = TRUE;
          } else if (!sleeping) {
            sleeping = TRUE;
            puts("Barber: Barber is sleeping");
          }
        } else {
            //--
            void fn (void) { sleeping = FALSE; }
          thread_sync(fn);
          cl = shop_barber_take_a_client();
        }
      }
    thread_sync(fn);
		if (!str_eq(cl, "")) {
			cut_hair(cl);
		} else if (!end) {
			sys_sleep(10);
		}
	}
}
