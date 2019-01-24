// Copyright 12-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Global definitions.

#ifndef DEFS_H
  #define DEFS_H

#include "dmc/std.h"

/// Program state
#define ST_ACTIVE "active"

/// Program state
#define ST_SLEEPING "sleeping"

/// Program state
#define ST_TO_ACTIVE "toActive"

/// Program state
#define ST_TO_SLEEPING1 "toSleeping1"

/// Program state
#define ST_TO_SLEEPING2 "toSleeping2"

/// Nicks list
#define NICKS {\
    "A3M",\
    "ACS",\
    "ACX",\
    "ADX",\
    "AENA",\
    "ALM",\
    "AMS",\
    "ANA",\
    "APPS",\
    "BBVA",\
    "BKIA",\
    "BKT",\
    "CABK",\
    "CIE",\
    "CLNX",\
    "COL",\
    "DIA",\
    "ELE",\
    "ENC",\
    "ENG",\
    "FER",\
    "GEST",\
    "GRF",\
    "GSJ",\
    "IAG",\
    "IBE",\
    "IDR",\
    "ITX",\
    "MAP",\
    "MAS",\
    "MEL",\
    "MRL",\
    "MTS",\
    "NTGY",\
    "OHL",\
    "PHM",\
    "PSG",\
    "REE",\
    "REP",\
    "SAB",\
    "SAN",\
    "SCYR",\
    "SGRE",\
    "SLR",\
    "TEF",\
    "TL5",\
    "TRE",\
    "UNI",\
    "VIS",\
    "ZOT",\
    NULL \
  }

/// Path of Market data directory
#define MARKET "/dm/wwwcgi/dmcgi/Market/data"

/// Path of Quotes data directory
#define QUOTES "/dm/wwwcgi/dmcgi/Quotes/data"

/// Path of fleas data directory
#define FLEAS "/home/deme/.dmCApp/fleas"

#endif
