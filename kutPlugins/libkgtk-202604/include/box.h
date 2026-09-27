// Copyright 10-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Box widget.
/// Widget to add widget children.
/// It is posible remove the added widgets, but they can not be reused.

#include "exp.h"

#ifndef BOX_H
  #define BOX_H

/// Creates a new horizontal box with a gap of 10 pixels between elements.
/// \ -> <gwg>
Exp *libkgtk_box_new (Arr *params);

/// Creates a new vertical box with a gap of 10 pixels between elements.
/// \ -> <gwg>
Exp *libkgtk_box_new_v (Arr *params);

/// Adds 'child' to 'b'.
///   b    : Box to set.
///   child: Widget to add. It can not be reused.
/// \<wg>, <wg> -> <wg>
Exp *libkgtk_box_add (Arr *params);

/// Adds 'children' to 'b'.
///   b       : Box to set.
///   children: Widgets to add.
/// \<wg>, [<wg>.] -> <wg>
Exp *libkgtk_box_adds (Arr *params);

/// Removes 'child' from 'b'.
///   b    : Box to set.
///   child: Widget to remove. It can not be reused.
/// \<wg>, <wg> -> <wg>
Exp *libkgtk_box_remove (Arr *params);

/// Removes 'children' from 'b'.
///   b       : Box to set.
///   children: Widgets to remove. Thet can not be reused.
/// \<wg>, [<wg>.] -> <wg>
Exp *libkgtk_box_removes (Arr *params);

/// Sets the elements gap (Default is 10 pixels).
///   b  : Box to set.
///   gap: Elements gap in pixels
Exp *libkgtk_box_spacing (Arr *params);

#endif
