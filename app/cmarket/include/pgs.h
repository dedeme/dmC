// Copyright 31-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Web pages process.

#ifndef PGS_H
  #define PGS_H

#include <inttypes.h>
#include "kut/DEFS.h"
#include "kut/map.h"
#include "kut/js.h"
//#include "kut/str.h"

/// Web client request process.
///   ck: Communication key.
///   rq: Map of JSON strings.
void pgs_process (char *ck, Map *rq);

/// Read a boolean of a request.
///   rq : Map of JSON strings.
///   key: Key to read
int pgs_rb (Map *rq, char *key);

/// Read a int of a request.
///   rq : Map of JSON strings.
///   key: Key to read
int pgs_ri (Map *rq, char *key);

/// Read a int64 of a request.
///   rq : Map of JSON strings.
///   key: Key to read
int64_t pgs_rl (Map *rq, char *key);

/// Read a string of a request.
///   rq : Map of JSON strings.
///   key: Key to read
char *pgs_rs (Map *rq, char *key);

/// Read the JS string of a request.
///   rq : Map of JSON strings.
///   key: Key to read
char *pgs_r (Map *rq, char *key);

/// Print a response.
///   rp : Map of JSON strings.
void pgs_print (Map *rp);

/// Print an empty response.
void pgs_print_empty (void);

#endif
