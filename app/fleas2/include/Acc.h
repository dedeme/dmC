// Copyright 25-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef ACC_H
  #define ACC_H

#include "dmc/std.h"

/*--*/

///
typedef struct acc_Acc Acc;

///
void acc_free(Acc *this);

///
double acc_cash(Acc *this);

/*--*/


/// Create a new Acc
Acc *acc_new(void);

/// Adds a buy
void acc_buy(Acc *this, int company, int stocks, double price);

/// Adds a sell. Returns 1 if sell was successful. Otherwise returns 0.
int acc_sell(Acc *this, int company, double price);

/// Returns cash after liquidation using the last day closes.
double acc_liquidation(Acc *this);

#endif
