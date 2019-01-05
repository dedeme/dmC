// Copyright 24-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Freader.h"

struct freader_Freader {
  int nline;
  char *fname;
  FileLck *lck;
};

Freader *freader_new(const char *file) {
  Freader *this = malloc(sizeof(Freader));
  this->nline = 0;
  this->lck = file_ropen(file);
  this->fname = str_new(file);
  path_name(&this->fname);
  return this;
}

void freader_close(Freader *this) {
  free(this->fname);
  file_close(this->lck);
  free(this);
}

char *freader_line_new(Freader *this) {
  ++this->nline;
  return file_read_line_new(this->lck);
}

void freader_msg(char **final_msg, Freader *this, char *msg) {
  *final_msg = str_f_new("%s:%d:\n  %s", this->fname, this->nline, msg);
}
