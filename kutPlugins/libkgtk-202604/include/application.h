// Copyright 09-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Gtk application.

#include "exp.h"

#ifndef APPLICATION_H
  #define APPLICATION_H

/// Execute an expression of type object<plclosure>.
///   cl: Expression of type function.
///   params: Arr<Exp>. Parameters for 'cl'.
void application_run_closure (Exp *cl, Arr *params);

/// Create a new GTK application.
///   id      : Application identifier.
///   clrunner: Function to execute expressions of type object<plclosure>.
/// \s -> <gtkApplication>
Exp *libkgtk_application_new (Arr *params);

/// Close a new GTK application and free memory.
///   app: Application to close.
/// \<gtkApplication> -> ()
Exp *libkgtk_application_close (Arr *params);

/// Start 'app' with arguments.
///   app : Application to close.
///   args: Application arguments.
/// \<gtkApplication>, \->() -> ()
Exp *libkgtk_application_run (Arr *params);

#endif
