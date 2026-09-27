// Copyright 10-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Button widget.

#include "exp.h"

#ifndef BUTTON_H
  #define BUTTON_H

/// Creates a new button.
/// \ -> <gwg>
Exp *libkgtk_button_new (Arr *params);

/// Sets 'w' text (label).
///   w : Widged to set.
///   tx: Value.
/// \<wg>, s -> <wg>
Exp *libkgtk_button_text (Arr *params);

#endif
