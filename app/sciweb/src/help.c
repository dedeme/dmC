// Copyright 09-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "help.h"

void help (void) {
  puts (
    "Use: sciweb <action> <params>\n\n"
    "Where 'action parmams' must be\n\n"
    "  book <name>\n"
    "  Initializes a new book creating a directory and initial files.\n"
    "  Example:\n"
    "    sciweb book CSS1\n\n"
    "  update\n"
    "  Update a book using book.js\n"
    "  Example:\n"
    "    sciweb update\n\n"
  );
}
