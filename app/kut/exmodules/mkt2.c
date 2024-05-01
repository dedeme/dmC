// Copyright 17-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "kut/dec.h"
#include "exmodule.h"
#include "math.h"

// GENERAL ---------------------------------------------------------------------

#define initialCapital 250000
#define min_to_bet 13000.0
#define bet 12000
#define days_loss 45 // 5 days per week -> 9 weeks
#define no_loss_multiplicator 1.02 // 'sell >= buy * no_loss_multiplicator' is ok.
static double withdrawal_limit = initialCapital + bet + bet;
#define order_buy 0
#define order_sell 1

typedef struct exmodule_Vec {
  int size;
  double *vs;
} Vec;

// private
static Exp *pack_vec(int size, double *vs) {
  double *dup = ATOMIC(sizeof(double) * size);
  memcpy(dup, vs, sizeof(double) * size);
  Vec *vec = MALLOC(Vec);
  vec->size = size;
  vec->vs = dup;
  return exp_object("<vec>", vec);
}

// \[f...] -> <vec>
static Exp *mk_vec(Exp *exp) {
  // <Exp>
  Arr *a = exp_get_array(exp);
  int size = arr_size(a);
  double *vs = ATOMIC(size * sizeof(double));
  EACH(a, Exp, e) {
    vs[_i] = exp_rget_float(e);
  }_EACH
  Vec *v = MALLOC(Vec);
  v->size = size;
  v->vs = vs;
  return exp_object("<vec>", v);
}

// \<vec> -> [f...]
static Exp *vec_to_arr(Exp *exp) {
  Vec *v = exp_rget_object("<vec>", exp);
  int size = v->size;
  double *vs = v->vs;
  Exp *es[size];
  for (int i = 0; i < size; ++i) es[i] = exp_float(vs[i]);
  return exp_array(arr_new_c(size, (void **)es));
}

// QUOTES ----------------------------------------------------------------------

// \s -> [[s, f, f, f, f, i, b]|s, b] (Result)
static Exp *quote_from_str_aux(char *qs) {
  char *q = str_trim(qs);
  Exp *mkErr(char *e) {
    char *msg = str_f("%s (%s)", e, q, NULL);
    return exp_array(arr_new_from(exp_string(msg), exp_bool(FALSE), NULL));
  }

  if (!*q) return mkErr("Quote string is empty");

  // <char>
  Arr *ps = str_csplit(q, ':');
  if (arr_size(ps) != 7) return mkErr("Bad fields number");
  char **a = (char **)arr_begin(ps);

  char *date = a[0];
  if (strlen(date) != 8 || !dec_digits(date))
    return mkErr(str_f("Bad date value: '%s'", date, NULL));

  double open, close, max, min;
  int64_t vol;
  char *err = "";
  char *msg = "";
  TRY {
    msg = "Bad open value";
    open = dec_stod(a[1]);
    msg = "Bad close value";
    close = dec_stod(a[2]);
    msg = "Bad max value";
    max = dec_stod(a[3]);
    msg = "Bad min value";
    min = dec_stod(a[4]);
    msg = "Bad vol value";
    vol = dec_stoi(a[5]);
  } CATCH(e) {
    err = exc_msg(e);
  }_TRY
  if (*err) return mkErr(msg);

  char *force = a[6];
  if (strcmp(force, "true") && strcmp(force, "false"))
    return mkErr("Bad force value");

  return exp_array(arr_new_from(
    exp_array(arr_new_from(
        exp_string(date),
        exp_float(open),
        exp_float(close),
        exp_float(max),
        exp_float(min),
        exp_int(vol),
        exp_bool(strcmp(force, "true") ? FALSE : TRUE)
      )),
    exp_bool(TRUE),
    NULL
  ));
}

// \s -> [[[s, f, f, f, f, i, b]...]|s, b] (Result)
static Exp *qs_from_str(Exp *exp) {
  char *qs = str_trim(exp_rget_string(exp));
  if (!*qs) {
    char *msg = "Quotes string is empty";
    return exp_array(arr_new_from(exp_string(msg), exp_bool(FALSE), NULL));
  }

  // <Exp>
  Arr *r = arr_new();
  // <char>
  Arr *ps = str_csplit(qs, '\n');
  EACH(ps, char, q) {
    Exp *q_rs = quote_from_str_aux(q);
    // <Exp>
    Exp **a = (Exp **)arr_begin(exp_get_array(q_rs));
    if (!exp_rget_bool(a[1]))
      return q_rs;
    arr_push(r, a[0]);
  }_EACH

  return exp_array(arr_new_from(
    exp_array(r),
    exp_bool(TRUE),
    NULL
  ));
}

// \s -> [[s, f, f, f, f, i, b]|s, b] (Result)
static Exp *quote_from_str(Exp *exp) {
  return quote_from_str_aux(exp_rget_string(exp));
}

// \[s, i] -> f
static Exp *qs_vol(Exp *exp) {
  // <Exp>
  Exp **exps = (Exp **)arr_begin(exp_rget_array(exp));
  char *qs = str_trim(exp_rget_string(exps[0]));
  int samples = exp_rget_int(exps[1]);

  if (!*qs) return exp_float(0);

  double maxs[3];
  double mins[3];
  double sum = 0;
  int c0 = 0;
  int c = 0;
  // <char>
  Arr *ps = str_csplit(qs, '\n');
  EACH(ps, char, q) {
    // <char>
    Arr *fields = str_csplit(q, ':');
    if (arr_size(fields) != 7) continue;
    char **a = (char **)arr_begin(fields);
    double tmp, max, min;
    double vol = -1;
    TRY {
      max = dec_stod(a[3]);
      min = dec_stod(a[4]);
      vol = dec_stoi(a[5]);
      vol = (max + min) * vol;
    } CATCH(e) {
      e = e;
    }_TRY
    if (vol < 0) continue;

    if (c0 < 3) {
      maxs[c0] = vol;
      mins[c0] = vol;
      ++c0;
      continue;
    }

    int more = TRUE;
    RANGE0(i, 3) {
      if (vol > maxs[i]) {
        tmp = vol, vol = maxs[i]; maxs[i] = tmp;
        more = FALSE;
      }
    }_RANGE

    if (more) {
      RANGE0(i, 3) {
        if (vol < mins[i]) {
          tmp = vol, vol = mins[i]; mins[i] = tmp;
        }
      }_RANGE
    }

    sum += vol;
    ++c;
    if (c >= samples) break;
  }_EACH

  float r = c < samples
    ? 0
    : sum / (samples * 2)
  ;
  return exp_float(r);
}

// STRATEGY --------------------------------------------------------------------

static double broker_buy_fees(double amount) {
  return (amount > 50000      // broker
      ? amount * 0.001
      : 9.75
    ) +
    amount * 0.00003 +       // market
    0.11 +                   // Execution fee
    amount * 0.004           // tobin + penalty
  ;
}

static double broker_buy (int64_t stocks, double price) {
  double amount = stocks * price;
  return amount + broker_buy_fees(amount);
}

static double broker_sell_fees(double amount) {
  return (amount > 50000      // broker
      ? amount * 0.001
      : 9.75
    ) +
    amount * 0.00003 +       // market
    0.11 +                   // Execution fee
    amount * 0.002           // penalty
  ;
}

static double broker_sell (int64_t stocks, double price) {
  double amount = stocks * price;
  return amount - broker_sell_fees(amount);
}

// \f -> f
static Exp *broker_buy_fees_f(Exp *exp) {
  return exp_float(broker_buy_fees(exp_rget_float(exp)));
}

// \i, f -> f
static Exp *broker_buy_f(Exp *exp) {
  Exp **a = (Exp**)arr_begin(exp_rget_array(exp));
  return exp_float(broker_buy(exp_rget_int(a[0]), exp_rget_float(a[1])));
}

// \f -> f
static Exp *broker_sell_fees_f(Exp *exp) {
  return exp_float(broker_sell_fees(exp_rget_float(exp)));
}

// \i, f -> f
static Exp *broker_sell_f(Exp *exp) {
  Exp **a = (Exp**)arr_begin(exp_rget_array(exp));
  return exp_float(broker_sell(exp_rget_int(a[0]), exp_rget_float(a[1])));
}

// \[s.], [s.], [<vec>.], [<vec>.], [<vec>.] -> <strgRs>
static Exp *strategy_run(Exp *exp) {
  // FUNCTIONS -----

  // <char *>
  Arr *unpackStrings(Exp *exp) {
    // <Exp<string>>
    Arr *sexps = exp_rget_array(exp);
    // <char *>
    Arr *r = arr_new_bf(arr_size(sexps));
    EACH(sexps, Exp, e) {
      arr_push(r, exp_rget_string(e));
    }_EACH
    return r;
  }

  double **unpackArrayVectors(Exp *exp) {
    // <Exp<Vector>>
    Arr *vexps = exp_rget_array(exp);

    // <char *>
    double **r = ATOMIC(sizeof(double *) * arr_size(vexps));;
    EACH(vexps, Exp, e) {
      r[_i] = ((Vec *)exp_rget_object("<vec>", e))->vs;
    }_EACH
    return r;
  }

  double *mkFloats(int n, double val) {
    double *r = ATOMIC(sizeof(double) * n);
    double *p = r;
    REPEAT(n) {
      *p++ = val;
    }_REPEAT
    return r;
  }

  // Exp<[f.]>
  Exp *mkFloatsArray(int n, double *vals) {
    double *p = vals;
    // <Exp>
    Arr *r = arr_new();
    REPEAT(n) {
      arr_push(r, exp_float(*p++));
    }_REPEAT
    return exp_array(r);
  }

  int *mkInts(int n, int val) {
    int *r = ATOMIC(sizeof(int) * n);
    int *p = r;
    REPEAT(n) {
      *p++ = val;
    }_REPEAT
    return r;
  }

  Exp *mkOrder(char *date, char *nick, int type, int stocks, double price) {
    // <Exp>
    Map *value = map_new();
    map_add(value, "date", exp_string(date));
    map_add(value, "nick", exp_string(nick));
    map_add(value, "type", exp_int(type));
    map_add(value, "stocks", exp_int(stocks));
    map_add(value, "price", exp_float(price));
    return exp_dic(value);
  }

  // PROCESS -----

  Exp **params = (Exp **)arr_begin(exp_get_array(exp));

  // <char *>
  Arr *adates = unpackStrings(params[0]);
  char **dates = (char **)arr_begin(adates);
  // <char *>
  Arr *cos = unpackStrings(params[1]);
  char **nks = (char **)arr_begin(cos);
  int ncos = arr_size(cos);
  double **allOpens = unpackArrayVectors(params[2]);
  double **allCloses = unpackArrayVectors(params[3]);
  double **allRefs = unpackArrayVectors(params[4]);

  double cash = initialCapital;
  double withdrawals = 0.0;
  // <Exp<order>>
  Arr *orders = arr_new();
  // <Exp<float>>
  Arr *hassets = arr_new();
  // <Exp<float>>
  Arr *hwithdrawals = arr_new();
  // <Arr<Exp<string>>>
  Arr *aallBuys = arr_new();
  REPEAT(ncos) {
    arr_push(aallBuys, arr_new());
  }_REPEAT
  Arr **allBuys = (Arr **)arr_begin(aallBuys);
  // <Arr<Exp<string>>>>
  Arr *aallSales = arr_new();
  REPEAT(ncos) {
    arr_push(aallSales, arr_new());
  }_REPEAT
  Arr **allSales = (Arr **)arr_begin(aallSales);

  double *prf_cashes = mkFloats(ncos, bet);
  int *prf_stocks = mkInts(ncos, 0);

  int *stockss = mkInts(ncos, 0);
  double *prices = mkFloats(ncos, 0);
  double *prf_prices = mkFloats(ncos, 0);
  int *to_dos = mkInts(ncos, FALSE);
  int *to_sells = mkInts(ncos, TRUE);
  int *traps = mkInts(ncos, 0);
  int *prf_traps = mkInts(ncos, 0);

  double *closes; // At the end of loop has got the last closes.
  REPEAT(arr_size(adates)) {
    char *date = *dates++;
    double *opens = *allOpens++;
    closes = *allCloses++;
    double *refs = *allRefs++;

    double assets = 0;
    char **pnks = nks;
    RANGE0(i, ncos) {
      char *nk = *pnks++;
      double op = *opens++;
      double cl = *closes++;
      double rf = *refs++;

      // <Exp<String>>
      Arr *buys = allBuys[i];
      // <Exp<String>>
      Arr *sales = allSales[i];
      --traps[i];
      --prf_traps[i];

      if (to_dos[i]) {
        if (to_sells[i]) { // there is buy order.
          // Global simulation.
          if (traps[i] < 1) {
            if (cash > min_to_bet) {
              int stocks = (int)(bet / op);
              stockss[i] = stocks;
              prices[i] = op;
              cash -= broker_buy(stocks, op);
              arr_push(orders, mkOrder(date, nk, order_buy, stocks, op));
            }
          }
          // Profits simulation.
          if (prf_traps[i] < 1) {
            double prf_cash = prf_cashes[i];
            int stocks = (int)((prf_cash - broker_buy_fees(prf_cash)) / op);
            double cost = broker_buy(stocks, op);
            while (cost > prf_cash) {
              --stocks;
              cost = broker_buy(stocks, op);
            }
            prf_stocks[i] = stocks;
            prf_prices[i] = op;
            prf_cashes[i] = prf_cash - cost;
            arr_push(buys, exp_string(date));
          }
        } else {
          // Global simulation.
          int stocks = stockss[i];
          if (stocks > 0) {
            stockss[i] = 0;
            cash += broker_sell(stocks, op);
            arr_push(orders, mkOrder(date, nk, order_sell, stocks, op));

            if (op < prices[i] * no_loss_multiplicator) traps[i] = days_loss;
          }
          // Profits simulation.
          stocks = prf_stocks[i];
          if (stocks > 0) {
            prf_stocks[i] = 0;
            prf_cashes[i] += broker_sell(stocks, op);
            arr_push(sales, exp_string(date));

            if (op < prf_prices[i] * no_loss_multiplicator)
              prf_traps[i] = days_loss;
          }
        }
        to_dos[i] = FALSE;
      }

      int stocks = stockss[i];
      if (stocks > 0) assets += broker_sell(stocks, cl);

      if (to_sells[i]) {
        if (rf > cl) {
          to_dos[i] = TRUE;
          to_sells[i] = FALSE;
        }
      } else if (rf < cl) {
        to_dos[i] = TRUE;
        to_sells[i] = TRUE;
      }
    }_RANGE

    double total = cash + assets;
    if (total > withdrawal_limit) {
      double dif = total - initialCapital - bet;
      double secur_amount = min_to_bet - bet;
      double withdraw = -1;
      if (cash > dif + secur_amount) {
        withdraw = dif;
      } else if (cash < min_to_bet) {
        withdraw = floor((cash - secur_amount) / bet) * bet;
      }
      if (withdraw > 0) {
        withdrawals += withdraw;
        cash -= withdraw;
      }
    }

    arr_push(hassets, exp_float(cash + withdrawals + assets));
    arr_push(hwithdrawals, exp_float(withdrawals));
  }_REPEAT

  double *prf_ratios = mkFloats(ncos, bet);
  RANGE0(i, ncos) {
    prf_ratios[i] = (prf_cashes[i] + prf_stocks[i] * closes[i] - bet) / bet;
  }_RANGE

  Map *r = map_new();
  map_add(r, "Orders", exp_array(orders));
  map_add(r, "Hassets", exp_array(hassets));
  map_add(r, "Hwithdrawals", exp_array(hwithdrawals));
  map_add(r, "cash", exp_float(cash + withdrawals));
  map_add(r, "Buys", exp_array(arr_map(aallBuys, (FMAP)exp_array)));
  map_add(r, "Sales", exp_array(arr_map(aallSales, (FMAP)exp_array)));
  map_add(r, "Profits", mkFloatsArray(ncos, prf_ratios));

  return exp_dic(r);
}

// MODELS ----------------------------------------------------------------------

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_appr(Exp *exp) {
  // <Exp>
  Arr * params = exp_rget_array(exp);

  double start = exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 0)
  );
  double up_start = (1 + start);
  double low_start = (1 - start);
  double incr = exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 1)
  );

  // <vec>
  Arr *all_closes = exp_rget_array(arr_get(params, 0));
  Vec *closes_v = exp_rget_object("<vec>", arr_get(all_closes, 0));
  double *closes = closes_v->vs;
  int ncos = closes_v->size;

  int *is_solds = ATOMIC(sizeof(int) * ncos);
  double *refs = ATOMIC(sizeof(double) * ncos);
  RANGE0(i, ncos) {
    is_solds[i] = FALSE;
    refs[i] = closes[i] * low_start;
  }_RANGE

  // <Exp>
  Arr *allRefs = arr_new();
  arr_push(allRefs, pack_vec(ncos, refs));
  EACH(arr_drop(all_closes, 1), Exp, row) {
    Vec *closes_v = exp_rget_object("<vec>", row);
    double *closes = closes_v->vs;

    RANGE0(i, ncos) {
      double c = closes[i];
      double rf = refs[i];
      int is_sold = is_solds[i];

      if (is_sold) {
        if (c > rf) {
          is_solds[i] = FALSE;
          refs[i] = c * low_start;
        } else {
          refs[i] = rf - (rf - c) * incr;
        }
      } else {
        if (c < rf) {
          is_solds[i] = TRUE;
          refs[i] = c * up_start;
        } else {
          refs[i] = rf + (c - rf) * incr;
        }
      }
    }_RANGE

    arr_push(allRefs, pack_vec(ncos, refs));
  }_EACH

  return exp_array(allRefs);
}

// \{Closes:[<vec>.], Params:[f.]} -> <vec>
static Exp *calc_appr2(Exp *exp) {
  // <Exp>
  Arr * params = exp_rget_array(exp);

  double start = exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 0)
  );
  double up_start = (1 + start);
  double low_start = (1 - start);
  double incr = exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 1)
  );

  // <vec>
  Arr *all_closes = exp_rget_array(arr_get(params, 0));
  Vec *closes_v = exp_rget_object("<vec>", arr_get(all_closes, 0));
  double *closes = closes_v->vs;
  int ncos = closes_v->size;

  int *is_solds = ATOMIC(sizeof(int) * ncos);
  double *refs = ATOMIC(sizeof(double) * ncos);
  RANGE0(i, ncos) {
    is_solds[i] = FALSE;
    refs[i] = closes[i] * low_start;
  }_RANGE

  // <Exp>
  Arr *allRefs = arr_new();
  arr_push(allRefs, pack_vec(ncos, refs));
  EACH(arr_drop(all_closes, 1), Exp, row) {
    Vec *closes_v = exp_rget_object("<vec>", row);
    double *closes = closes_v->vs;

    RANGE0(i, ncos) {
      double c = closes[i];
      double rf = refs[i];
      int is_sold = is_solds[i];

      if (is_sold) {
        if (c > rf) {
          is_solds[i] = FALSE;
          refs[i] = c * low_start;
        } else {
          double r1 = rf - (rf - c) * incr;
          double r2 = c * up_start;
          if (r1 < r2) refs[i] = r1;
          else refs[i] = r2;
        }
      } else {
        if (c < rf) {
          is_solds[i] = TRUE;
          refs[i] = c * up_start;
        } else {
          double r1 = rf + (c - rf) * incr;
          double r2 = c * low_start;
          if (r1 > r2) refs[i] = r1;
          else refs[i] = r2;
        }
      }
    }_RANGE

    arr_push(allRefs, pack_vec(ncos, refs));
  }_EACH

  return exp_array(allRefs);
}

// \{Closes:[<vec>.], Params:[f.]} -> <vec>
static Exp *calc_appr3(Exp *exp) {
  // <Exp>
  Arr * params = exp_rget_array(exp);

  double start = exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 0)
  );
  double up_start = (1 + start);
  double low_start = (1 - start);
  double incr = exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 1)
  );

  // <vec>
  Arr *all_closes = exp_rget_array(arr_get(params, 0));
  Vec *closes_v = exp_rget_object("<vec>", arr_get(all_closes, 0));
  double *closes = closes_v->vs;
  int ncos = closes_v->size;

  int *is_solds = ATOMIC(sizeof(int) * ncos);
  double *refs = ATOMIC(sizeof(double) * ncos);
  RANGE0(i, ncos) {
    is_solds[i] = FALSE;
    refs[i] = closes[i] * low_start;
  }_RANGE

  // <Exp>
  Arr *allRefs = arr_new();
  arr_push(allRefs, pack_vec(ncos, refs));
  double *pcloses = closes;
  EACH(arr_drop(all_closes, 1), Exp, row) {
    Vec *closes_v = exp_rget_object("<vec>", row);
    double *closes = closes_v->vs;

    RANGE0(i, ncos) {
      double c = closes[i];
      double rf = refs[i];
      int is_sold = is_solds[i];

      if (is_sold) {
        if (c > rf) {
          is_solds[i] = FALSE;
          refs[i] = c * low_start;
        } else {
          double pc = pcloses[i];

          double r1 = rf - (rf - c) * incr;
          double r2 = c * rf / pc;
          if (r1 < r2) refs[i] = r1;
          else refs[i] = r2;
        }
      } else {
        if (c < rf) {
          is_solds[i] = TRUE;
          refs[i] = c * up_start;
        } else {
          double pc = pcloses[i];

          double r1 = rf + (c - rf) * incr;
          double r2 = c * rf / pc;
          if (r1 > r2) refs[i] = r1;
          else refs[i] = r2;
        }
      }
    }_RANGE

    arr_push(allRefs, pack_vec(ncos, refs));
    pcloses = closes;
  }_EACH

  return exp_array(allRefs);
}

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_ea(Exp *exp) {
  // <Exp>
  Arr * params = exp_rget_array(exp);

  int days = (int)(exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 0)
  ) + 0.4);
  double strip = exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 1)
  );
  double strip_up = 1 + strip;
  double strip_low = 1 - strip;
  int up_days = days + 1;

  // <vec>
  Arr *all_closes = exp_rget_array(arr_get(params, 0));
  Vec *closes_v = exp_rget_object("<vec>", arr_get(all_closes, 0));
  int ncos = closes_v->size;

  int *is_solds = ATOMIC(sizeof(int) * ncos);
  double *refs = ATOMIC(sizeof(double) * ncos);
  double *avgs = ATOMIC(sizeof(double) * ncos);
  RANGE0(i, ncos) {
    is_solds[i] = FALSE;
    refs[i] = 0;
    avgs[i] = 0;
  }_RANGE

  // <Exp>
  Arr *allRefs = arr_new();
  EACH(all_closes, Exp, row) {
    Vec *closes_v = exp_rget_object("<vec>", row);
    double *closes = closes_v->vs;

    RANGE0(i, ncos) {
      double c = closes[i];
      if (_i < days) {
        avgs[i] += c;
        refs[i] = c * strip_low;
      } else if (_i == days) {
        double avg = avgs[i] / days;
        double new_avg = avg + 2 * (c - avg) / up_days;
        avgs[i] = new_avg;

        if (c <= new_avg) {
          is_solds[i] = TRUE;
          refs[i] = new_avg * strip_up;
        } else {
          refs[i] = new_avg * strip_low;
        }
      } else {
        double rf = refs[i];
        double avg = avgs[i];
        double new_avg = avg + 2 * (c - avg) / up_days;
        avgs[i] = new_avg;

        if (is_solds[i]) {
          if (c > rf) {
            is_solds[i] = FALSE;
            refs[i] = new_avg * strip_low;
          } else {
            double new_rf = new_avg * strip_up;
            if (new_rf < rf) refs[i] = new_rf;
            else refs[i] = rf;
          }
        } else {
          if (c < rf) {
            is_solds[i] = TRUE;
            refs[i] = new_avg * strip_up;
          } else {
            double new_rf = new_avg * strip_low;
            if (new_rf > rf) refs[i] = new_rf;
            else refs[i] = rf;
          }
        }
      }
    }_RANGE

    arr_push(allRefs, pack_vec(ncos, refs));
  }_EACH

  return exp_array(allRefs);
}

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_ea2(Exp *exp) {
  // <Exp>
  Arr * params = exp_rget_array(exp);

  int days = (int)(exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 0)
  ) + 0.4);
  double strip = exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 1)
  );
  double strip_up = 1 + strip;
  double strip_low = 1 - strip;

  // <vec>
  Arr *all_closes = exp_rget_array(arr_get(params, 0));
  Vec *closes_v = exp_rget_object("<vec>", arr_get(all_closes, 0));
  int ncos = closes_v->size;

  int *is_solds = ATOMIC(sizeof(int) * ncos);
  double *refs = ATOMIC(sizeof(double) * ncos);
  double *avgs = ATOMIC(sizeof(double) * ncos);
  RANGE0(i, ncos) {
    is_solds[i] = FALSE;
    refs[i] = 0;
    avgs[i] = 0;
  }_RANGE

  // <Exp>
  Arr *allRefs = arr_new();
  EACH(all_closes, Exp, row) {
    Vec *closes_v = exp_rget_object("<vec>", row);
    double *closes = closes_v->vs;

    RANGE0(i, ncos) {
      double c = closes[i];
      if (_i < days) {
        avgs[i] += c;
        refs[i] = c * (1 - strip);
      } else if (_i == days) {
        double avg = avgs[i] / days;
        double new_avg = avg + (c - avg) / days;
        avgs[i] = new_avg;

        if (c <= new_avg) {
          is_solds[i] = TRUE;
          refs[i] = new_avg * strip_up;
        } else {
          refs[i] = new_avg * strip_low;
        }
      } else {
        double rf = refs[i];
        double avg = avgs[i];
        double new_avg = avg + (c - avg) / days;
        avgs[i] = new_avg;

        if (is_solds[i]) {
          if (c > rf) {
            is_solds[i] = FALSE;
            refs[i] = new_avg * strip_low;
          } else {
            double new_rf = new_avg * strip_up;
            if (new_rf < rf) refs[i] = new_rf;
            else refs[i] = rf;
          }
        } else {
          if (c < rf) {
            is_solds[i] = TRUE;
            refs[i] = new_avg * strip_up;
          } else {
            double new_rf = new_avg * strip_low;
            if (new_rf > rf) refs[i] = new_rf;
            else refs[i] = rf;
          }
        }
      }
    }_RANGE

    arr_push(allRefs, pack_vec(ncos, refs));
  }_EACH

  return exp_array(allRefs);
}

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_ma(Exp *exp) {
  // <Exp>
  Arr * params = exp_rget_array(exp);

  int days = (int)(exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 0)
  ) + 0.4);
  double strip = exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 1)
  );
  double strip_up = 1 + strip;
  double strip_low = 1 - strip;

  // <vec>
  Arr *all_closes = exp_rget_array(arr_get(params, 0));
  Vec *closes_v = exp_rget_object("<vec>", arr_get(all_closes, 0));
  int ncos = closes_v->size;

  int *is_solds = ATOMIC(sizeof(int) * ncos);
  double *refs = ATOMIC(sizeof(double) * ncos);
  double *sums = ATOMIC(sizeof(double) * ncos);
  RANGE0(i, ncos) {
    is_solds[i] = FALSE;
    refs[i] = 0;
    sums[i] = 0;
  }_RANGE

  // <Exp>
  Arr *allRefs = arr_new();
  EACH(all_closes, Exp, row) {
    Vec *closes_v = exp_rget_object("<vec>", row);
    double *closes = closes_v->vs;
    double *old_closes = closes;
    if (_i >= days) {
      Vec *old_v = exp_rget_object("<vec>", arr_get(all_closes, _i - days));
      old_closes = old_v->vs;
    }

    RANGE0(i, ncos) {
      double c = closes[i];
      if (_i < days) {
        sums[i] += c;
        refs[i] = c * strip_low;
      } else if (_i == days) {
        sums[i] += c - old_closes[i];
        double avg = sums[i] / days;

        if (c <= avg) {
          is_solds[i] = TRUE;
          refs[i] = avg * strip_up;
        } else {
          refs[i] = avg * strip_low;
        }
      } else {
        double rf = refs[i];
        sums[i] += c - old_closes[i];
        double avg = sums[i] / days;

        if (is_solds[i]) {
          if (c > rf) {
            is_solds[i] = FALSE;
            refs[i] = avg * strip_low;
          } else {
            double new_rf = avg * strip_up;
            if (new_rf < rf) refs[i] = new_rf;
            else refs[i] = rf;
          }
        } else {
          if (c < rf) {
            is_solds[i] = TRUE;
            refs[i] = avg * strip_up;
          } else {
            double new_rf = avg * strip_low;
            if (new_rf > rf) refs[i] = new_rf;
            else refs[i] = rf;
          }
        }
      }
    }_RANGE

    arr_push(allRefs, pack_vec(ncos, refs));
  }_EACH

  return exp_array(allRefs);
}

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_mm(Exp *exp) {
  // <Exp>
  Arr * params = exp_rget_array(exp);

  int days = (int)(exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 0)
  ) + 0.4);
  double strip = exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 1)
  );
  double strip_up = 1 + strip;
  double strip_low = 1 - strip;

  // <vec>
  Arr *all_closes = exp_rget_array(arr_get(params, 0));
  Vec *closes_v = exp_rget_object("<vec>", arr_get(all_closes, 0));
  int ncos = closes_v->size;

  int *is_solds = ATOMIC(sizeof(int) * ncos);
  double *refs = ATOMIC(sizeof(double) * ncos);
  double *sums = ATOMIC(sizeof(double) * ncos);
  RANGE0(i, ncos) {
    is_solds[i] = FALSE;
    refs[i] = 0;
    sums[i] = 0;
  }_RANGE

  // <Exp>
  Arr *allRefs = arr_new();
  EACH(all_closes, Exp, row) {
    Vec *closes_v = exp_rget_object("<vec>", row);
    double *closes = closes_v->vs;
    double *old_closes = closes;
    if (_i >= days) {
      Vec *old_v = exp_rget_object("<vec>", arr_get(all_closes, _i - days));
      old_closes = old_v->vs;
    }

    RANGE0(i, ncos) {
      double c = closes[i];
      if (_i < days) {
        refs[i] = c * strip_low;
      } else if (_i == days) {
        double old_c = old_closes[i];

        if (c <= old_c) {
          is_solds[i] = TRUE;
          refs[i] = old_c * strip_up;
        } else {
          refs[i] = old_c * strip_low;
        }
      } else {
        double rf = refs[i];
        double old_c = old_closes[i];

        if (is_solds[i]) {
          if (c > rf) {
            is_solds[i] = FALSE;
            refs[i] = old_c * strip_low;
          } else {
            double new_rf = old_c * strip_up;
            if (new_rf < rf) refs[i] = new_rf;
            else refs[i] = rf;
          }
        } else {
          if (c < rf) {
            is_solds[i] = TRUE;
            refs[i] = old_c * strip_up;
          } else {
            double new_rf = old_c * strip_low;
            if (new_rf > rf) refs[i] = new_rf;
            else refs[i] = rf;
          }
        }
      }
    }_RANGE

    arr_push(allRefs, pack_vec(ncos, refs));
  }_EACH

  return exp_array(allRefs);
}

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_qfix(Exp *exp) {
  // <Exp>
  Arr * params = exp_rget_array(exp);

  double jmp = exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 0)
  ) + 1;
  double lg_jmp = log(jmp);

  // FUNCTIONS -------------------------

  double down_gap (double q) {
    return pow(jmp, round(log(q)/lg_jmp) - 1);
  }

  double up_gap (double q) {
    return pow(jmp, round(log(q)/lg_jmp) + 1);
  }

  double down_gap2 (double q, double ref) {
    double ref2 = ref * jmp;
    return ref2 * sqrt(jmp) >= q ? ref : down_gap2(q, ref2);
  }

  double up_gap2 (double q, double ref) {
    double ref2 = ref / jmp;
    return ref2 / sqrt(jmp) <= q ? ref : up_gap2(q, ref2);
  }

  // END FUNCTIONS ---------------------

  // <vec>
  Arr *all_closes = exp_rget_array(arr_get(params, 0));
  Vec *closes_v = exp_rget_object("<vec>", arr_get(all_closes, 0));
  double *closes = closes_v->vs;
  int ncos = closes_v->size;

  double *refs = ATOMIC(sizeof(double) * ncos);
  RANGE0(i, ncos) {
    refs[i] = pow(jmp, round(log(closes[i])/lg_jmp) - 1) / jmp;
  }_RANGE


  // <Exp>
  Arr *allRefs = arr_new();
  double *prv_closes = closes;
  arr_push(allRefs, pack_vec(ncos, refs));
  EACH(arr_drop(all_closes, 1), Exp, row) {
    Vec *closes_v = exp_rget_object("<vec>", row);
    double *closes = closes_v->vs;

    RANGE0(i, ncos) {
      double q0 = prv_closes[i];
      double ref = refs[i];
      double q = closes[i];

      if (q0 <= ref) {
        if (q < q0) {
          refs[i] = up_gap2(q, ref);
        } else if (q > ref) {
          refs[i] = down_gap(q);
        }
      } else {
        if (q > q0) {
          refs[i] = down_gap2(q, ref);
        } else if (q <= ref) {
          refs[i] = up_gap(q);
        }
      }
    }_RANGE

    prv_closes = closes;
    arr_push(allRefs, pack_vec(ncos, refs));
  }_EACH

  return exp_array(allRefs);
}

// \[[<vec>...], Params:[f...]] -> [<vec>.]
static Exp *calc_qmob(Exp *exp) {
  // <Exp>
  Arr * params = exp_rget_array(exp);

  double gap = exp_rget_float(
    arr_get(exp_rget_array(arr_get(params, 1)), 0)
  );
  double up_gap = 1 + gap;
  double down_gap = 1 - gap;

  // <vec>
  Arr *all_closes = exp_rget_array(arr_get(params, 0));
  Vec *closes_v = exp_rget_object("<vec>", arr_get(all_closes, 0));
  double *closes = closes_v->vs;
  int ncos = closes_v->size;

  int *is_solds = ATOMIC(sizeof(int) * ncos);
  double *refs = ATOMIC(sizeof(double) * ncos);
  RANGE0(i, ncos) {
    is_solds[i] = FALSE;
    refs[i] = closes[i] * down_gap;
  }_RANGE

  // <Exp>
  Arr *allRefs = arr_new();
  arr_push(allRefs, pack_vec(ncos, refs));
  EACH(arr_drop(all_closes, 1), Exp, row) {
    Vec *closes_v = exp_rget_object("<vec>", row);
    double *closes = closes_v->vs;

    RANGE0(i, ncos) {
      double c = closes[i];
      double rf = refs[i];
      int is_sold = is_solds[i];

      if (is_sold) {
        if (c > rf) {
          is_solds[i] = FALSE;
          refs[i] = c * down_gap;
        } else {
          double new_rf = c * up_gap;
          if (new_rf > rf) new_rf = rf;
          refs[i] = new_rf;
        }
      } else {
        if (c < rf) {
          is_solds[i] = TRUE;
          refs[i] = c * up_gap;
        } else {
          double new_rf = c * down_gap;
          if (new_rf < rf) new_rf = rf;
          refs[i] = new_rf;
        }
      }
    }_RANGE

    arr_push(allRefs, pack_vec(ncos, refs));
  }_EACH

  return exp_array(allRefs);
}

Exp *exmodule_run (char *fn, Exp *data) {
  // CONSTANTS

  if (!strcmp(fn, "bet")) return exp_float(bet);
  if (!strcmp(fn, "initialCapital")) return exp_float(initialCapital);
  if (!strcmp(fn, "minToBet")) return exp_float(min_to_bet);
  if (!strcmp(fn, "daysLoss")) return exp_float(days_loss);
  if (!strcmp(fn, "noLossMultiplicator")) return exp_float(no_loss_multiplicator);
  if (!strcmp(fn, "withdrawalLimit")) return exp_float(withdrawal_limit);

  // FUNCTIONS

  if (!strcmp(fn, "brokerBuy")) return broker_buy_f(data);
  if (!strcmp(fn, "brokerBuyFees")) return broker_buy_fees_f(data);
  if (!strcmp(fn, "brokerSell")) return broker_sell_f(data);
  if (!strcmp(fn, "brokerSellFees")) return broker_sell_fees_f(data);
  if (!strcmp(fn, "calcAppr")) return calc_appr(data);
  if (!strcmp(fn, "calcAppr2")) return calc_appr2(data);
  if (!strcmp(fn, "calcAppr3")) return calc_appr3(data);
  if (!strcmp(fn, "calcEa")) return calc_ea(data);
  if (!strcmp(fn, "calcEa2")) return calc_ea2(data);
  if (!strcmp(fn, "calcMa")) return calc_ma(data);
  if (!strcmp(fn, "calcMm")) return calc_mm(data);
  if (!strcmp(fn, "calcQfix")) return calc_qfix(data);
  if (!strcmp(fn, "calcQmob")) return calc_qmob(data);
  if (!strcmp(fn, "mkVec")) return mk_vec(data);
  if (!strcmp(fn, "qsFromStr")) return qs_from_str(data);
  if (!strcmp(fn, "qsVol")) return qs_vol(data);
  if (!strcmp(fn, "quoteFromStr")) return quote_from_str(data);
  if (!strcmp(fn, "strategyRun")) return strategy_run(data);
  if (!strcmp(fn, "vecToArr")) return vec_to_arr(data);

  EXC_KUT(str_f("Extern 'c' function '%s' not found", fn));
  return NULL; // Unreachable
}
