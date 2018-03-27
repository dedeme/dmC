// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "parser/Program.h"
#include "parser/Pclass.h"

/*.
-struct: Program
  cpath: Cpath *
  -
  -classes: Arr * = arr_new()
*/

/*.-.*/
struct program_Program {
  Cpath *cpath;

  Arr *classes;
};

Program *program_new(Cpath *cpath) {
  Program *this = MALLOC(Program);
  this->cpath = cpath;
  this->classes = arr_new();
  return this;
}

inline
Cpath *program_cpath(Program *this) {
  return this->cpath;
}
/*.-.*/

void program_add_class(Program *this, char *path) {

}

void program_print(Program *this, char *jsf) {
  LckFile *js = file_wopen(jsf);
  file_write_text(js,
    "(function () {\n"
    "'use strict'\n"
  );


  file_write_text(js,
    str_printf("%s.main();\n}());\n", cpath_id(this->cpath))
  );

  file_close(js);
}
