// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "parser/Pclass.h"

/*.
-struct: Pclass
  path: char *
*/

/*.-.*/
struct pclass_Pclass {
  char *path;
};

Pclass *pclass_new(char *path) {
  Pclass *this = MALLOC(Pclass);
  this->path = path;
  return this;
}

inline
char *pclass_path(Pclass *this) {
  return this->path;
}
/*.-.*/

char *pclass_str(Pclass *this) {
  return "";
}
