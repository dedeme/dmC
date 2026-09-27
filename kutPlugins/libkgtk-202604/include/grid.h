// Copyright 27-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Grid widget.

#include "exp.h"

#ifndef GRID_H
  #define GRID_H

/// Creates a new Grid.
/// \ -> <gwg>
Exp *libkgtk_grid_new (Arr *params);

/// Insert 'child' in 'w'.
///   w      : Grid to set.
///   child  : Widget to add.
///   row    : Row index (0-based)
///   col    : Column index (0-based)
///   rowspan: Rows span.
///   colspan: Columns span.
/// \<wg>, <wg>, row, i, i, i -> <wg>
Exp *libkgtk_grid_put_at (Arr *params);

/// Equals to libkgtk_grid_put_at(w, child, 0, 0, 1, 1), but removing the
/// widget that could be there.
///   w      : Grid to set.
///   child  : Widget to add.
/// \<wg>, <wg> -> <wg>
Exp *libkgtk_grid_set (Arr *params);

/// Removes child in position.
///   w      : Grid to set.
///   row    : Row index (0-based)
///   col    : Column index (0-based)
/// \<wg>, i, i -> <wg>
Exp *libkgtk_grid_remove_at (Arr *params);

/// Removes 'child' from 'w' and returns 'w'.
/// \<wg>, <wg> -> <wg>
Exp *libkgtk_grid_remove (Arr *params);

#endif
