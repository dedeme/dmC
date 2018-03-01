// Copyright 13-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dm/dm.h>

typedef struct pos_Pos Pos;

struct pos_Pos {
  char *file;
  uint line;
};

static Pos *pos_new(char *file, uint line) {
  Pos *this = MALLOC(Pos);
  this->file = file;
  this->line = line;
  return this;
}

static void help(char *msg) {
	printf("%s\n"
		"Use:\n"
		"  jsi18n <languages> <roots> <target>\n"
		"For example:\n"
		"  jsi18n \\\n"
    "         \"en:es_ES\" \\\n"
    "         \"/deme/dmjs18/src/client/bet:/deme/dmjs18/src/lib/basic\" \\\n"
    "         \"/deme/dmjs18/src/client/bet\"\n"
		"Note that different languages and roots are separated by ':'\n\n",
    msg
  );
}

static char *rdoc(char *tx) {
  return str_replace(str_replace(str_trim(tx), "\\\"", "\""), "\"", "\\\"");
}

static void extract_file(Map/*Arr[Pos]*/ *keys, char *file) {
  enum Status {
    CODE,
    PRECOMMENT,
    LCOMMENT,
    BCOMMENT,
    BCOMMENT2,
    QUOTE,
    QUOTE2,
    SQUOTE,
    ENTRY1,
    ENTRY2,
    EQUOTE,
    EQUOTE2
  };

  enum Status code_state(char ch) {
    switch(ch) {
    case '/': return PRECOMMENT;
    case '"': return QUOTE;
    case '\'': return SQUOTE;
    case '_': return ENTRY1;
    default: return CODE;
    }
  }

  Buf *bf = buf_new();
  enum Status state = CODE;
  uint nl = 1;
  LckFile *lck = file_ropen (file);
  char *l = file_read_line(lck);
  while (*l) {
    char *pl = l;
    while (*pl) {
      char ch = *pl++;
      switch (state) {
      case PRECOMMENT:
        switch (ch){
        case '/':
          state = LCOMMENT;
          break;
        case '*':
          state = BCOMMENT;
          break;
        default:
          state = code_state(ch);
        }
        break;
      case LCOMMENT:
        if (ch == '\n') {
          state = CODE;
        }
        break;
      case BCOMMENT:
        if (ch == '*') {
          state = BCOMMENT2;
        }
        break;
      case BCOMMENT2:
        if (ch == '/') {
          state = CODE;
        } else {
          state = BCOMMENT;
        }
        break;
      case QUOTE:
        switch (ch) {
        case '"':
        case '\n':
          state = CODE;
          break;
        case '\\':
          state = QUOTE2;
          break;
        }
        break;
      case QUOTE2:
        state = QUOTE;
        break;
      case SQUOTE:
        switch (ch) {
        case '\'':
        case '\n':
          state = CODE;
          break;
        case '\\':
          state = QUOTE2;
          break;
        }
        break;
      case ENTRY1:
        if (ch == '(') {
          state = ENTRY2;
        } else {
          state = code_state(ch);
        }
        break;
      case ENTRY2:
        if (ch == '"') {
          state = EQUOTE;
        } else {
          state = code_state(ch);
        }
        break;
      case EQUOTE:
        switch (ch) {
        case '"':
        case '\n':
          if (ch == '"') {
            char *key = buf_str(bf);
            if (str_cindex(key, '=') == -1) {
              Pos *pos = pos_new(file, nl);
              Arr/*Pos*/ *value = map_get(keys, key);
              if (value) {
                arr_add(value, pos);
              } else {
                value = arr_new();
                arr_add(value, pos);
                map_put(keys, key, value);
              }
            }
          }
          bf = buf_new();
          state = CODE;
          break;
        case '\\':
          buf_cadd(bf, ch);
          state = EQUOTE2;
          break;
        default:
          buf_cadd(bf, ch);
        }
        break;
      case EQUOTE2:
        buf_cadd(bf, ch);
        state = EQUOTE;
        break;
      default:
        state = code_state(ch);
      }
    }

    l = file_read_line(lck);
    ++nl;
  }

  file_close(lck);
}

static void extract(Map/*Arr[Pos]*/ *keys, char *source) {
  puts(source);
  if (file_is_directory(source)) {
    Arr/*char*/ *files = file_dir(source);
    EACH(files, char, f) {
      extract(keys, f);
    }_EACH
  } else if (file_exists(source)) {
    if (str_ends(source, ".js")) {
      extract_file(keys, source);
    }
  } else {
    THROW "'%s' does not exist\n", source _THROW
  }
}

static char *make_dic(Map/*Arr[Pos]*/ *keys, char *lang, char *target) {
  char *tdir = path_cat(target, "i18n", NULL);
  if (!file_exists(tdir)) {
    file_mkdir(tdir);
  }

  if (!file_is_directory(tdir)) {
    THROW "'%s' is not a directory\n", tdir _THROW
  }

  char *dic_path = path_cat(tdir, str_printf("%s.txt", lang), NULL);
  if (!file_exists(dic_path)) {
    file_write(dic_path, "");
  }

  Map/*char*/ *old_dic = map_new();
  char *text = file_read(dic_path);
  Arr/*char*/ *entries = str_csplit(text, '\n');
  EACH(entries, char, e) {
    char *trim = str_trim(e);
    if (!*trim || *trim == '#') {
      continue;
    }
    int ix = str_cindex(trim, '=');
    if (ix == -1) {
      continue;
    }
    char *key = rdoc(str_sub(trim, 0, ix));
    char *value = rdoc(str_sub_end(trim, ix + 1));
    if (*key && *value) {
      map_put(old_dic, key, value);
    }
  }_EACH

	Buf *orphan = buf_new();
	Buf *todo = buf_new();
	Buf *trans = buf_new();
	Buf *dic = buf_new();

  bool sort(void *e1, void *e2) {
    Kv *kv1 = e1;
    Kv *kv2 = e2;
    return strcmp(kv1->key, kv2->key) > 0;
  }
  arr_sort((Arr *)keys, sort);
  arr_sort((Arr *)old_dic, sort);

  EACH((Arr *)keys, Kv, kv) {
    char *k = kv ->key;
    Arr/*Pos*/ *poss = kv->value;
    char *v = map_get(old_dic, k);
    if (v) {
      EACH(poss, Pos, p) {
        buf_add(trans, str_printf("# %s: %d\n", p->file, p->line));
      }_EACH
      buf_add(trans, str_printf("%s = %s\n\n", k, v));
      buf_add(dic, str_printf("    \"%s\": \"%s\",\n", k, v));

      map_remove(old_dic, k);
    } else {
      buf_add(todo, "# TO DO\n");
      EACH(poss, Pos, p) {
        buf_add(todo, str_printf("# %s: %d\n", p->file, p->line));
      }_EACH
      buf_add(todo, k);
      buf_add(todo, " = \n\n");
    }
  }_EACH

  EACH((Arr *)old_dic, Kv, kv) {
    buf_add(orphan, str_printf(
      "# ORPHAN\n%s = %s\n\n", kv->key, (char *)kv->value
    ));
  }_EACH

  LckFile *lck = file_wopen(dic_path);
 	file_write_text(lck, "# File generated by jsi18n.\n\n");
	file_write_text(lck, buf_str(orphan));
	file_write_text(lck, buf_str(todo));
	file_write_text(lck, buf_str(trans));
  file_close(lck);

  if (*buf_str(dic)) {
    return str_printf("%s\n", str_sub(buf_str(dic), 0, buf_length(dic) - 2));
  }
  return "";
}

int main (int argc, char **argv) {
  exc_init();

  if (argc != 4) {
    help("Wrong number of parameters calling jsi18n");
    return 0;
  }

  Arr/*char*/ *langs = str_csplit(argv[1], ':');
  Arr/*char*/ *sources = str_csplit(argv[2], ':');
  char *target = argv[3];

  if (!file_is_directory(target)) {
    THROW "'%s' is not a directory\n", target _THROW
  }

  Map/*Arr[Pos]*/ *current_keys = map_new();
  EACH(sources, char, source) {
    extract(current_keys, source);
  }_EACH

  char *jstarget_dir = path_cat(target, "src", NULL);
  if (!file_is_directory(jstarget_dir)) {
    THROW "'%s' is not a directory\n", jstarget_dir _THROW
  }

  char *jstarget = path_cat(target, "src", "i18n.js", NULL);
  LckFile *lck = file_wopen (jstarget);

  file_write_text(lck,
    "// Generate by jsi18n. Don't modify\n"
    "\n"
    "goog.provide(\"I18n\");\n"
    "\n"
    "{\n"
    "  let lang = {};\n\n"
  );

  EACH(langs, char, lang) {
    char *dic = make_dic(current_keys, lang, target);
    char *tx = str_printf(
      "  const %s = {\n"
			"%s"
			"  };\n\n",
      lang, dic);
		file_write_text(lck, tx);
  }_EACH

  file_write_text(lck,
    "  I18n = class {\n"
    "  /** @return {void} */\n"
    "  static en () {\n"
    "    lang = en;\n"
    "  }\n"
    "\n"
    "  /** @return {void} */\n"
    "  static es () {\n"
    "    lang = es;\n"
    "  }\n"
    "\n"
    "  /**\n"
    "   * @private\n"
    "   * @return {!Object<string, string>}\n"
    "   */\n"
    "  static lang () {\n"
    "    return lang;\n"
    "  }\n"
    "}}\n"
    "\n"
    "function _(key) {\n"
    "  let v = I18n.lang()[key];\n"
    "  if (v !== undefined) {\n"
    "    return v;\n"
    "  }\n"
    "  return key;\n"
    "}\n"
    "\n"
    "function _args(key, ...args) {\n"
    "  let bf = \"\";\n"
    "  let v = _(key);\n"
    "  let isCode = false;\n"
    "  for (let i = 0; i < v.length; ++i) {\n"
    "    let ch = v.charAt(i);\n"
    "    if (isCode) {\n"
    "      bf += ch === \"0\" ? args[0]\n"
    "        : ch === \"1\" ? args[1]\n"
    "        : ch === \"2\" ? args[2]\n"
    "        : ch === \"3\" ? args[3]\n"
    "        : ch === \"4\" ? args[4]\n"
    "        : ch === \"5\" ? args[5]\n"
    "        : ch === \"6\" ? args[6]\n"
    "        : ch === \"7\" ? args[7]\n"
    "        : ch === \"8\" ? args[8]\n"
    "        : ch === \"9\" ? args[9]\n"
    "        : ch === \"%\" ? \"%\"\n"
    "        : \"%\" + ch;\n"
    "      isCode = false;\n"
    "    } else {\n"
    "      if (ch === \"%\") {\n"
    "        isCode = true;\n"
    "      } else {\n"
    "        bf += ch\n"
    "      }\n"
    "    }\n"
    "  }\n"
    "  return bf;\n"
    "}\n"
  );


  file_close(lck);
  return 0;
}
