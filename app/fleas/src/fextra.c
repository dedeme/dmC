// Copyright 17-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dmc/all.h>
#include "fextra.h"
#include "Flea.h"
#include "DEFS.h"
#include "families/buyAndHold.h"
#include "families/upDown.h"
#include "families/movingAverage.h"
#include "families/wmovingAverage.h"
#include "families/rsi.h"
#include "families/follow.h"
#include "families/followMa.h"
#include "families/followWma.h"

Flea *fextra_new(Flea *f) {
  switch (flea_family(f)) {
    case BUY_AND_HOLD:
      return buyAndHold_new(f);
    case UP_DOWN:
      return updown_new(f);
    case MOVING_AVERAGE:
      return movingAverage_new(f);
    case WMOVING_AVERAGE:
      return wmovingAverage_new(f);
    case RSI:
      return rsi_new(f);
    case FOLLOW:
      return follow_new(f);
    case FOLLOW_MA:
      return followma_new(f);
    case FOLLOW_WMA:
      return followwma_new(f);
    default:
      THROW "family %d does no exists", flea_family(f) _THROW
      return NULL;
  }
}

Flea *fextra_restore(Flea *f, Json *serial) {
  switch (flea_family(f)) {
    case BUY_AND_HOLD:
      return buyAndHold_restore(f, serial);
    case UP_DOWN:
      return updown_restore(f, serial);
    case MOVING_AVERAGE:
      return movingAverage_restore(f, serial);
    case WMOVING_AVERAGE:
      return wmovingAverage_restore(f, serial);
    case RSI:
      return rsi_restore(f, serial);
    case FOLLOW:
      return follow_restore(f, serial);
    case FOLLOW_MA:
      return followma_restore(f, serial);
    case FOLLOW_WMA:
      return followwma_restore(f, serial);
    default:
      THROW "family %d does not exists", flea_family(f) _THROW
      return NULL;
  }
}
