// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js/wclass.h"
#include "js/wval.h"
#include "js/wfun.h"

/*
     function Teacher(first, last, age, gender, interests, subject) {
        Person.call(this, first, last, age, gender, interests);
        this.subject = subject;
      }
      Teacher.prototype = Object.create(Person.prototype);
      Teacher.prototype.constructor = Teacher;
*/

void wclass_start(Class *class) {
  Cpath *cp = class_cpath(class);
  Cvalue *new = NULL;
  EACH(class_cvalues(class), Cvalue, v) {
    if (!strcmp(cvalue_id(v), "new")) {
      new = v;
      break;
    }
  }_EACH

  char *tx;
  if (new) {
    THROW "Without implementation" _THROW
  } else {
    tx = str_printf("function %s() {}\n", cpath_id(cp));
  }
  file_write(cpath_js(cp), tx);
}

void wclass_end(Class *class) {
  LckFile *lck = file_aopen(cpath_js(class_cpath(class)));
  file_write_text(lck, "}\n");
  file_close(lck);
}

void wclass_object(Class *class, Map/*char*/ *imports, Cvalue *cv) {
  LckFile *lck = file_aopen(cpath_js(class_cpath(class)));
  file_write_text(lck, cpath_id(class_cpath(class)));
  if (!cvalue_is_static(cv)) {
    file_write_text(lck, ".prototype");
  }
  file_write_text(lck, str_printf(".%s = ", cvalue_id(cv)));

  Dvalue *dv = cvalue_dvalue(cv);
  if (type_type(dvalue_type(dv)) == FN) {
    wfun_write(lck, imports, dvalue_value(dv));
  } else {
    wval_write(lck, imports, dvalue_value(dv));
  }

  file_write_text(lck, ";\n\n");

  file_close(lck);
}
