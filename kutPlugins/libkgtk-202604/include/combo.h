// Copyright 16-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Combo list widget.

#include "exp.h"

#ifndef COMBO_H
  #define COMBO_H

/// Creates a new checkButtton.
///   Items: Combi items.
///   sel  : Index of selected item. It is forced to items range.
/// \[s.], i -> <gwg>
Exp *libkgtk_combo_new (Arr *params);

/// Set text of 'w'.
///   w    : Widget to set.
///   sel  : Index of selected item. It is forced to items range.
/// \<wg>, i -> <wg>
Exp *libkgtk_combo_select (Arr *params);

/// Returns the selected index of 'w', or -1 if there is no selection.
///   w  : Combo box.
/// \<wg> -> i
Exp *libkgtk_combo_get_select (Arr *params);

#endif
