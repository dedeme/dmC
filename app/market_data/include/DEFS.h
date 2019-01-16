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
#define ST_TO_SLEEPING "toSleeping"

/// Delay time in ST_ACTIVE state
#define ST_TIME_ACTIVE 5

/// Delay time in ST_SLEEPING state
#define ST_TIME_SLEEPING 5

/// Nicks list
#define NICKS {\
    "A3M",\
    "ACS",\
    "ACX",\
    "AENA",\
    "ALM",\
    "AMS",\
    "ANA",\
    "BBVA",\
    "BKIA",\
    "BKT",\
    "CABK",\
    "CAF",\
    "CIE",\
    "CLNX",\
    "COL",\
    "DIA",\
    "ECR",\
    "EKT",\
    "ELE",\
    "ENC",\
    "ENG",\
    "FAE",\
    "FCC",\
    "FER",\
    "GEST",\
    "GRF",\
    "IAG",\
    "IBE",\
    "IDR",\
    "ITX",\
    "MAP",\
    "MEL",\
    "MRL",\
    "MTS",\
    "NHH",\
    "NTGY",\
    "OHL",\
    "PHM",\
    "PSG",\
    "REE",\
    "REP",\
    "SAB",\
    "SAN",\
    "SCYR",\
    "SGREN",\
    "SLR",\
    "TEF",\
    "TL5",\
    "TLGO",\
    "TRE",\
    "TUB",\
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
