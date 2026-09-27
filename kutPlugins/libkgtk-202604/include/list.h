// Copyright 25-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// List widget.

#include "exp.h"

#ifndef LIST_H
  #define LIST_H

/// Creates a new List.
///   isSelectable: 'true' if the list is selectable.
///   values: Array of strings.
///   fn    : Callback to create and set the row widgets. It has two parameters:
///             cbox - Object with a Gtk horizontal box to place value widgets.
///             value- Array to determine widgets and values to pace in 'cbox'.
/// \[s.], \<wg>,s->() -> <wg>
Exp *libkgtk_list_new (Arr *params);

/// Scrolls 'w' at 'ix' position.
/// NOTE: Make sure that 'w' has not be removed.
///   w: List.
///   ix: Row index to do the operation. It must be less than the list length.
/// \<wg>, i -> <wg>
Exp *libkgtk_list_scroll (Arr *params);

#endif
