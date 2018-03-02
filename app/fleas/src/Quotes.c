// Copyright 16-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Quotes.h"
#include "DEFS.h"
#include <dm/dm.h>

static Quote *quotes[sizeof(Quote *) * QUOTES_NUMBER * NICKS_NUMBER];

Quote **quotes_get() {
    return quotes;
}
