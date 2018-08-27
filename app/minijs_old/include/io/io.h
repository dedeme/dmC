// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Io utilities

#ifndef IO_IO_H
  # define IO_IO_H

typedef struct cpath_Cpath Cpath;

/// io_check_install checks initial data base status
void io_check_install(void);

/// io_rclass_serial reads a class serialización from library if it exists.
Ochar *io_rclass_serial(Cpath *path);

/// io_rclass_code reads mini code
char *io_rclass_code(Cpath *path);

/// io_wclass writes a serialized class in library
void io_wclass(Cpath *path, char *serial);

#endif
