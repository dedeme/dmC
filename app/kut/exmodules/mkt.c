// Copyright 04-Jul-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "kut/dec.h"
#include "exmodule.h"
#include "math.h"

// GENERAL ---------------------------------------------------------------------

#define initialCapital 250000
#define min_to_bet 11000.0
#define bet 10000
//#define days_win 25 // 5 weeks
//#define days_loss 90 // 18 weeks
#define no_loss_multiplicator 1.02 // 'sell >= buy * no_loss_multiplicator' is ok.
static double withdrawalLimit = initialCapital + bet + bet;
#define order_buy 0
#define order_sell 1

typedef struct exmodule_Vec {
  int size;
  double *vs;
} Vec;

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

// \[d, <vec>, <vec>] -> ()
static Exp *strategy_run(Exp *exp) {
  Exp **a = (Exp**)arr_begin(exp_rget_array(exp));
  // <Exp>
  Map *env = exp_rget_map(a[0]);
  int days_win = exp_rget_int(opt_get(map_get(env, "daysWin")));
  int days_loss = exp_rget_int(opt_get(map_get(env, "daysLoss")));
  double *cls = ((Vec *)exp_rget_object("<vec>", a[1]))->vs;
  Exp *rfs_ex = a[2];
  Vec *rfs_v = exp_rget_object("<vec>", rfs_ex);
  double *rfs = rfs_v->vs;
  int cols = rfs_v->size;

  Exp *env2 = opt_get(map_get(env, "env"));
  if (!env2) {
    int *prow = ATOMIC(sizeof(int));
    *prow = 0;
    int *to_dos = ATOMIC(sizeof(int) * cols);
    RANGE0(i, cols) { to_dos[i] = FALSE; }_RANGE;
    int *to_sells = ATOMIC(sizeof(int) * cols);
    RANGE0(i, cols) { to_sells[i] = TRUE; }_RANGE;
    double *prices = ATOMIC(sizeof(double) * cols);
    int *stockss = ATOMIC(sizeof(int) * cols);
    RANGE0(i, cols) { stockss[i] = 0; }_RANGE;
    int *traps = ATOMIC(sizeof(int) * cols);
    RANGE0(i, cols) { traps[i] = 0; }_RANGE;
    double *prf_cashs = ATOMIC(sizeof(double) * cols);
    RANGE0(i, cols) { prf_cashs[i] = 0; }_RANGE;
    double *prf_prices = ATOMIC(sizeof(double) * cols);
    int *prf_stocks = ATOMIC(sizeof(int) * cols);
    RANGE0(i, cols) { prf_stocks[i] = 0; }_RANGE;
    int *prf_traps = ATOMIC(sizeof(int) * cols);
    RANGE0(i, cols) { prf_traps[i] = 0; }_RANGE;

    env2 = exp_array(arr_new_from(
      prow, // 0
      arr_begin(exp_rget_array(opt_get(map_get(env, "Refs")))), // 1
      arr_begin(exp_rget_array(opt_get(map_get(env, "Dates")))), // 2
      arr_begin(exp_rget_array(opt_get(map_get(env, "Opens")))), // 3
      exp_rget_array(opt_get(map_get(env, "Nks"))), // 4
      to_dos, //5
      to_sells, // 6
      traps, // 7
      arr_begin(exp_rget_array(opt_get(map_get(env, "CashIn")))), // 8
      stockss, // 9
      prices, // 10
      exp_rget_array(opt_get(map_get(env, "Orders"))), // 11
      arr_begin(exp_rget_array(opt_get(map_get(env, "PrfCashs")))), // 12
      prf_prices, // 13
      arr_begin(exp_rget_array(opt_get(map_get(env, "PrfStocks")))), // 14
      prf_traps, // 15
      exp_rget_array(opt_get(map_get(env, "Buys"))), // 16
      exp_rget_array(opt_get(map_get(env, "Sales"))), // 17
      arr_begin(exp_rget_array(opt_get(map_get(env, "Withdrawals")))), // 18
      arr_begin(exp_rget_array(opt_get(map_get(env, "Hassets")))), // 19
      arr_begin(exp_rget_array(opt_get(map_get(env, "Hwithdrawals")))), // 20
      NULL
    ));
    map_add(env, "env", env2);
  }
  void **envs = arr_begin(exp_rget_array(env2));

  // FUNCTIONS -----

  Exp *mkOrder(char *date, char *nick, int type, int stocks, double price) {
    // <Exp>
    Map *value = map_new();
    map_add(value, "date", exp_string(date));
    map_add(value, "nick", exp_string(nick));
    map_add(value, "type", exp_int(type));
    map_add(value, "stocks", exp_int(stocks));
    map_add(value, "price", exp_float(price));
    return exp_map(value);
  }

  double broker_buy_fees(double amount) {
    return (amount > 50000      // broker
        ? amount * 0.001
        : 9.75
      ) +
      amount * 0.00003 +       // market
      0.11 +                   // Execution fee
      amount * 0.002           // tobin
    ;
  }

  double broker_buy (int stocks, double price) {
    double amount = stocks * price;
    return amount + broker_buy_fees(amount);
  }

  double broker_sell_fees(double amount) {
    return (amount > 50000      // broker
        ? amount * 0.001
        : 9.75
      ) +
      amount * 0.00003 +       // market
      0.11                     // Execution fee
    ;
  }

  double broker_sell (int stocks, double price) {
    double amount = stocks * price;
    return amount - broker_sell_fees(amount);
  }

  // PROCESS -----

  int *prow = envs[0];
  int row = *prow;
  *prow = row + 1;

  // [f...]
  Exp **refs = envs[1];
  refs[row] = vec_to_arr(rfs_ex);

  Exp **date_ex = envs[2];
  char *date = exp_rget_string(date_ex[row]);
  Exp **os_ex = envs[3];
  double *os = ((Vec *)exp_rget_object("<vec>", os_ex[row]))->vs;

  double assets = 0;
  char *nks = envs[4];
  int *to_dos = envs[5];
  int *to_sells = envs[6];
  int *traps = envs[7];
  // f
  Exp **cash_in = envs[8];
  int *stockss = envs[9];
  double *prices = envs[10];
  // <Exp><order>
  Arr *orders = envs[11];
  // f
  Exp **prf_cashs = envs[12];
  double *prf_prices = envs[13];
  // i
  Exp **prf_stocks = envs[14];
  int *prf_traps = envs[15];
  // <Exp> [s...]
  Arr *abuys = envs[16];
  // <Exp> [s...]
  Arr *asales = envs[17];
  // f
  Exp **withdrawals = envs[18];
  // f
  Exp **hassets = envs[19];
  // f
  Exp **hwithdrawals = envs[20];

  EACH(nks, Exp, enk) {
    char *nk = exp_rget_string(enk);
    double op = os[_i];
    double cl = cls[_i];
    double rf = rfs[_i];
    // <Exp> s
    Arr *buys = exp_rget_array(arr_get(abuys, _i));
    // <Exp> s
    Arr *sales = exp_rget_array(arr_get(asales, _i));

    --traps[_i];
    --prf_traps[_i];
    if (to_dos[_i]) {
      if (to_sells[_i]) { // there is buy order.
        if (traps[_i] < 1) {
          double cash = exp_rget_float(cash_in[0]);
          if (cash > min_to_bet) {
            int stocks = (int)(bet / op);
            stockss[_i] = stocks;
            prices[_i] = op;
            cash -= broker_buy(stocks, op);
            cash_in[0] = exp_float(cash);
            arr_push(orders, mkOrder(date, nk, order_buy, stocks, op));
          }
        }

        if (prf_traps[_i] < 1) {
          double prf_cash = exp_rget_float(prf_cashs[_i]);
          int stocks = (int)((prf_cash - broker_buy_fees(prf_cash)) / op);
          double cost = broker_buy(stocks, op);
          while (cost > prf_cash) {
            --stocks;
            cost = broker_buy(stocks, op);
          }
          prf_stocks[_i] = exp_int(stocks);
          prf_prices[_i] = op;
          prf_cashs[_i] = exp_float(prf_cash - cost);
          arr_push(buys, exp_string(date));
        }
      } else {
        int stocks = stockss[_i];
        if (stocks > 0) {
          stockss[_i] = 0;
          double cash = exp_rget_float(cash_in[0]);
          cash += broker_sell(stocks, op);
          cash_in[0] = exp_float(cash);
          arr_push(orders, mkOrder(date, nk, order_sell, stocks, op));

          if (op < prices[_i] * no_loss_multiplicator)
            traps[_i] = days_loss;
          else
            traps[_i] = days_win;
        }

        stocks = exp_rget_int(prf_stocks[_i]);
        if (stocks > 0) {
          prf_stocks[_i] = exp_int(0);
          double cash = exp_rget_float(prf_cashs[_i]);
          prf_cashs[_i] = exp_float(cash + broker_sell(stocks, op));
          arr_push(sales, exp_string(date));

          if (op < prf_prices[_i] * no_loss_multiplicator)
            prf_traps[_i] = days_loss;
          else
            traps[_i] = days_win;
        }
      }
      to_dos[_i] = FALSE;
    }

    int stocks = stockss[_i];
    if (stocks > 0) {
      assets += broker_sell(stocks, cl);
    }

    if (to_sells[_i]) {
      if (rf > cl) {
        to_dos[_i] = TRUE;
        to_sells[_i] = FALSE;
      }
    } else if (rf < cl) {
      to_dos[_i] = TRUE;
      to_sells[_i] = TRUE;
    }
  }_EACH

  double cash = exp_rget_float(cash_in[0]);
  double w = exp_rget_float(withdrawals[0]);
  double total = cash + assets;
  if (total > withdrawalLimit) {
    double dif = total - initialCapital - bet;
    double secur_amount = min_to_bet - bet;
    double withdraw = -1;
    if (cash > dif + secur_amount) {
      withdraw = dif;
    } else if (cash < min_to_bet) {
      withdraw = floor((cash - secur_amount) / bet) * bet;
    }
    if (withdraw > 0) {
      w += withdraw;
      cash -= withdraw;
      withdrawals[0] = exp_float(w);
      cash_in[0] = exp_float(cash);
    }
  }

  hassets[row] = exp_float(cash + w + assets);
  hwithdrawals[row] = exp_float(w);

  return exp_empty();
}

// MODELS ----------------------------------------------------------------------

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_appr(Exp *exp) {
  // <Exp>
  Map *env = exp_rget_map(exp);
  Exp *env2 = opt_get(map_get(env, "env"));
  if (!env2) {
    double start = exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 0)
    );
    double incr = exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 1)
    );

    // <vec>
    Exp **all_closes =
      (Exp **)arr_begin(exp_rget_array(opt_get(map_get(env, "Closes"))));
    Vec *closes_v = exp_rget_object("<vec>", all_closes[0]);
    double *closes = closes_v->vs;
    int cos = closes_v->size;

    int *is_solds = ATOMIC(sizeof(int) * cos);
    double *refs = ATOMIC(sizeof(double) * cos);
    RANGE0(i, cos) {
      is_solds[i] = FALSE;
      refs[i] = closes[i] * (1 - start);
    }_RANGE

    double *pstart = ATOMIC(sizeof(double));
    *pstart = start;
    double *pincr = ATOMIC(sizeof(double));
    *pincr = incr;
    int *row = ATOMIC(sizeof(int));
    *row = 1;

    env2 = exp_array(arr_new_from(
      pstart, // 0
      pincr, // 1
      row, // 2
      is_solds, // 3
      refs, // 4
      all_closes, // 5
      NULL
    ));
    map_add(env, "env", env2);

    Vec *refs_v = MALLOC(Vec);
    refs_v->size = cos;
    refs_v->vs = refs;
    return exp_object("<vec>", refs_v);
  }
  void **envs = arr_begin(exp_rget_array(env2));

  // FUNCTIONS -----

  // PROCESS -----

  double start = *((double *)envs[0]);
  double incr = *((double *)envs[1]);
  int *prow = envs[2];
  int row = *prow;
  *prow = row + 1;
  int *is_solds = envs[3];
  double *refs = envs[4];
  Exp **all_closes = envs[5];
  Vec *closes_v = exp_rget_object("<vec>", all_closes[row]);
  int cos = closes_v->size;
  double *closes = closes_v->vs;

  RANGE0(i, cos) {
    double c = closes[i];
    double rf = refs[i];
    int is_sold = is_solds[i];

    if (is_sold) {
      if (c > rf) {
        is_solds[i] = FALSE;
        refs[i] = c * (1 - start);
      } else {
        refs[i] = rf - (rf - c) * incr;
      }
    } else {
      if (c <= rf) {
        is_solds[i] = TRUE;
        refs[i] = c * (1 + start);
      } else {
        refs[i] = rf + (c - rf) * incr;
      }
    }
  }_RANGE

  Vec *refs_v = MALLOC(Vec);
  refs_v->size = cos;
  refs_v->vs = refs;
  return exp_object("<vec>", refs_v);
}

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_appr2(Exp *exp) {
  // <Exp>
  Map *env = exp_rget_map(exp);
  Exp *env2 = opt_get(map_get(env, "env"));
  if (!env2) {
    double start = exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 0)
    );
    double incr = exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 1)
    );

    // <vec>
    Exp **all_closes =
      (Exp **)arr_begin(exp_rget_array(opt_get(map_get(env, "Closes"))));
    Vec *closes_v = exp_rget_object("<vec>", all_closes[0]);
    double *closes = closes_v->vs;
    int cos = closes_v->size;

    int *is_solds = ATOMIC(sizeof(int) * cos);
    double *refs = ATOMIC(sizeof(double) * cos);
    RANGE0(i, cos) {
      is_solds[i] = FALSE;
      refs[i] = closes[i] * (1 - start);
    }_RANGE

    double *pstart = ATOMIC(sizeof(double));
    *pstart = start;
    double *pincr = ATOMIC(sizeof(double));
    *pincr = incr;
    int *row = ATOMIC(sizeof(int));
    *row = 1;

    env2 = exp_array(arr_new_from(
      pstart, // 0
      pincr, // 1
      row, // 2
      is_solds, // 3
      refs, // 4
      all_closes, // 5
      NULL
    ));
    map_add(env, "env", env2);

    Vec *refs_v = MALLOC(Vec);
    refs_v->size = cos;
    refs_v->vs = refs;
    return exp_object("<vec>", refs_v);
  }
  void **envs = arr_begin(exp_rget_array(env2));

  // FUNCTIONS -----

  // PROCESS -----

  double start = *((double *)envs[0]);
  double incr = *((double *)envs[1]);
  int *prow = envs[2];
  int row = *prow;
  *prow = row + 1;
  int *is_solds = envs[3];
  double *refs = envs[4];
  Exp **all_closes = envs[5];
  Vec *closes_v = exp_rget_object("<vec>", all_closes[row]);
  int cos = closes_v->size;
  double *closes = closes_v->vs;

  RANGE0(i, cos) {
    double c = closes[i];
    double rf = refs[i];
    int is_sold = is_solds[i];

    if (is_sold) {
      if (c > rf) {
        is_solds[i] = FALSE;
        refs[i] = c * (1 - start);
      } else {
        double r1 = rf - (rf - c) * incr;
        double r2 = c * (1 + start);
        if (r1 < r2) refs[i] = r1;
        else refs[i] = r2;
      }
    } else {
      if (c <= rf) {
        is_solds[i] = TRUE;
        refs[i] = c * (1 + start);
      } else {
        double r1 = rf + (c - rf) * incr;
        double r2 = c * (1 - start);
        if (r1 > r2) refs[i] = r1;
        else refs[i] = r2;
      }
    }
  }_RANGE

  Vec *refs_v = MALLOC(Vec);
  refs_v->size = cos;
  refs_v->vs = refs;
  return exp_object("<vec>", refs_v);
}

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_ea(Exp *exp) {
  // <Exp>
  Map *env = exp_rget_map(exp);
  Exp *env2 = opt_get(map_get(env, "env"));
  if (!env2) {
    int days = (int)(exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 0)
    ) + 0.4);
    double strip = exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 1)
    );

    // <vec>
    Exp **all_closes =
      (Exp **)arr_begin(exp_rget_array(opt_get(map_get(env, "Closes"))));
    Vec *closes_v = exp_rget_object("<vec>", all_closes[0]);
    double *closes = closes_v->vs;
    int cos = closes_v->size;

    int *is_solds = ATOMIC(sizeof(int) * cos);
    double *avgs = ATOMIC(sizeof(double) * cos);
    double *refs = ATOMIC(sizeof(double) * cos);
    RANGE0(i, cos) {
      is_solds[i] = FALSE;
      avgs[i] = closes[i];
      refs[i] = closes[i] * (1 - strip);
    }_RANGE

    int *pdays = ATOMIC(sizeof(int));
    *pdays = days;
    double *pstrip = ATOMIC(sizeof(double));
    *pstrip = strip;
    int *row = ATOMIC(sizeof(int));
    *row = 1;

    env2 = exp_array(arr_new_from(
      pdays, // 0
      pstrip, // 1
      row, // 2
      is_solds, // 3
      avgs, // 4
      refs, // 5
      all_closes, // 6
      NULL
    ));
    map_add(env, "env", env2);

    Vec *refs_v = MALLOC(Vec);
    refs_v->size = cos;
    refs_v->vs = refs;
    return exp_object("<vec>", refs_v);
  }
  void **envs = arr_begin(exp_rget_array(env2));

  // FUNCTIONS -----

  // PROCESS -----

  int days = *((int *)envs[0]);
  double strip = *((double *)envs[1]);
  int *prow = envs[2];
  int row = *prow;
  *prow = row + 1;
  int *is_solds = envs[3];
  double *avgs = envs[4];
  double *refs = envs[5];
  Exp **all_closes = envs[6];
  Vec *closes_v = exp_rget_object("<vec>", all_closes[row]);
  int cos = closes_v->size;
  double *closes = closes_v->vs;

  RANGE0(i, cos) {
    double c = closes[i];
    if (row < days) {
      avgs[i] += c;
      refs[i] = c * (1 - strip);
    } else if (row == days) {
      double avg = avgs[i] / days;
      double new_avg = avg + 2 * (c - avg) / (days + 1);
      avgs[i] = new_avg;

      if (c <= new_avg) {
        is_solds[i] = TRUE;
        refs[i] = new_avg * (1 + strip);
      } else {
        refs[i] = new_avg * (1 - strip);
      }
    } else {
      double rf = refs[i];
      double avg = avgs[i];
      double new_avg = avg + 2 * (c - avg) / (days + 1);
      avgs[i] = new_avg;

      if (is_solds[i]) {
        if (c > rf) {
          is_solds[i] = FALSE;
          refs[i] = new_avg * (1 - strip);
        } else {
          double new_rf = new_avg * (1 + strip);
          if (new_rf < rf) refs[i] = new_rf;
          else refs[i] = rf;
        }
      } else {
        if (c <= rf) {
          is_solds[i] = TRUE;
          refs[i] = new_avg * (1 + strip);
        } else {
          double new_rf = new_avg * (1 - strip);
          if (new_rf > rf) refs[i] = new_rf;
          else refs[i] = rf;
        }
      }
    }
  }_RANGE

  Vec *refs_v = MALLOC(Vec);
  refs_v->size = cos;
  refs_v->vs = refs;
  return exp_object("<vec>", refs_v);
}

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_ea2(Exp *exp) {
  // <Exp>
  Map *env = exp_rget_map(exp);
  Exp *env2 = opt_get(map_get(env, "env"));
  if (!env2) {
    int days = (int)(exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 0)
    ) + 0.4);
    double strip = exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 1)
    );

    // <vec>
    Exp **all_closes =
      (Exp **)arr_begin(exp_rget_array(opt_get(map_get(env, "Closes"))));
    Vec *closes_v = exp_rget_object("<vec>", all_closes[0]);
    double *closes = closes_v->vs;
    int cos = closes_v->size;

    int *is_solds = ATOMIC(sizeof(int) * cos);
    double *avgs = ATOMIC(sizeof(double) * cos);
    double *refs = ATOMIC(sizeof(double) * cos);
    RANGE0(i, cos) {
      is_solds[i] = FALSE;
      avgs[i] = closes[i];
      refs[i] = closes[i] * (1 - strip);
    }_RANGE

    int *pdays = ATOMIC(sizeof(int));
    *pdays = days;
    double *pstrip = ATOMIC(sizeof(double));
    *pstrip = strip;
    int *row = ATOMIC(sizeof(int));
    *row = 1;

    env2 = exp_array(arr_new_from(
      pdays, // 0
      pstrip, // 1
      row, // 2
      is_solds, // 3
      avgs, // 4
      refs, // 5
      all_closes, // 6
      NULL
    ));
    map_add(env, "env", env2);

    Vec *refs_v = MALLOC(Vec);
    refs_v->size = cos;
    refs_v->vs = refs;
    return exp_object("<vec>", refs_v);
  }
  void **envs = arr_begin(exp_rget_array(env2));

  // FUNCTIONS -----

  // PROCESS -----

  int days = *((int *)envs[0]);
  double strip = *((double *)envs[1]);
  int *prow = envs[2];
  int row = *prow;
  *prow = row + 1;
  int *is_solds = envs[3];
  double *avgs = envs[4];
  double *refs = envs[5];
  Exp **all_closes = envs[6];
  Vec *closes_v = exp_rget_object("<vec>", all_closes[row]);
  int cos = closes_v->size;
  double *closes = closes_v->vs;

  RANGE0(i, cos) {
    double c = closes[i];
    if (row < days) {
      avgs[i] += c;
      refs[i] = c * (1 - strip);
    } else if (row == days) {
      double avg = avgs[i] / days;
      double new_avg = avg + (c - avg) / days;
      avgs[i] = new_avg;

      if (c <= new_avg) {
        is_solds[i] = TRUE;
        refs[i] = new_avg * (1 + strip);
      } else {
        refs[i] = new_avg * (1 - strip);
      }
    } else {
      double rf = refs[i];
      double avg = avgs[i];
      double new_avg = avg + (c - avg) / days;
      avgs[i] = new_avg;

      if (is_solds[i]) {
        if (c > rf) {
          is_solds[i] = FALSE;
          refs[i] = new_avg * (1 - strip);
        } else {
          double new_rf = new_avg * (1 + strip);
          if (new_rf < rf) refs[i] = new_rf;
          else refs[i] = rf;
        }
      } else {
        if (c <= rf) {
          is_solds[i] = TRUE;
          refs[i] = new_avg * (1 + strip);
        } else {
          double new_rf = new_avg * (1 - strip);
          if (new_rf > rf) refs[i] = new_rf;
          else refs[i] = rf;
        }
      }
    }
  }_RANGE

  Vec *refs_v = MALLOC(Vec);
  refs_v->size = cos;
  refs_v->vs = refs;
  return exp_object("<vec>", refs_v);
}

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_ma(Exp *exp) {
  // <Exp>
  Map *env = exp_rget_map(exp);
  Exp *env2 = opt_get(map_get(env, "env"));
  if (!env2) {
    int days = (int)(exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 0)
    ) + 0.4);
    double strip = exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 1)
    );

    // <vec>
    Exp **all_closes =
      (Exp **)arr_begin(exp_rget_array(opt_get(map_get(env, "Closes"))));
    Vec *closes_v = exp_rget_object("<vec>", all_closes[0]);
    double *closes = closes_v->vs;
    int cos = closes_v->size;

    int *is_solds = ATOMIC(sizeof(int) * cos);
    double *sums = ATOMIC(sizeof(double) * cos);
    double *refs = ATOMIC(sizeof(double) * cos);
    RANGE0(i, cos) {
      is_solds[i] = FALSE;
      sums[i] = closes[i];
      refs[i] = closes[i] * (1 - strip);
    }_RANGE

    int *pdays = ATOMIC(sizeof(int));
    *pdays = days;
    double *pstrip = ATOMIC(sizeof(double));
    *pstrip = strip;
    int *row = ATOMIC(sizeof(int));
    *row = 1;

    env2 = exp_array(arr_new_from(
      pdays, // 0
      pstrip, // 1
      row, // 2
      is_solds, // 3
      sums, // 4
      refs, // 5
      all_closes, // 6
      NULL
    ));
    map_add(env, "env", env2);

    Vec *refs_v = MALLOC(Vec);
    refs_v->size = cos;
    refs_v->vs = refs;
    return exp_object("<vec>", refs_v);
  }
  void **envs = arr_begin(exp_rget_array(env2));

  // FUNCTIONS -----

  // PROCESS -----

  int days = *((int *)envs[0]);
  double strip = *((double *)envs[1]);
  int *prow = envs[2];
  int row = *prow;
  *prow = row + 1;
  int *is_solds = envs[3];
  double *sums = envs[4];
  double *refs = envs[5];
  Exp **all_closes = envs[6];
  Vec *closes_v = exp_rget_object("<vec>", all_closes[row]);
  int cos = closes_v->size;
  double *closes = closes_v->vs;
  double *old_closes = closes;
  if (row >= days) {
    Vec *old_closes_v = exp_rget_object("<vec>", all_closes[row - days]);
    old_closes = old_closes_v->vs;
  }

  RANGE0(i, cos) {
    double c = closes[i];
    if (row < days) {
      sums[i] += c;
      refs[i] = c * (1 - strip);
    } else if (row == days) {
      sums[i] += c - old_closes[i];
      double avg = sums[i] / days;

      if (c <= avg) {
        is_solds[i] = TRUE;
        refs[i] = avg * (1 + strip);
      } else {
        refs[i] = avg * (1 - strip);
      }
    } else {
      double rf = refs[i];
      sums[i] += c - old_closes[i];
      double avg = sums[i] / days;

      if (is_solds[i]) {
        if (c > rf) {
          is_solds[i] = FALSE;
          refs[i] = avg * (1 - strip);
        } else {
          double new_rf = avg * (1 + strip);
          if (new_rf < rf) refs[i] = new_rf;
          else refs[i] = rf;
        }
      } else {
        if (c <= rf) {
          is_solds[i] = TRUE;
          refs[i] = avg * (1 + strip);
        } else {
          double new_rf = avg * (1 - strip);
          if (new_rf > rf) refs[i] = new_rf;
          else refs[i] = rf;
        }
      }
    }
  }_RANGE

  Vec *refs_v = MALLOC(Vec);
  refs_v->size = cos;
  refs_v->vs = refs;
  return exp_object("<vec>", refs_v);
}

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_mm(Exp *exp) {
  // <Exp>
  Map *env = exp_rget_map(exp);
  Exp *env2 = opt_get(map_get(env, "env"));
  if (!env2) {
    int days = (int)(exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 0)
    ) + 0.4);
    double strip = exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 1)
    );

    // <vec>
    Exp **all_closes =
      (Exp **)arr_begin(exp_rget_array(opt_get(map_get(env, "Closes"))));
    Vec *closes_v = exp_rget_object("<vec>", all_closes[0]);
    double *closes = closes_v->vs;
    int cos = closes_v->size;

    int *is_solds = ATOMIC(sizeof(int) * cos);
    double *refs = ATOMIC(sizeof(double) * cos);
    RANGE0(i, cos) {
      is_solds[i] = FALSE;
      refs[i] = closes[i] * (1 - strip);
    }_RANGE

    int *pdays = ATOMIC(sizeof(int));
    *pdays = days;
    double *pstrip = ATOMIC(sizeof(double));
    *pstrip = strip;
    int *row = ATOMIC(sizeof(int));
    *row = 1;

    env2 = exp_array(arr_new_from(
      pdays, // 0
      pstrip, // 1
      row, // 2
      is_solds, // 3
      refs, // 4
      all_closes, // 5
      NULL
    ));
    map_add(env, "env", env2);

    Vec *refs_v = MALLOC(Vec);
    refs_v->size = cos;
    refs_v->vs = refs;
    return exp_object("<vec>", refs_v);
  }
  void **envs = arr_begin(exp_rget_array(env2));

  // FUNCTIONS -----

  // PROCESS -----

  int days = *((int *)envs[0]);
  double strip = *((double *)envs[1]);
  int *prow = envs[2];
  int row = *prow;
  *prow = row + 1;
  int *is_solds = envs[3];
  double *refs = envs[4];
  Exp **all_closes = envs[5];
  Vec *closes_v = exp_rget_object("<vec>", all_closes[row]);
  int cos = closes_v->size;
  double *closes = closes_v->vs;
  double *old_closes = closes;
  if (row >= days) {
    Vec *old_closes_v = exp_rget_object("<vec>", all_closes[row - days]);
    old_closes = old_closes_v->vs;
  }

  RANGE0(i, cos) {
    double c = closes[i];
    if (row < days) {
      refs[i] = c * (1 - strip);
    } else if (row == days) {
      double old_c = old_closes[i];

      if (c <= old_c) {
        is_solds[i] = TRUE;
        refs[i] = old_c * (1 + strip);
      } else {
        refs[i] = old_c * (1 - strip);
      }
    } else {
      double rf = refs[i];
      double old_c = old_closes[i];

      if (is_solds[i]) {
        if (c > rf) {
          is_solds[i] = FALSE;
          refs[i] = old_c * (1 - strip);
        } else {
          double new_rf = old_c * (1 + strip);
          if (new_rf < rf) refs[i] = new_rf;
          else refs[i] = rf;
        }
      } else {
        if (c <= rf) {
          is_solds[i] = TRUE;
          refs[i] = old_c * (1 + strip);
        } else {
          double new_rf = old_c * (1 - strip);
          if (new_rf > rf) refs[i] = new_rf;
          else refs[i] = rf;
        }
      }
    }
  }_RANGE

  Vec *refs_v = MALLOC(Vec);
  refs_v->size = cos;
  refs_v->vs = refs;
  return exp_object("<vec>", refs_v);
}

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_qfix(Exp *exp) {
  // <Exp>
  Map *env = exp_rget_map(exp);
  Exp *env2 = opt_get(map_get(env, "env"));
  if (!env2) {
    double *jmp = ATOMIC(sizeof(double));
    *jmp = exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 0)
    ) + 1;

    // <vec>
    Exp **all_closes =
      (Exp **)arr_begin(exp_rget_array(opt_get(map_get(env, "Closes"))));
    Vec *closes_v = exp_rget_object("<vec>", all_closes[0]);
    double *closes = closes_v->vs;
    int cos = closes_v->size;
    double *refs = ATOMIC(sizeof(double) * cos);
    RANGE0(i, cos) {
      double jmp2 = *jmp;
      double lg_jmp = log(jmp2);
      refs[i] = pow(jmp2, round(log(closes[i])/lg_jmp) - 1) / jmp2;
    }_RANGE

    int *row = ATOMIC(sizeof(int));
    *row = 1;

    env2 = exp_array(arr_new_from(
      jmp, // 0
      row, // 1
      closes, // 2
      refs, // 3
      all_closes, // 4
      NULL
    ));
    map_add(env, "env", env2);

    Vec *refs_v = MALLOC(Vec);
    refs_v->size = cos;
    refs_v->vs = refs;
    return exp_object("<vec>", refs_v);
  }
  void **envs = arr_begin(exp_rget_array(env2));

  double jmp = *((double *)envs[0]);
  double lg_jmp = log(jmp);

  // FUNCTIONS -----

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

  // PROCESS -----

  int *prow = envs[1];
  int row = *prow;
  *prow = row + 1;
  double *prv_closes = envs[2];
  double *refs = envs[3];
  Exp **all_closes = envs[4];
  Vec *closes_v = exp_rget_object("<vec>", all_closes[row]);
  int cos = closes_v->size;
  double *closes = closes_v->vs;
  envs[2] = closes;

  RANGE0(i, cos) {
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

  Vec *refs_v = MALLOC(Vec);
  refs_v->size = cos;
  refs_v->vs = refs;
  return exp_object("<vec>", refs_v);
}

// \{Closes:[<vec>...], Params:[f...]} -> <vec>
static Exp *calc_qmob(Exp *exp) {
  // <Exp>
  Map *env = exp_rget_map(exp);
  Exp *env2 = opt_get(map_get(env, "env"));
  if (!env2) {
    double gap = exp_rget_float(
      arr_get(exp_rget_array(opt_get(map_get(env, "Params"))), 0)
    );

    // <vec>
    Exp **all_closes =
      (Exp **)arr_begin(exp_rget_array(opt_get(map_get(env, "Closes"))));
    Vec *closes_v = exp_rget_object("<vec>", all_closes[0]);
    double *closes = closes_v->vs;
    int cos = closes_v->size;

    int *is_solds = ATOMIC(sizeof(int) * cos);
    double *refs = ATOMIC(sizeof(double) * cos);
    RANGE0(i, cos) {
      is_solds[i] = FALSE;
      refs[i] = closes[i] * (1 - gap);
    }_RANGE

    double *pgap = ATOMIC(sizeof(double));
    *pgap = gap;
    int *row = ATOMIC(sizeof(int));
    *row = 1;

    env2 = exp_array(arr_new_from(
      pgap, // 0
      row, // 1
      is_solds, // 2
      refs, // 3
      all_closes, // 4
      NULL
    ));
    map_add(env, "env", env2);

    Vec *refs_v = MALLOC(Vec);
    refs_v->size = cos;
    refs_v->vs = refs;
    return exp_object("<vec>", refs_v);
  }
  void **envs = arr_begin(exp_rget_array(env2));

  // FUNCTIONS -----

  // PROCESS -----

  double gap = *((double *)envs[0]);
  int *prow = envs[1];
  int row = *prow;
  *prow = row + 1;
  int *is_solds = envs[2];
  double *refs = envs[3];
  Exp **all_closes = envs[4];
  Vec *closes_v = exp_rget_object("<vec>", all_closes[row]);
  int cos = closes_v->size;
  double *closes = closes_v->vs;

  RANGE0(i, cos) {
    double c = closes[i];
    double rf = refs[i];
    int is_sold = is_solds[i];

    if (is_sold) {
      if (c > rf) {
        is_solds[i] = FALSE;
        refs[i] = c * (1 - gap);
      } else {
        double new_rf = c * (1 + gap);
        if (new_rf > rf) new_rf = rf;
        refs[i] = new_rf;
      }
    } else {
      if (c <= rf) {
        is_solds[i] = TRUE;
        refs[i] = c * (1 + gap);
      } else {
        double new_rf = c * (1 - gap);
        if (new_rf < rf) new_rf = rf;
        refs[i] = new_rf;
      }
    }
  }_RANGE

  Vec *refs_v = MALLOC(Vec);
  refs_v->size = cos;
  refs_v->vs = refs;
  return exp_object("<vec>", refs_v);
}

Exp *exmodule_run (char *fn, Exp *data) {
  if (!strcmp(fn, "calcAppr")) return calc_appr(data);
  if (!strcmp(fn, "calcAppr2")) return calc_appr2(data);
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
