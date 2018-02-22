// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>


#include <dm.h>
#include <Cgi.h>
#include "BolsaData.h"

static char *app_name = "BolsaData";
static char *data_version = "201711";
static char *app_dir = "dmcgi/BolsaData";
static time_t expiration = 3600;

static char *mk_date() {
  return date_format(date_now(), "%Y%m%d");
}

static char *mk_date2() {
	Date t = date_now();
  return str_printf("%s-%ld", date_format(t, "%Y%m%d"), t);
}

static void bolsa_data_init(Cgi *cgi) {
	char *dir = path_cat(cgi_home(cgi), "data", NULL);

  if (!file_exists(dir)) {
    file_mkdir(dir);
    char *version = str_printf(
      "%s\nData version: %s\n", app_name, data_version
    );
    char *fversion = path_cat(dir, "version.txt", NULL);
    file_write(fversion, version);
    file_mkdir(path_cat(cgi_home(cgi), "tmp", NULL));
    file_mkdir(path_cat(cgi_home(cgi), "trash", NULL));
  }
}

static Arr/*Json*/ *read_trash(Cgi *cgi) {
  Arr/*char*/ *fs = file_dir(path_cat(cgi_home(cgi), "trash", NULL));
  Arr/*char*/ *rs = arr_new();
  EACH(fs, char, f) {
    arr_add(rs, json_wstring(path_name(f)));
  }_EACH
	return rs;
}

static void to_trash(Cgi *cgi) {
  char *date = mk_date2();
  char *source = path_cat(cgi_home(cgi), "data", NULL);
  char *target = path_cat(
    cgi_home(cgi), "trash", str_printf("%s.zip", date), NULL
  );
  ext_zip(source, target);
}

static void clear_tmp(Cgi *cgi) {
  char *dir = path_cat(cgi_home(cgi), "tmp", NULL);
  file_del(dir);
  file_mkdir(dir);
}

static char *unzip(Cgi *cgi) {
  char *source = path_cat(cgi_home(cgi), "tmp", "back.zip", NULL);
  char *target = path_cat(cgi_home(cgi), "tmp", NULL);
  char *fail = "";
  TRY {
    ext_unzip(source, target);
  } CATCH(e) {
    fail = "restore:unzip";
  }_TRY
  if (*fail) {
    return fail;
  }

  source = path_cat(cgi_home(cgi), "tmp", "data", "version.txt", NULL);
  if (!file_exists(source)) {
    return "restore:version does not exist";
  } else {
    char *good_version =str_printf(
      "%s\nData version: %s\n", app_name, data_version
    );
    char *version = file_read(source);
    if (strcmp(version, good_version)) {
      return "restore:version is wrong";
    } else {
      return "";
    }
  }
  return "";
}

static CgiRp *app_process(Cgi *cgi, char *session_id, Map/*Json*/ *rqm) {
  char *home = cgi_home(cgi);
  char *rq = jmap_gstring(rqm, "rq");
  // ------------------------------------------------------- chpass
  if (!strcmp(rq, "chpass")) {
    return cgi_change_pass(
      cgi,
      jmap_gstring(rqm, "user"),
      jmap_gstring(rqm, "pass"),
      jmap_gstring(rqm, "newPass")
    );

  // ------------------------------------------------------- getDb
  } else if (!strcmp(rq, "getDb")) {
    char *db_path = path_cat(home, "data", "data.db", NULL);

    Map/*Json*/ *m = map_new();
    if (file_exists(db_path)) {
      jmap_pstring(m, "db", file_read(db_path));
    } else {
      jmap_pobject(m, "db", map_new());
    }
    jmap_parray(m, "trash", read_trash(cgi));
    return cgi_ok(cgi, m);

  // ------------------------------------------------------- setDb
  } else if (!strcmp(rq, "setDb")) {
    file_write(
      path_cat(home, "data", "data.db", NULL),
      jmap_gstring(rqm, "db")
    );

    return cgi_ok(cgi, map_new());

  // ------------------------------------------------------- getQuotes
  } else if (!strcmp(rq, "getQuotes")) {
    char *file = str_printf("%s.db", jmap_gstring(rqm, "nick"));
    char *path = path_cat(home, "data", file, NULL);

    Map/*Json*/ *m = map_new();
    if (file_exists(path)) {
      jmap_pstring(m, "quotes", file_read(path));
    } else {
      jmap_pstring(m, "quotes", "");
    }

    return cgi_ok(cgi, m);

  // ------------------------------------------------------- setQuotes
  } else if (!strcmp(rq, "setQuotes")) {
    char *file = str_printf("%s.db", jmap_gstring(rqm, "nick"));
    file_write(
      path_cat(home, "data", file, NULL),
      jmap_gstring(rqm, "quotes")
    );

    return cgi_ok(cgi, map_new());

  // ------------------------------------------------------- getSource
  } else if (!strcmp(rq, "getSource")) {
    char*url = jmap_gstring(rqm, "url");
    Arr/*char*/ *page = ext_wget(url);

    char *fail = "";
    if (!arr_size(page)) {
      fail = str_printf("Wget error reading page '%s'", url);
    }

    Map/*Json*/ *m = map_new();
    jmap_pstring(m, "fail", fail);
    jmap_pstring(m, "page", str_join(it_from(page), ""));
    return cgi_ok(cgi, m);

  // ------------------------------------------------------- backup
  } else if (!strcmp(rq, "backup")) {
    clear_tmp(cgi);
    char *name = str_printf("BolsaDataBackup%s.zip", mk_date());

    ext_zip(
      path_cat(home, "data", NULL),
      path_cat(home, "tmp", name, NULL)
    );

    Map/*Json*/ *m = map_new();
    jmap_pstring(m, "name", name);
    return cgi_ok(cgi, m);

  // ------------------------------------------------------- restoreStart
  } else if (!strcmp(rq, "restoreStart")) {
    clear_tmp(cgi);
    LckFile *lck = file_wopen(path_cat(home, "tmp", "back.zip", NULL));
    file_close(lck);

    return cgi_ok(cgi, map_new());

  // ------------------------------------------------------- restoreAppend
  } else if (!strcmp(rq, "restoreAppend")) {
    LckFile *lck = file_aopen(path_cat(home, "tmp", "back.zip", NULL));
    file_write_bin(lck, b64_decode_bytes(jmap_gstring(rqm, "data")));
    file_close(lck);

    return cgi_ok(cgi, map_new());

  // ------------------------------------------------------- restoreAbort
  } else if (!strcmp(rq, "restoreAbort")) {
    clear_tmp(cgi);

    return cgi_ok(cgi, map_new());

  // ------------------------------------------------------- restoreEnd
  } else if (!strcmp(rq, "restoreEnd")) {
    char *fail = unzip(cgi);

    if (!*fail) {
      char *data = path_cat(home, "data", NULL);

      to_trash(cgi);
      file_del(data);
      file_rename(path_cat(home, "tmp", "data", NULL), data);
      clear_tmp(cgi);
    }

    Map/*Json*/ *m = map_new();
    jmap_pstring(m, "fail", fail);
    return cgi_ok(cgi, m);

  // ------------------------------------------------------- clearTrash
  } else if (!strcmp(rq, "clearTrash")) {
    char *path = path_cat(home, "trash", NULL);
    file_del(path);
    file_mkdir(path);

    return cgi_ok(cgi, map_new());

  // ------------------------------------------------------- restoreTrash
  } else if (!strcmp(rq, "restoreTrash")) {
    char *source = path_cat(home, "trash", jmap_gstring(rqm, "file"), NULL);

    if (!file_exists(source)) {
      return cgi_error(cgi, str_printf("Trash back '%s' not found", source));
    }

    to_trash(cgi);
    file_del(path_cat(home, "data", NULL));
    ext_unzip(source, home);

    return cgi_ok(cgi, map_new());

  // ------------------------------------------------------- logout
  } else if (!strcmp(rq, "logout")) {
    return cgi_del_session(cgi, session_id);

  // ------------------------------------------------------- error!
  } else {
    return cgi_error(cgi, str_printf("'%s': Unknown request", rq));
  }
}

int main (int argc, char **argv) {
  exc_init();
  TRY {
    if (argc != 2) {
      THROW "argc must be 2" _THROW
    }
    Cgi *cgi = cgi_new(app_dir, expiration);

    bolsa_data_init(cgi);

    char *rq = argv[1];
    int ix = str_cindex(rq, ':');
    CgiRp *rp;
    if (ix == -1) { //............................................. CONNECTION
      cgi_set_key(cgi, rq);
      rp = cgi_connect(cgi, rq);
   } else if (ix == 0) { //................................... AUTHENTICATION
      char *key = cryp_key(app_name, cgi_klen());
      cgi_set_key(cgi, key);

      char *data = cryp_decryp(key, rq + 1);
      Arr/*char*/ *parts = str_csplit(data, ':');
      rp = cgi_authentication(
        cgi,
        (char *)arr_get(parts, 0),
        (char *)arr_get(parts, 1),
        *(char *)arr_get(parts, 2) == '1'
      );
    } else { //................................................... NORMAL DATA
      char *session_id = str_sub(rq, 0, ix);
      char *key;
      char *connectionId;
      cgi_get_session_data(&key, &connectionId, cgi, session_id);

      if (!*key) {
        cgi_set_key(cgi, "nosession");
        rp = cgi_expired(cgi);
      } else {
        cgi_set_key(cgi, key);
        Map/*Json*/ *m = json_robject(
          cryp_decryp(key, str_sub_end(rq, ix + 1))
        );
        if (
          map_has_key(m, "connectionId") &&
          strcmp(connectionId, jmap_gstring(m, "connectionId"))
        ) {
          cgi_set_key(cgi, "nosession");
          rp = cgi_expired(cgi);
        } else {
          rp = app_process(cgi, session_id, m);
        }
      }
    }

    printf("%s", rp);
  } CATCH (e) {
    puts (e);
  }_TRY

  return 0;
}

