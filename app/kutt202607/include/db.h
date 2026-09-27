// Copyright 10-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Kutt database.

#ifndef DB_H
  #define DB_H

#include "DEFS.h"
#include "data/indexEntry.h"
#include "data/mdRc.h"

/// Initializes data base and returns the canonical path e identifier of
/// 'fmain'.
/// If the path of 'fmain' is not found, it returns ["", ""].
///   fmain: Main file.
IndexEntry *db_init (char *fmain);

/// Parent of the main file path.
char *db_main_root (void);

/// Returns the canonical path e identifier of a module [path, id]. If the
/// path is not found returns ["", ""].
/// The id is of type md_xxxxxxxx.
IndexEntry *db_get_path_id (char *path);

/// Returns the path of an identifier of module or "" if it is not found.
char *db_get_path (char *mdId);

/// Returns the module (<MdRc>) with identifier 'id' or Opt.none if it
/// is not found.
/// Returns Opt<MdRc>
Opt *db_read_module (char *id);

/// Writes the module 'mod' with the identifier 'id'.
void db_write_module (MdRc *mod, char *id);

/// Returns date of the last module 'mod' modification date or '-1' if such
/// module is not found.
int64_t db_module_date (char *mod);

/// Returns date of the last object 'o' modification date or '-1' if such
/// file is not found.
int64_t db_object_date (char *mod);

/// Removes spare files in cts.compilationPath
void db_clean (void);

/// Removes all the files of cts.compilationPath
void db_delete (void);

#endif
