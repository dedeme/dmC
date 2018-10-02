// Copyright 27-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Market inverstor

#ifndef FLEA_H
  #define FLEA_H

#include "dmc/std.h"
#include "market/Quote.h"
#include "Gen.h"

///
enum flea_BuySellStatus {BUY_SELL_BUY, BUY_SELL_SELL, BUY_SELL_NONE};

///
typedef struct flea_Family Family;

/*.-.*/

#include "dmc/Json.h"

///
typedef struct flea_Flea Flea;

///
Gen *flea_gens(Flea *this);

///
int flea_buys(Flea *this);

///
int flea_sells(Flea *this);

///
double flea_assets(Flea *this);

///
double flea_ponderation(Flea *this);

///
void flea_set_ponderation(Flea *this, double value);

///
Json *flea_to_json(Flea *this);

///
Flea *flea_from_json(Json *s);

/*.-.*/

/// Creates a flea.
///   Family: one of 'flea_family_list()'
Flea *flea_prototype(Family *family);

/// Mutates a flea:
///   this: The flea
///   cycle: Cycle of creation.
///   id: Order number within the cycle.
Flea *flea_mutate(Flea *this, int cycle, int id);

/// Make a operations cycle.
///   this: The flea.
///   money: Initial amount to operate.
///   quotes: Quotes with days in rows and nicks in columns.
///   days_number: Number of days (rows)
///   companies_number: Number of companies (columns)
void flea_operate(
  Flea *this,
  double money,
  Quote ***quotes,
  int days_number,
  int companies_number
);

/// Returns a list of current families.
///   familes: Array with all the families
///   families_number: The families number
void flea_families_list(Family ***families, int *families_number);

///
char *flea_family_to_str(Family *family);

#endif
