// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/thread.h"
#include "client.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include "kut/sys.h"
#include "shop.h"

static void run (char *client) {
	void msg (char *m) {
		printf(str_f("Client %s: %s\n", client, m));
	}

	msg("Arrive to barbery");
	if (!shop_is_open()) {
		msg("Go out because barbery is close");
	} else if (shop_is_full()) {
		msg("Go out because barbery is full");
	} else {
      //--
      void fn (void *value) {
        shop_client_take_a_sit(client);
      }
    thread_sync2(fn, NULL);
	}
}

void client_maker_run (void) {
	int i = 0;
	while(shop_is_preopen() || shop_is_open()) {
		++i;
      //--
      void fn (void) {
        run(str_f("%d", i));
      }
    thread_run(fn);
    sys_sleep(10 + sys_rnd_i(CLIENT_MAKER_TIME));
	}
}
