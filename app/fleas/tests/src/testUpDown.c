// Copyright 28-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "testUpDown.h"
#include "assert.h"
#include "market/Quote.h"
#include "families/UpDown.h"

void testUpDown_run(void) {
  puts("UpDown test:");

  Gen *gens = upDown_prototype();
  assert(gen_get(gens, 0) == 15000);
  assert(gen_get(gens, 1) == 20);
  assert(gen_get(gens, 2) == 0.1);
  assert(gen_get(gens, 3) == 0.05);

  double dgens[4] = {15000, 3, 0.1, 0.05};
  gens = gen_new(4, dgens);

  Quote *qs[20] = {
    quote_new(-1, 20, -1, -1, -1), quote_new(-1, 26, -1, -1, -1),
    quote_new(-1, 22, -1, -1, -1), quote_new(-1, 24, -1, -1, -1),
    quote_new(-1, 24, -1, -1, -1), quote_new(-1, 20, -1, -1, -1),
    quote_new(-1, 26, -1, -1, -1), quote_new(-1, 18, -1, -1, -1),
    quote_new(-1, 28, -1, -1, -1), quote_new(-1, 16, -1, -1, -1),
    quote_new(-1, 26, -1, -1, -1), quote_new(-1, 18, -1, -1, -1),
    quote_new(-1, 24, -1, -1, -1), quote_new(-1, 20, -1, -1, -1),
    quote_new(-1, 22, -1, -1, -1), quote_new(-1, 24, -1, -1, -1),
    quote_new(-1, 20, -1, -1, -1), quote_new(-1, 26, -1, -1, -1),
    quote_new(-1, 18, -1, -1, -1), quote_new(-1, 28, -1, -1, -1)
  };

  Abuy *buys;
  int *sell_nks;
  int sell_nks_len;
  Quote **pqs = qs;

  upDown_init(gens, 2);

  upDown_operate(&buys, &sell_nks, &sell_nks_len, pqs);
  assert(abuy_size(buys) == 0);
  assert(sell_nks_len == 0);

  upDown_operate(&buys, &sell_nks, &sell_nks_len, pqs + 2);
  assert(abuy_size(buys) == 0);
  assert(sell_nks_len == 0);

  upDown_operate(&buys, &sell_nks, &sell_nks_len, pqs + 4);
  assert(abuy_size(buys) == 0);
  assert(sell_nks_len == 0);

  upDown_operate(&buys, &sell_nks, &sell_nks_len, pqs + 6);
  assert(abuy_size(buys) == 1);
  assert(buy_nick(abuy_get(buys, 0)) == 0);
  assert(sell_nks_len == 1);
  assert(sell_nks[0] == 1);

  upDown_operate(&buys, &sell_nks, &sell_nks_len, pqs + 8);
  assert(abuy_size(buys) == 0);
  assert(sell_nks_len == 0);

  upDown_operate(&buys, &sell_nks, &sell_nks_len, pqs + 10);
  assert(abuy_size(buys) == 0);
  assert(sell_nks_len == 0);

  upDown_operate(&buys, &sell_nks, &sell_nks_len, pqs + 12);
  assert(abuy_size(buys) == 1);
  assert(buy_nick(abuy_get(buys, 0)) == 1);
  assert(sell_nks_len == 1);
  assert(sell_nks[0] == 0);

  upDown_operate(&buys, &sell_nks, &sell_nks_len, pqs + 14);
  assert(abuy_size(buys) == 0);
  assert(sell_nks_len == 0);

  upDown_operate(&buys, &sell_nks, &sell_nks_len, pqs + 16);
  assert(abuy_size(buys) == 0);
  assert(sell_nks_len == 0);

  upDown_operate(&buys, &sell_nks, &sell_nks_len, pqs + 18);
  assert(abuy_size(buys) == 0);
  assert(sell_nks_len == 0);

  puts("    Finished");
}

