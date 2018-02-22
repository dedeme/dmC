// Copyright 13-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Jsdoc utilities

#ifndef UTILS_H
  # define UTILS_H

#include <Cgi.h>

///
CgiRp *send_conf(Cgi *cgi);

///
CgiRp *set_conf(Cgi *cgi, Arr/*Json*/ *data);

///
CgiRp *send_paths(Cgi *cgi);

///
CgiRp *set_paths(Cgi *cgi, Arr/*Json*/ *data);

///
CgiRp *send_index_tree(Cgi *cgi, char *path);

///
CgiRp *send_file(Cgi *cgi, char *path);

#endif



