// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "cts.h"
#include "kut/DEFS.h"
#include "kut/js.h"

char *cts_to_js(void) {
  return js_wo(map_from_array(arr_new_from(
    kv_new("historicQuotes", js_wi(cts_historic_quotes)),
    kv_new("initialCapital", js_wf(cts_initial_capital, 0)),
    kv_new("minToBet", js_wf(cts_min_to_bet, 0)),
    kv_new("bet", js_wf(cts_bet, 0)),
    kv_new("daysLoss", js_wi(cts_days_loss)),
    kv_new("noLossMultiplicator", js_wf(cts_no_loss_multiplicator, 2)),
    kv_new("withdrawalLimit", js_wf(cts_withdrawal_limit, 0)),
    kv_new("orderBuy", js_wi(cts_order_buy)),
    kv_new("orderSell", js_wi(cts_order_sell)),
    NULL
  )));
}
