// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing files

#ifndef DM_FILE_H
# define DM_FILE_H

/// file_cwd returns the working directory<br>
/// Throws exception
char *file_cwd ();

/// file_mkdir makes a directory with 0755 permissions.<br>
/// If parent directory does not exist it creates it.<br>
/// If 'path' already exists it does nothing.<br>
/// Throws exception
void file_mkdir (char *path);

#endif
