// Copyright 06-Sep-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// C code data base.

#ifndef TYPED_TDB_H
  #define TYPED_TDB_H

/// 'path' is the path of file '.kut' (without .kut)
void tdb_init (char *path);

/// Returns path of .c file.
char *tdb_c_path ();

/// Returns path of .o file.
char *tdb_o_path ();

/// Returns path of .so library of program in 'kut_path' or "" if it is not
/// found and with_add is FALSE.
///   kut_path: Path of file kut without extension '.kut'.
///   create  : If it is TRUE 'kut_path' is add to data base, if it is not found.
char *tdb_so_path (char *kut_path, int create);

#endif
