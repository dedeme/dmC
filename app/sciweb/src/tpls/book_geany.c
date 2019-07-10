// Copyright 09-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tpls/book_geany.h"

char *book_geany (char *name) {
  return str_f(""
    "[file_prefs]\n"
    "final_new_line=true\n"
    "ensure_convert_new_lines=false\n"
    "strip_trailing_spaces=true\n"
    "replace_tabs=false\n"
    "\n"
    "[indentation]\n"
    "indent_width=2\n"
    "indent_type=0\n"
    "indent_hard_tab_width=8\n"
    "detect_indent=false\n"
    "detect_indent_width=false\n"
    "indent_mode=2\n"
    "\n"
    "[project]\n"
    "name=%s\n"
    "base_path=./\n"
    "\n"
    "[long line marker]\n"
    "long_line_behaviour=1\n"
    "long_line_column=80\n"
    "\n"
    "[build-menu]\n"
    "filetypes=HTML;\n"
    "HTMLFT_00_LB=\n"
    "HTMLFT_00_CM=\n"
    "HTMLFT_00_WD=\n"
    "HTMLFT_02_LB=Browser\n"
    "HTMLFT_02_CM=firefox http://localhost/sci\n"
    "HTMLFT_02_WD=\n"
    "\n"
    "[editor]\n"
    "line_wrapping=false\n"
    "line_break_column=80\n"
    "auto_continue_multiline=false\n",
    name
  );
}
