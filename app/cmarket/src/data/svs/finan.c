// Copyright 06-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/svs/finan.h"
#include "kut/DEFS.h"
#include "kut/str.h"
#include "kut/rs.h"
#include "kut/sys.h"
#include "kut/dec.h"
#include "kut/time.h"
#include "mkt/qs.h"

// DIARY -----------------------------------------------------------------------

// <Arr<Kv<double>>, Arr<char>>
static Tp *read_diary (char *html) {
  EXC_ILLEGAL_STATE("Not implemented");
  return NULL; // Unreachable
}

// HISTORIC --------------------------------------------------------------------

// <Opt<Qs>, Arr<char>>
static Tp *read_historic (char *html) {
  EXC_ILLEGAL_STATE("Not implemented");
  return NULL; // Unreachable
}

// OTHER -----------------------------------------------------------------------

static char *id (void) {
  return "FINAN";
}

static char *name (void) {
  return "Finanzas";
}

static int with_diary (void) {
  return FALSE;
}

// <char>
static Rs *download_diary (void) {
  EXC_ILLEGAL_STATE("Not implemented");
  return NULL; // Unreachable
}

static int with_historic (void) {
  return FALSE;
}

// <char>
static Rs *download_historic (char *code) {
  EXC_ILLEGAL_STATE("Not implemented");
  return NULL; // Unreachable
}

Qsv *finan_new (void) {
  return qsv_new(
    id, name, with_diary, download_diary, read_diary,
    with_historic, download_historic, read_historic
  );
}
