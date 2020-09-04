// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dmc/all.h>
#include <sys/stat.h>
#include <time.h>

void help () {
  puts(
    "haxei18n. Version 201712\n\n"
    "Use:\n"
    "haxei18n sourcesDir targetDir [lang1 lang2 ...]\n"
    "For example:\n"
    "haxei18n /home/john/src /home/john/i18n en es\n"
  );
}

int is_directory (char *path) {
  struct stat buf;
  stat(path, &buf);
  if (S_ISDIR(buf.st_mode)) {
    return 1;
  }
  return 0;
}

void backup (char *source_file) {
  char *source_dir = path_parent(source_file);

  Arr/*char*/ *files = file_dir(source_dir);
  arr_sort_str(files);
  int ix__, ix_;
  EACH(files, char, file) {
    ix__ = str_index(file, "__");
    if (ix__ != -1 && str_starts(file, source_file)) {
      ix_ = str_index(file + (ix__ + 2), "_");
      if (ix_ != -1) {
        ix_ += ix__;
        rename(file, str_sub(file, 0, ix_));
      }
    }
  }_EACH

  time_t t = time(NULL);
  struct tm *lt = localtime(&t);
  char *tstr = str_printf("%d", (int)t);
  char *date = str_printf("%d%02d%02d",
    lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);
  char *target = str_printf("%s__%s_%s", source_file, date, tstr);
  file_write(target, file_read(source_file));

  files = file_dir(source_dir);
  arr_sort_str(files);
  ix_ = 0;
  EACH(files, char, file) {
    if (ix_ < 5)
      ix_++;
    else
      file_del(file);
  }_EACH
}

void read_file(char *file, Map/*Arr[char]*/ *data) {
  Arr/*char*/ *lines = str_csplit(file_read(file), '\n');

  int ix;
  EACH(lines, char, l) {
    char *l2 = str_ltrim(l);
    if (!*l2 || *l2 == '#') {
      continue;
    }
    ix = str_index(l2, "=");
    char *key = str_trim(str_sub(l2, 0, ix));
    Arr/*char*/ *avalues = arr_new();

    arr_add(avalues, str_trim(str_sub(l2, ix + 1, strlen(l2))));

    map_put(data, key, avalues);
  }_EACH
}

void read_sources (int len, char *dir, Map/*Arr[char]*/ *data) {
  Arr/*char*/ *files = file_dir(dir);
  EACH(files, char, file) {
    if (is_directory(file)) {
      read_sources(len, file, data);
      continue;
    }
    if (!str_ends(file, ".hx")) {
      continue;
    }

    char *f_name = str_sub(file, len, strlen(file));
    Arr/*char*/ *lines = str_csplit(file_read(file), '\n');
    int nline = 0;
    EACH(lines, char, line) {
      ++nline;
      int ix, q1, q2, q3;
      q3 = 0;
      while (1) {
        ix = str_index(line + q3, "_(");
        if (ix == -1) {
          break;
        }
        ix += q3;
        q1 = str_index(line + (ix + 2), "\"");
        if (q1 == -1) {
          break;
        }
        q1 += ix + 2;
        q2 = q1 + 1;
        while (1) {
          q3 = str_index(line + q2, "\"");
          if (q3 == -1) {
            break;
          }
          q3 += q2;
          if (line[q3 - 1] != '\\') {
            break;
          }
          q2 = q3 + 1;
        }
        if (q3 == -1) {
          break;
        }
        char *key = str_trim(str_sub(line, q1 + 1, q3));
        char *msg = str_printf("%s [%d:%d]", f_name, nline + 1, ix);
        Arr/*char*/ *previous = map_get(data, key);
        if (previous) {
          arr_add(previous, msg);
        } else {
          previous = arr_new();
          arr_add(previous, "");
          arr_add(previous, msg);
          map_put(data, key, previous);
        }
      }
    }_EACH
  }_EACH
}

bool write_sort(void *p1, void *p2) {
  Kv *kv1 = p1;
  char *k1 = kv1->key;
  Arr/*char*/ *vs1 = kv1->value;
  char *v1 = arr_get(vs1, 0);

  Kv *kv2 = p2;
  char *k2 = kv2->key;
  Arr/*char*/ *vs2 = kv2->value;
  char *v2 = arr_get(vs2, 0);

  if (arr_size(vs1) == 1) {
    if (arr_size(vs2) != 1) {
      return -1;
    }
    return strcasecmp(k1, k2);
  }
  if (arr_size(vs2) == 1) {
    return 1;
  }
  if (*v1) {
    if (!*v2) {
      return 1;
    }
    return strcasecmp(k1, k2);
  }
  if (*v2) {
    return -1;
  }
  return strcasecmp(k1, k2) > 0;
}

void write(char *file, Map/*Arr[char]*/ *map, Buf *hx_code) {
  Arr/*Kv[Arr[char]]*/ *data = (Arr *)map;
  arr_sort(data, write_sort);
  Buf *bf1 = buf_new();
  buf_add(bf1, "# File generated file by haxei18n\n");
  EACH(data, Kv, kv) {
    Buf *bf2 = buf_new();
    buf_add(bf1, "\n");
    char *k = kv->key;
    Arr/*char*/ *vs = kv->value;
    char *v = arr_get(vs, 0);
    if (arr_size(vs) == 1) {
      buf_add(bf1, "#ORPHAN\n");
    } else if (!*v) {
      buf_add(bf1, "#TO DO\n");
    }
    buf_add(bf2, k);
    buf_add(bf2, " = ");
    buf_add(bf2, v);
    buf_add(bf2, "\n");

    if (*v) {
      buf_add(hx_code, "      \"");
      buf_add(hx_code, k);
      buf_add(hx_code, " = ");
      buf_add(hx_code, v);
      buf_add(hx_code, "\\n\" +\n");
    }

    arr_remove(vs, 0);

    EACH(vs, char , ref) {
      buf_add(bf1, "#");
      buf_add(bf1, ref);
      buf_add(bf1, "\n");
    }_EACH
    buf_add(bf1, buf_str(bf2));
  }_EACH
  file_write(file, buf_str(bf1));
}

void read_lang (char *dir, char *file, Buf *hx_code) {
  if (!file_exists(dir) || !is_directory(dir)) {
    THROW "'%s' can not be read", dir _THROW
  }

  if (!file_exists(file)) {
    file_write(file, "# File generated file by haxei18n\n");
  }

  backup(file);

  Map/*Arr[chr]*/ *data = map_new();
  read_file(file, data);
  read_sources(strlen(dir) + 1, dir, data);
  write(file, data, hx_code);
}

int main (int argc, char **argv) {
  if (argc < 3) {
    help();
    return 0;
  }
  exc_init();
  char *source = argv[1];
  char *target = argv[2];

  Buf *bf = buf_new();
  buf_add(bf, "// File automatically generated. Don't modify.\n\n");
  buf_add(bf, "class I18nData {");
  RANGE (i, 3, argc) {
    const char *lang = argv[i];

    buf_add(bf, "\n  public static function ");
    buf_add(bf, lang);
    buf_add(bf, " ():String {\n");
    buf_add(bf, "    return \"\" +\n");

    char *file = str_printf("%s/i18n_%s", target, lang);
    read_lang(source, file, bf);

    buf_add(bf, "      \"\";\n");
    buf_add(bf, "  };\n");
  }_RANGE
  buf_add(bf, "}\n\n");

  char *file = str_printf("%s/I18nData.hx", source);
  file_write(file, buf_str(bf));

  return 0;
}
