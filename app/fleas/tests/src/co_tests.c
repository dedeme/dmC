// Copyright 27-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "co_tests.h"
#include "Co.h"
#include <assert.h>

void co_tests() {
  puts("Co tests");

  Co *c = co_new(3);

  assert(*co_closes(c) == -1);
  assert(*(co_closes(c) + 1) == -1);
  assert(*co_last_close(c) == -1);

  assert(!co_add(c, 15));
  assert(*co_closes(c) == -1);
  assert(*(co_closes(c) + 1) == -1);
  assert(*co_last_close(c) == 15);
  assert(co_ref(c) == -1);
  assert(!co_buying(c));

  assert(!co_add(c, 14));
  assert(*co_closes(c) == -1);
  assert(*(co_closes(c) + 1) == 15);
  assert(*co_last_close(c) == 14);
  assert(co_ref(c) == -1);
  assert(!co_buying(c));

  assert(co_add(c, 12));
  assert(*co_closes(c) == 15);
  assert(*(co_closes(c) + 1) == 14);
  assert(*co_last_close(c) == 12);
  assert(co_ref(c) == 15);
  assert(!co_buying(c));

  assert(!co_add(c, -1));
  assert(*co_closes(c) == 14);
  assert(*(co_closes(c) + 1) == 12);
  assert(*co_last_close(c) == -1);
  assert(co_ref(c) == 15);
  assert(!co_buying(c));

  assert(co_add(c, 16));
  assert(*co_closes(c) == 12);
  assert(*(co_closes(c) + 1) == -1);
  assert(*co_last_close(c) == 16);
  assert(co_ref(c) == 15);
  assert(!co_buying(c));

  assert(co_add(c, 14));
  assert(*co_closes(c) == -1);
  assert(*(co_closes(c) + 1) == 16);
  assert(*co_last_close(c) == 14);
  assert(co_ref(c) == 15);
  assert(!co_buying(c));

  assert(co_add(c, 14));
  assert(*co_closes(c) == 16);
  assert(*(co_closes(c) + 1) == 14);
  assert(*co_last_close(c) == 14);
  assert(co_ref(c) == 16);
  assert(!co_buying(c));

  assert(co_add(c, 13));
  assert(*co_closes(c) == 14);
  assert(*(co_closes(c) + 1) == 14);
  assert(*co_last_close(c) == 13);
  assert(co_ref(c) == 16);
  assert(!co_buying(c));

  assert(co_add(c, 10));
  assert(*co_closes(c) == 14);
  assert(*(co_closes(c) + 1) == 13);
  assert(*co_last_close(c) == 10);
  assert(co_ref(c) == 16);
  assert(!co_buying(c));
  assert(co_down(c, 0.50) < 0);
  assert(co_down(c, 0.20) > 0);
  assert(co_down(c, 0.20) > co_down(c, 0.25));

  co_set_buying(c, 1);
  co_set_ref(c, *co_closes(c));
  assert(co_buying(c));
  assert(co_ref(c) == 14);

  assert(co_add(c, 12));
  assert(*co_closes(c) == 13);
  assert(*(co_closes(c) + 1) == 10);
  assert(*co_last_close(c) == 12);
  assert(co_ref(c) == 13);
  assert(co_buying(c));

  assert(co_add(c, 12));
  assert(*co_closes(c) == 10);
  assert(*(co_closes(c) + 1) == 12);
  assert(*co_last_close(c) == 12);
  assert(co_ref(c) == 10);
  assert(co_buying(c));

  assert(co_add(c, 15));
  assert(*co_closes(c) == 12);
  assert(*(co_closes(c) + 1) == 12);
  assert(*co_last_close(c) == 15);
  assert(co_ref(c) == 10);
  assert(co_buying(c));
  assert(co_up(c, 0.50) < 0);
  assert(co_up(c, 0.20) > 0);
  assert(co_up(c, 0.20) > co_up(c, 0.25));

  co_set_buying(c, 1);




  co_free(c);

  puts("    Finished");
}

