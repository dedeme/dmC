// Copyright 27-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Object for mapping indexes to files.

#ifndef FILEIX_H
  #define FILEIX_H

/// Initialize code file paths array.
void fileix_init(void);

/// Set the program working directory.
void fileix_set_root (char *root);

/// Returns the program working directory.
char *fileix_get_root (void);

/// Adds 'ipath' to code file paths array and returns its index.
/// If path is not found, it returns -1.
/// If path already has been added, it do nothing and also returns its index.
///    fix  : Index of file which imports 'ipath' or -1 if such file is the main
///           one.
///    ipath: Relative file path without the extesion ".kut":
///             * If 'fix' == -1, 'root/ipath' is used (used for 'main' module).
///             * Otherwise:
///               · If parent(fix)/ipath exists, such path is used.
///               · Otherwise root/ipath is used.
int fileix_add(int fix, char *ipath);

/// Returns the path with index 'ix' to print in error messages. It is with
/// extension and made canonical.
char *fileix_to_str (int ix);

/// Read the file with index 'ix'
/// Throw EXC_IO if file can not be found.
char *fileix_read (int ix);

#endif
