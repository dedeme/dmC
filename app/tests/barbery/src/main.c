// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/thread.h"
#include "main.h"
#include "kut/DEFS.h"
#include "kut/sys.h"
#include "clock.h"
#include "shop.h"
#include "client.h"
#include "barber.h"

int main(int argc, char *argv[]) {
  sys_init();
  shop_init();

  pthread_t *th1 = thread_start(clock_run);
  pthread_t *th2 = thread_start(client_maker_run);
  pthread_t *th3 = thread_start(barber_run);


  thread_join(th1);
  thread_join(th2);
  thread_join(th3);

  puts("Program end.");
}
