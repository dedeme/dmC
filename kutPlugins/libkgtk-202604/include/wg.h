// Copyright 10-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Global widget functions.

#include "exp.h"

#ifndef WG_H
  #define WG_H

/// Add a new CSS class to use for any widget.
/// Example:
///   wg.setCss("""
///     .a {background-color: #135; color: #800000; font-weight:bold;}
///     .b {background-color: #135; color: #000095; font-weight:normal; }
///     """);
///
///   style: Style definition.
/// \s, s -> ()
Exp *libkgtk_wg_setCss (Arr *params);

/// Set css attributes of 'w'.
/// \<wg>, s -> <wg>
Exp *libkgtk_wg_css (Arr *params);

/// Change css attributes of 'w' from before to after.
///   w     : Widget to set.
///   before: css attributes to remove. It is not an error If 'before' was not
///           added previuosly.
///   after : css attributes to set. If its value is "" no css will be set.
/// \<wg>, s, s -> <wg>
Exp *libkgtk_wg_ch_css (Arr *params);

/// Sets the cursor type when it is over 'w'.
///   w     : Widget to set.
///   cursor: Cursor name. See
///           https://docs.gtk.org/gdk4/ctor.Cursor.new_from_name.html.
/// \<wg>, s -> <wg>
Exp *libkgtk_wg_cursor (Arr *params);

/// Makes 'w' to get focus.
/// \<wg> -> <wg>
Exp *libkgtk_wg_focus (Arr *params);

/// Set horizontal alignement of 'w'.
///   w    : Widget to set.
///   align: Can be "left", "right" or "center".
/// \<wg>, s -> <wg>
Exp *libkgtk_wg_halign (Arr *params);

/// Set horizontal expansion of 'w'.
///   w    : Widget to set.
///   value: Expand value.
/// \<wg>, b -> <wg>
Exp *libkgtk_wg_hexpand (Arr *params);

/// Add a listener type 'event' to 'w'.
///   w: Widget to set.
///   event: One of 'Events'
///   fn   : Function to execute.
/// \<wg>, s, (\->()|\*->()) -> <wg>
Exp *libkgtk_wg_on (Arr *params);

/// Add a listener of mouse events.
///   w    : Widget to set.
///   event: One of mouse events (pressed, ...)
///   fn   : Function to execute. Its parameters are:
///           npress(i), x(f) and y(f).
/// \<wg>, s, \i,f,f->() -> <wg>
Exp *libkgtk_wg_on_mouse (Arr *params);

/// Makes 'w' to be able to get focus.
/// \<wg>, b -> <wg>
Exp *libkgtk_wg_set_focusable (Arr *params);

/// Makes 'w' to be able to get focus when clicked.
/// \<wg>, b -> <wg>
Exp *libkgtk_wg_set_focusable_on_click (Arr *params);

/// Set vertical alignement of 'w'.
///   w    : Widget to set.
///   align: Can be "top", "bottom" or "middle".
/// \<wg>, s -> <wg>
Exp *libkgtk_wg_valign (Arr *params);

/// Set vertical expansion of 'w'.
///   w    : Widget to set.
///   value: Expand value.
/// \<wg>, b -> <wg>
Exp *libkgtk_wg_vexpand (Arr *params);

#endif
