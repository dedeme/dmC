// Copyright 09-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tpls/book_index.h"

char *book_index (void) {
  return ""
    "<!doctype html>\n"
    "<html>\n"
    "<head>\n"
    "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n"
    "<meta name=\"lang\" content=\"es\" />\n"
    "<meta name=\"author\" content=\"ºDeme.\" />\n"
    "<link rel=\"icon\" type=\"image/png\" href=\"../../img/favicon.png\" />\n"
    "<link rel=\"stylesheet\" href=\"../../styles.css\" type=\"text/css\" />\n"
    "<script type=\"text/javascript\" src=\"../../libsci.js\"></script>\n"
    "<script type=\"text/javascript\" src=\"../globals.js\"></script>\n"
    "<script type=\"text/javascript\" src=\"book.js\"></script>\n"
    "<script type=\"text/javascript\">\n"
    "  const unit = null;\n"
    "  const exercise = null;\n"
    "</script>\n"
    "<script type=\"text/javascript\">\n"
    "window.onload = () => {\n"
    "  divs();\n"
    "};\n"
    "</script>\n"
    "\n"
    "</head>\n"
    "<body>\n"
    "<div id=\"HEADER\"></div>\n"
    "\n"
    "<h2>Índice</h2>\n"
    "\n"
    "<div id=\"PG_INDEX\"></div>\n"
    "\n"
    "</body>\n"
    "</html>\n"
  ;
}
