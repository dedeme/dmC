// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing file paths

#ifndef DM_PATH_H
  #define DM_PATH_H

/// path_name returns name and extension of path.<br>
/// If path is "" or ends at ("/") it returns "".
char *path_name (char *path);

/// path_cat concatenates paths. Variable argumens must finish with NULL.
char *path_cat (char *s, ...);

/// path_parnet puts the parent path of 'path'.<br>
/// If 'path' is "" or "/" 'target' is an empty string.
char *path_parent (char *path);

#endif
