// Copyright 15-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Check button widget.

#include "exp.h"

#ifndef CHECKBUTTON_H
  #define CHECKBUTTON_H

/// Creates a new checkButtton.
/// \ -> <gwg>
Exp *libkgtk_checkbutton_new (Arr *params);

/// Set text of 'w'.
///   w : Widget to set.
///   tx: Label.
/// \<wg>, s -> <wg>
Exp *libkgtk_checkbutton_text (Arr *params);

/// Set text of 'w'.
///   w  : Widget to set.
///   sel: Selection state: 1:"on" (default), 0:"off", 2:"mixed".
/// \<wg>, i -> <wg>
Exp *libkgtk_checkbutton_select (Arr *params);

/// Returns the selection state of 'w':
///   -> 1:"on" (default), 0:"off", 2:"mixed".
///
///   w  : Check Button.
/// \<wg> -> i
Exp *libkgtk_checkbutton_get_select (Arr *params);

/// Group 'w' inside the group of 'other'.
///   w    : Widget to set.
///   other: Another checkButton.
/// \<wg>, <wg> -> <wg>
Exp *libkgtk_checkbutton_group (Arr *params);

#endif
