// Copyright 09-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "strategy.h"
#include "kut/DEFS.h"
#include "kut/arr.h"
#include "kut/map.h"
#include "model/appr.h"
#include "model/appr2.h"
#include "model/appr3.h"
#include "model/ea.h"
#include "model/ea2.h"
#include "model/ma.h"
#include "model/mm.h"
#include "model/qfix.h"
#include "model/qmob.h"
#include "model/sprs.h"
#include "model/uudd.h"
#include "model/uudd0.h"
#include "model/uudd2.h"
#include "model/uuqf.h"
#include "exp.h"
#include "iface.h"
#include "order.h"
#include "broker.h"
#include "DEFS.h"

#define FREFS(id) Arr *(*id)(ModelParams *)

typedef struct { char *mdId; FREFS(fn); } MdEntry;

static MdEntry mdList[] = {
  { "APRX", libmkt_appr_refs_c },
  { "APRX2", libmkt_appr2_refs_c },
  { "APRX3", libmkt_appr3_refs_c },
  { "ME", libmkt_ea_refs_c },
  { "ME2", libmkt_ea2_refs_c },
  { "MM", libmkt_ma_refs_c },
  { "MX_MN", libmkt_mm_refs_c },
  { "QFIJO", libmkt_qfix_refs_c },
  { "QMOV", libmkt_qmob_refs_c },
  { "SPRS", libmkt_sprs_refs_c },
  { "SS_BB", libmkt_uudd_refs_c },
  { "SSBB0", libmkt_uudd0_refs_c },
  { "SSBB2", libmkt_uudd2_refs_c },
  { "SSQF", libmkt_uuqf_refs_c },
  { NULL, NULL }
};

static FREFS(mdGet (char *mdId)) {
  int i = 0;
  FREFS(r) = NULL;
  for (;;) {
    MdEntry e = mdList[i];
    if (!e.mdId) EXC_KUT(str_f("Module %s not found", mdId));
    if (!strcmp(e.mdId, mdId)) {
      r = e.fn;
      break;
    }
    ++i;
  }
  return r;
}

static Exp *doubles_to_exp (int n, double *values) {
  // <Exp>
  Arr *a = arr_new_bf(n);
  for (int i = 0; i < n; ++i) arr_push(a, exp_float(values[i]));
  return exp_array(a);
}

// Arrays of values are Arr<char>
static Exp *orders_to_exp (int n, Arr **values) {
  // <Exp>
  Arr *a = arr_new_bf(n);
  for (int i = 0; i < n; ++i)
    arr_push(a, exp_array(arr_map(values[i], (FMAP)exp_string)));
  return exp_array(a);
}

Exp *libmkt_strategy_open (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);

  Quotes *qts = exp_get_object("cquotes", aparams[0]);
  int ndates = libmkt_HISTORIC_QUOTES;
  int ncos = qts->ncos;
  char **cos = qts->cos;
  char **dates = qts->dates;
  double **opens = qts->opens;
  double **closes = qts->closes;

  // <double>
  Arr *areferences = arr_new();
  EACH(exp_get_array(aparams[1]), Exp, row_exp) {
    double *row = ATOMIC(sizeof(double) * ncos);
    EACH(exp_get_array(row_exp), Exp, val_exp) {
      row[_i] = exp_get_float(val_exp);
    }_EACH
    arr_push(areferences, row);
  }_EACH
  double **references = (double **)arr_begin(areferences);

  // Global simulation.

  double cash = INITIAL_CAPITAL;
  double withdrawals = 0;

  // Arr<Order>
  Arr *Orders = arr_new();

  double hreals[ndates];
  double haccs[ndates];
  double hrefs[ndates];
  double hwithdrawals[ndates];
  for (int i = 0; i < ncos; ++i) {
    hreals[i] = 0;
    haccs[i] = 0;
    hrefs[i] = 0;
    hwithdrawals[i] = 0;
  }

  int stocks[ncos];
  double prices[ncos];
  int to_dos[ncos];
  int to_sells[ncos];
  int days_traps[ncos];
  double profits[ncos];
  for (int i = 0; i < ncos; ++i) {
    stocks[i] = 0;
    prices[i] = 0;
    to_dos[i] = FALSE;
    to_sells[i] = TRUE;
    days_traps[i] = 0;
    profits[i] = 0;
  }

  // Profits simulation.

  // Arr<Arr<char>>
  Arr *all_buys[ncos];
  // Arr<Arr<char>>
  Arr *all_sales[ncos];

  double prf_cashes[ncos];
  int prf_stocks[ncos];
  double prf_prices[ncos];
  int prf_days_traps[ncos];
  for (int i = 0; i < ncos; ++i) {
    all_buys[i] = arr_new();
    all_sales[i] = arr_new();

    prf_cashes[i] = BET;
    prf_stocks[i] = 0;
    prf_prices[i] = 0;
    prf_days_traps[i] = 0;
  }

  // START -----------------------------

  int purchased = 0;
  int max_to_buy = MAX_COS;
  hreals[0] = cash;
  haccs[0] = cash;
  hrefs[0] = cash;
  for (int idate = 1; idate < ndates; ++idate) {
    double day_cash = cash;
    char *date = dates[idate];
    double *ops = opens[idate];
    double *cls = closes[idate];
    double *rfs = references[idate - 1];

    max_to_buy = MAX_COS - purchased;
    for (int ico = 0; ico < ncos; ++ico)
      if (to_dos[ico] && !to_sells[ico] && stocks[ico] > 0) ++max_to_buy;

    double real = 0;
    double acc = 0;
    double ref = 0;
    for (int ico = 0; ico < ncos; ++ico) {
      char *nk = cos[ico];
      // <char>
      Arr *buys = all_buys[ico];
      // <char>
      Arr *sales = all_sales[ico];
      double op = ops[ico];
      double cl = cls[ico];
      double rf = rfs[ico];

      days_traps[ico] = days_traps[ico] - 1;
      prf_days_traps[ico] = prf_days_traps[ico] - 1;

      if (to_dos[ico]) {
        if (to_sells[ico]) { // there is buy order.
          // Global simulation.
          if (days_traps[ico] < 1) {
            if (day_cash > MIN_TO_BET && purchased < MAX_COS) {
              int stks = BET / op;
              stocks[ico] = stks;
              prices[ico] = op;
              double bk = broker_buy(stks, op);
              cash -= bk;
              day_cash -= bk;
              arr_push(Orders, order_new(date, nk, ORDER_BUY, stks, op));
              purchased += 1;
              max_to_buy -= 1;
            }
          }

          // Profits simulation.
          if (prf_days_traps[ico] < 1) {
            double prf_cash = prf_cashes[ico];
            int stocks = (prf_cash - broker_buy_fees(prf_cash)) / op;
            double cost = broker_buy(stocks, op);
            while (cost > prf_cash) {
              stocks -= 1;
              cost = broker_buy(stocks, op);
            }
            prf_stocks[ico] = stocks;
            prf_prices[ico] = op;
            prf_cashes[ico] = prf_cash - cost;
            arr_push(buys, date);
          }

        } else {
          // Global simulation.
          double stks = stocks[ico];
          if (stks > 0) {
            stocks[ico] = 0;
            cash += broker_sell(stks, op);
            arr_push(Orders, order_new(date, nk, ORDER_SELL, stks, op));
            purchased -= 1;

            if (op < prices[ico] * NO_LOSS_MULTIPLICATOR)
              days_traps[ico] = DAYS_LOSS;
          }

          // Profits simulation.
          int pstocks = prf_stocks[ico];
          if (pstocks > 0) {
            prf_stocks[ico] = 0;
            prf_cashes[ico] = prf_cashes[ico] + broker_sell(pstocks, op);
            arr_push(sales, date);

            if (op < prf_prices[ico] * NO_LOSS_MULTIPLICATOR)
              prf_days_traps[ico] = DAYS_LOSS;
          }
        }
        to_dos[ico] = FALSE;
      }
      double stks = stocks[ico];
      if (stks > 0) {
        real += broker_sell(stks, cl);
        acc += broker_sell(stks, prices[ico]);
        ref += broker_sell(stks, rf < cl ? rf : cl);
      }

      if (to_sells[ico]) {
        if (rf > cl) {
          to_dos[ico] = TRUE;
          to_sells[ico] = FALSE;
        }
      } else if (rf < cl) {
        to_dos[ico] = TRUE;
        to_sells[ico] = TRUE;
      }
    }

    double total = cash + real;
    if (total > WITHDRAWAL_LIMIT && cash > MIN_TO_BET) {
      withdrawals += BET;
      cash -= BET;
    }

    hreals[idate] = cash + withdrawals + real;
    haccs[idate] = cash + withdrawals + acc;
    hrefs[idate] = cash + withdrawals + ref;
    hwithdrawals[idate] = withdrawals;
  }

  double *last_closes = closes[ndates - 1];
  for (int i = 0; i < ncos; ++i) {
    profits[i] =
      (prf_cashes[i] + prf_stocks[i] * last_closes[i] - BET) / BET;
  }

  return exp_array(arr_new_from(
    exp_array(arr_map(Orders, (FMAP)exp_string)),
    doubles_to_exp(ndates, hreals),
    doubles_to_exp(ndates, haccs),
    doubles_to_exp(ndates, hrefs),
    doubles_to_exp(ndates, hwithdrawals),
    exp_float(cash + withdrawals),
    orders_to_exp(ncos, all_buys),
    orders_to_exp(ncos, all_sales),
    doubles_to_exp(ncos, profits),
    NULL
  ));
}

Exp *libmkt_strategy_open_simple (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  Quotes *qts = exp_get_object("cquotes", aparams[0]);
  // <double>
  Arr *areferences = arr_new();
  EACH(exp_get_array(aparams[1]), Exp, row_exp) {
    double *row = ATOMIC(sizeof(double) * qts->ncos);
    EACH(exp_get_array(row_exp), Exp, val_exp) {
      row[_i] = exp_get_float(val_exp);
    }_EACH
    arr_push(areferences, row);
  }_EACH
  double **references = (double **)arr_begin(areferences);

  double *r = libmkt_strategy_open_simple_c(qts, references);

  return exp_dic(
    map_from_array(arr_new_from(
      kv_new("sales", exp_float(r[0])), // nsales
      kv_new("assets", exp_float(r[1])), // assets
      kv_new("accs", exp_float(r[2])), // accs
      kv_new("rfAssets", exp_float(r[3])), //rf_assets
      NULL
    ))
  );
}

double *libmkt_strategy_open_simple_c (Quotes *qts, double **references) {
  int ndates = libmkt_HISTORIC_QUOTES;
  int ncos = qts->ncos;
  double **opens = qts->opens;
  double **closes = qts->closes;

  // Global simulation.

  double cash = INITIAL_CAPITAL;
  double withdrawals = 0;
  int nsales = 0;

  int stocks[ncos];
  double prices[ncos];
  int to_dos[ncos];
  int to_sells[ncos];
  int days_traps[ncos];
  for (int i = 0; i < ncos; ++i) {
    stocks[i] = 0;
    prices[i] = 0;
    to_dos[i] = FALSE;
    to_sells[i] = TRUE;
    days_traps[i] = 0;
  }

  // START -----------------------------

  int purchased = 0;
  for (int idate = 1; idate < ndates; ++idate) {
    double day_cash = cash;
    double *ops = opens[idate];
    double *cls = closes[idate];
    double *rfs = references[idate - 1];

    double assets = 0;
    for (int ico = 0; ico < ncos; ++ico) {
      double op = ops[ico];
      double cl = cls[ico];
      double rf = rfs[ico];

      days_traps[ico] = days_traps[ico] - 1;

      if (to_dos[ico]) {
        if (to_sells[ico]) { // there is buy order.
          // Global simulation.
          if (days_traps[ico] < 1) {
            if (day_cash > MIN_TO_BET && purchased < MAX_COS) {
              int stks = BET / op;
              stocks[ico] = stks;
              prices[ico] = op;
              double bk = broker_buy(stks, op);
              cash -= bk;
              day_cash -= bk;
              purchased += 1;
            }
          }
        } else {
          // Global simulation.
          double stks = stocks[ico];
          if (stks > 0) {
            stocks[ico] = 0;
            cash += broker_sell(stks, op);
            nsales += 1;
            purchased -= 1;

            if (op < prices[ico] * NO_LOSS_MULTIPLICATOR)
              days_traps[ico] = DAYS_LOSS;
          }
        }
        to_dos[ico] = FALSE;
      }
      double stks = stocks[ico];
      if (stks > 0) assets += broker_sell(stks, cl);

      if (to_sells[ico]) {
        if (rf > cl) {
          to_dos[ico] = TRUE;
          to_sells[ico] = FALSE;
        }
      } else if (rf < cl) {
        to_dos[ico] = TRUE;
        to_sells[ico] = TRUE;
      }
    }

    double total = cash + assets;
    if (total > WITHDRAWAL_LIMIT && cash > MIN_TO_BET) {
      withdrawals += BET;
      cash -= BET;
    }
  }
  // Global simulation.
  cash += withdrawals;
  double assets = cash;
  double accs = cash;
  double rf_assets = cash;
  double *last_cls = closes[ndates - 1];
  double *last_rfs = references[ndates - 1];
  for (int i = 0; i < ncos; ++i) {
    int stk = stocks[i];
    double cl = last_cls[i];
    double rf = last_rfs[i];
    if (stk > 0) {
      assets += broker_sell(stk, cl);
      accs += broker_sell(stk, prices[i]);
      rf_assets += broker_sell(stk, rf < cl ? rf : cl);
    }
  }

  double *r = ATOMIC(sizeof(double) * 4);
  r[0] = nsales;
  r[1] = assets;
  r[2] = accs;
  r[3] = rf_assets;
  return r;
}

Exp *libmkt_strategy_open_simple2 (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  Quotes *qts = exp_get_object("cquotes", aparams[0]);
  // <double>
  Arr *areferences = arr_new();
  EACH(exp_get_array(aparams[1]), Exp, row_exp) {
    double *row = ATOMIC(sizeof(double) * qts->ncos);
    EACH(exp_get_array(row_exp), Exp, val_exp) {
      row[_i] = exp_get_float(val_exp);
    }_EACH
    arr_push(areferences, row);
  }_EACH
  double **references = (double **)arr_begin(areferences);

  double *r = libmkt_strategy_open_simple2_c(qts, references);

  return exp_dic(
    map_from_array(arr_new_from(
      kv_new("sales", exp_float(r[0])), // nsales
      kv_new("assets", exp_float(r[1])), // assets
      kv_new("accs", exp_float(r[2])), // accs
      kv_new("rfAssets", exp_float(r[3])), //rf_assets

      kv_new("profits", exp_float(r[4])), // profits / ncos
      kv_new("rfProfits", exp_float(r[5])), // rf_profits / ncos
      NULL
    ))
  );
}

double *libmkt_strategy_open_simple2_c (Quotes *qts, double **references) {
  int ndates = libmkt_HISTORIC_QUOTES;
  int ncos = qts->ncos;
  double **opens = qts->opens;
  double **closes = qts->closes;

  // Global simulation.

  double cash = INITIAL_CAPITAL;
  double withdrawals = 0;
  int nsales = 0;

  int stocks[ncos];
  double prices[ncos];
  int to_dos[ncos];
  int to_sells[ncos];
  int days_traps[ncos];
  for (int i = 0; i < ncos; ++i) {
    stocks[i] = 0;
    prices[i] = 0;
    to_dos[i] = FALSE;
    to_sells[i] = TRUE;
    days_traps[i] = 0;
  }

  // Profits simulation.

  double prf_cashes[ncos];
  int prf_stocks[ncos];
  double prf_prices[ncos];
  int prf_days_traps[ncos];
  for (int i = 0; i < ncos; ++i) {
    prf_cashes[i] = BET;
    prf_stocks[i] = 0;
    prf_prices[i] = 0;
    prf_days_traps[i] = 0;
  }

  // START -----------------------------

  int purchased = 0;
  for (int idate = 1; idate < ndates; ++idate) {
    double day_cash = cash;
    double *ops = opens[idate];
    double *cls = closes[idate];
    double *rfs = references[idate - 1];

    double assets = 0;
    for (int ico = 0; ico < ncos; ++ico) {
      double op = ops[ico];
      double cl = cls[ico];
      double rf = rfs[ico];

      days_traps[ico] = days_traps[ico] - 1;
      prf_days_traps[ico] = prf_days_traps[ico] - 1;

      if (to_dos[ico]) {
        if (to_sells[ico]) { // there is buy order.
          // Global simulation.
          if (days_traps[ico] < 1) {
            if (day_cash > MIN_TO_BET && purchased < MAX_COS) {
              int stks = BET / op;
              stocks[ico] = stks;
              prices[ico] = op;
              double bk = broker_buy(stks, op);
              cash -= bk;
              day_cash -= bk;
              purchased += 1;
            }
          }

          // Profits simulation.
          if (prf_days_traps[ico] < 1) {
            double prf_cash = prf_cashes[ico];
            int stocks = (prf_cash - broker_buy_fees(prf_cash)) / op;
            double cost = broker_buy(stocks, op);
            while (cost > prf_cash) {
              stocks -= 1;
              cost = broker_buy(stocks, op);
            }
            prf_stocks[ico] = stocks;
            prf_prices[ico] = op;
            prf_cashes[ico] = prf_cash - cost;
          }

        } else {
          // Global simulation.
          double stks = stocks[ico];
          if (stks > 0) {
            stocks[ico] = 0;
            cash += broker_sell(stks, op);
            nsales += 1;
            purchased -= 1;

            if (op < prices[ico] * NO_LOSS_MULTIPLICATOR)
              days_traps[ico] = DAYS_LOSS;
          }

          // Profits simulation.
          int pstocks = prf_stocks[ico];
          if (pstocks > 0) {
            prf_stocks[ico] = 0;
            prf_cashes[ico] = prf_cashes[ico] + broker_sell(pstocks, op);

            if (op < prf_prices[ico] * NO_LOSS_MULTIPLICATOR)
              prf_days_traps[ico] = DAYS_LOSS;
          }
        }
        to_dos[ico] = FALSE;
      }
      double stks = stocks[ico];
      if (stks > 0) assets += broker_sell(stks, cl);

      if (to_sells[ico]) {
        if (rf > cl) {
          to_dos[ico] = TRUE;
          to_sells[ico] = FALSE;
        }
      } else if (rf < cl) {
        to_dos[ico] = TRUE;
        to_sells[ico] = TRUE;
      }
    }

    double total = cash + assets;
    if (total > WITHDRAWAL_LIMIT && cash > MIN_TO_BET) {
      withdrawals += BET;
      cash -= BET;
    }
  }
  // Global simulation.
  cash += withdrawals;
  double assets = cash;
  double accs = cash;
  double rf_assets = cash;
  double *last_cls = closes[ndates - 1];
  double *last_rfs = references[ndates - 1];
  for (int i = 0; i < ncos; ++i) {
    int stk = stocks[i];
    double cl = last_cls[i];
    double rf = last_rfs[i];
    if (stk > 0) {
      assets += broker_sell(stk, cl);
      accs += broker_sell(stk, prices[i]);
      rf_assets += broker_sell(stk, rf < cl ? rf : cl);
    }
  }

  // Profits simulation.
  double profits = 0;
  double rf_profits = 0;
  for (int i = 0; i < ncos; ++i) {
    double prf_cash = prf_cashes[i];
    int stk = prf_stocks[i];
    double cl = last_cls[i];
    double rf = last_rfs[i];
    profits += (prf_cash + (stk > 0 ? stk * cl : 0) - BET) / BET;
    rf_profits += (
      prf_cash + (
        stk > 0
        ? stk * (rf < cl ? rf : cl)
        : 0
      ) - BET) / BET
    ;
  }

  double *r = ATOMIC(sizeof(double) * 6);
  r[0] = nsales;
  r[1] = assets;
  r[2] = accs;
  r[3] = rf_assets;
  r[4] = profits / ncos;
  r[5] = rf_profits / ncos;
  return r;
}

double **libmkt_strategy_group_c (char *mdId, Quotes *qts, Arr *params) {
  FREFS(fn) = mdGet(mdId);
  int nparams = arr_size(params);
  double *sales = ATOMIC(sizeof(double) * nparams);
  double *assets = ATOMIC(sizeof(double) * nparams);
  double *accs = ATOMIC(sizeof(double) * nparams);
  double *rf_assets = ATOMIC(sizeof(double) * nparams);
  EACH(params, double, ps) {
    ModelParams mpars = {
      libmkt_HISTORIC_QUOTES,
      qts->ncos,
      qts->closes,
      ps
    };
    Arr *refs = fn(&mpars);
    double *rMd = libmkt_strategy_open_simple_c(qts, (double **)arr_begin(refs));
    sales[_i] = rMd[0];
    assets[_i] = rMd[1];
    accs[_i] = rMd[2];
    rf_assets[_i] = rMd[3];
  }_EACH

  double **r = GC_MALLOC(sizeof(double **) * 4);
  r[0] = sales;
  r[1] = assets;
  r[2] = accs;
  r[3] = rf_assets;
  return r;
}

Exp *libmkt_strategy_group (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  char *mdId = exp_get_string(aparams[0]);
  Quotes *qts = exp_get_object("cquotes", aparams[1]);
  // <double>
  Arr *ps = arr_new();
  EACH(exp_get_array(aparams[2]), Exp, e) {
    Arr *params = exp_get_array(e);
    double *mdPs = ATOMIC(sizeof(double) * arr_size(params));
    EACH(params, double, v) {
      mdPs[_i] = *v;
    }_EACH
    arr_push(ps, mdPs);
  }_EACH
  int nps = arr_size(ps);

  double **r = libmkt_strategy_group_c(mdId, qts, ps);

  return exp_dic(
    map_from_array(arr_new_from(
      kv_new("Sales", iface_doubles_to_exp(nps, r[0])), // nsales
      kv_new("Assets", iface_doubles_to_exp(nps, r[1])), // assets
      kv_new("Accs", iface_doubles_to_exp(nps, r[2])), // accs
      kv_new("RfAssets", iface_doubles_to_exp(nps, r[3])), //rf_assets
      NULL
    ))
  );
}

double **libmkt_strategy_group2_c (char *mdId, Quotes *qts, Arr *params) {
  FREFS(fn) = mdGet(mdId);
  int nparams = arr_size(params);
  double *sales = ATOMIC(sizeof(double) * nparams);
  double *assets = ATOMIC(sizeof(double) * nparams);
  double *accs = ATOMIC(sizeof(double) * nparams);
  double *rf_assets = ATOMIC(sizeof(double) * nparams);
  double *profits = ATOMIC(sizeof(double) * nparams);
  double *rf_profits = ATOMIC(sizeof(double) * nparams);
  EACH(params, double, ps) {
    ModelParams mpars = {
      libmkt_HISTORIC_QUOTES,
      qts->ncos,
      qts->closes,
      ps
    };
    Arr *refs = fn(&mpars);
    double *rMd = libmkt_strategy_open_simple2_c(qts, (double **)arr_begin(refs));
    sales[_i] = rMd[0];
    assets[_i] = rMd[1];
    accs[_i] = rMd[2];
    rf_assets[_i] = rMd[3];
    profits[_i] = rMd[4];
    rf_profits[_i] = rMd[5];
  }_EACH

  double **r = GC_MALLOC(sizeof(double **) * 6);
  r[0] = sales;
  r[1] = assets;
  r[2] = accs;
  r[3] = rf_assets;
  r[4] = profits;
  r[5] = rf_profits;
  return r;
}

Exp *libmkt_strategy_group2 (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  char *mdId = exp_get_string(aparams[0]);
  Quotes *qts = exp_get_object("cquotes", aparams[1]);
  // <double>
  Arr *ps = arr_new();
  EACH(exp_get_array(aparams[2]), Exp, e) {
    Arr *params = exp_get_array(e);
    double *mdPs = ATOMIC(sizeof(double) * arr_size(params));
    EACH(params, double, v) {
      mdPs[_i] = *v;
    }_EACH
    arr_push(ps, mdPs);
  }_EACH
  int nps = arr_size(ps);

  double **r = libmkt_strategy_group2_c(mdId, qts, ps);

  return exp_dic(
    map_from_array(arr_new_from(
      kv_new("Sales", iface_doubles_to_exp(nps, r[0])), // nsales
      kv_new("Assets", iface_doubles_to_exp(nps, r[1])), // assets
      kv_new("Accs", iface_doubles_to_exp(nps, r[2])), // accs
      kv_new("RfAssets", iface_doubles_to_exp(nps, r[3])), // rf_assets
      kv_new("Profits", iface_doubles_to_exp(nps, r[4])), // profits
      kv_new("RfProfits", iface_doubles_to_exp(nps, r[5])), // rf_profits
      NULL
    ))
  );
}
