// Copyright 11-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "unistd.h"
#include "dmc/cryp.h"
#include "dmc/date.h"
#include "io.h"
#include "DEFS.h"
#include "Server.h"
#include "Nick.h"
#include "pf.h"
#include "trading.h"

static void start (char *key) {
  char *state;
  io_state_read(&state);
  Server *sv = server_current_new();
  for(;;) {
    if (!io_lock_check(key)) {
      break;
    }
    if (str_eq(state, ST_SLEEPING)) {
puts("sleeping");

      if (server_active(sv)) {
        server_update(sv);
        sleep(420);
      } else {
        server_update(sv);
        if (server_active(sv)) {
          state = ST_TO_ACTIVE;
          io_state_write(state);
        } else {
          sleep(420);
        }
      }
    } else if (str_eq(state, ST_TO_ACTIVE)) {
puts("toActive 1");
      // Arr[Server]
      Arr *all = server_servers_new();
      int n = server_number();
      EACH(all, Server, sv)
        server_update(sv);
      _EACH
      sleep(420);
puts("toActive 2");
      int actives = 0;
      EACH(all, Server, sv)
        server_update(sv);
        if (server_active(sv)) ++actives;
      _EACH

      if (actives > n / 2) {
        // Map[PfValue]
        Map *pf = pf_read_new();
        Darr *signals = trading_read_new();
        char *nicks[] = NICKS;
        char **p = nicks;
        int c = 0;
        while (*p) {
          char *nick = *p++;
          nick_new(nick, map_get_null(pf, nick), darr_get(signals, c++));
        }
        state = ST_ACTIVE;
        io_state_write(state);
      } else {
        server_set_active(sv, 0);
        state = ST_SLEEPING;
        io_state_write(state);
        sleep(420);
      }
    } else if (str_eq(state, ST_ACTIVE)) {
puts("active");
      server_update(sv);
      if (server_active(sv)) {
        Darr *qs = server_qs(sv);
        EACH_IX(server_nicks(sv), char, nk, ix)
          nick_update(nk, darr_get(qs, ix));       // Update nicks data
        _EACH
        sleep(420);
      } else {
        state = ST_TO_SLEEPING;
        io_state_write(state);
      }
    } else if (str_eq(state, ST_TO_SLEEPING)) {
puts("toSleeping");
      // Arr[Server]
      Arr *all = server_servers_new();
      int n = server_number();
      EACH(all, Server, sv)
        server_update(sv);
      _EACH
      sleep(420);
puts("toSleeping2");
      int actives = 0;
      EACH(all, Server, sv)
        server_update(sv);
        if (server_active(sv)) ++actives;
      _EACH

      if (actives < n / 4) {
        server_free(sv);
        sv = server_next_new();
        state = ST_SLEEPING;
        io_state_write(state);
      } else {
        server_set_active(sv, 1);
        state = ST_ACTIVE;
        io_state_write(state);
        sleep(420);
      }
    } else {
      FAIL(str_f_new("Unknown state '%s'", state));
    }
  }
  server_free(sv);
  free(state);
}

int main(int argc, char* args[]) {
  sys_init("market_data");

  io_init();

  if (argc == 2 && str_eq(args[1], "start")) {
    char *k0 = cryp_genk_new(10);
    char *k = str_f_new("%s%d", k0, date_now());
    free(k0);
    io_lock_write(k);
    start(k);
    free(k);
  } else if (argc == 2 && str_eq(args[1], "stop")) {
    io_lock_del();
  } else {
    puts(
      "market_data. v201901\n"
      "\n"
      "Usage:\n"
      "  market_data start\n"
      "    Start daemon\n"
      "  market_data stop\n"
      "    Stop daemon\n"
    );
  }

  sys_end();
  return 0;
}
