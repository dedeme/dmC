// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>


#include <dm.h>
#include <Cgi.h>
#include <sys/stat.h>
#include "FleasData.h"

static char *app_name = "FleasData";
static char *data_version = "201712";
static char *app_dir = "dmcgi/FleasData";
static time_t expiration = 1800;  // 1/2 hour

static char *mk_date() {
  return date_format(date_now(), "%Y%m%d");
}

static char *mk_date2() {
	Date t = date_now();
  return str_printf("%s-%ld", date_format(t, "%Y%m%d"), t);
}

static void fleas_data_init(Cgi *cgi) {
  char *fleas = path_cat(app_dir, "fleas", NULL);

  if (!file_exists(fleas)) {
    THROW "'fleas' directory is missing" _THROW
  }

  char *data = path_cat(app_dir, "data", NULL);
  if (!file_exists(data)) {
    file_mkdir(data);

    file_write(
      path_cat(data, "version.txt", NULL),
      str_printf("%s\nData version: %s\n", app_name, data_version)
    );

    file_write(
      path_cat(data, "conf.db", NULL),
      json_warray(arr_new())
    );
  }

  file_mkdir(path_cat(app_dir, "tmp", NULL));
  file_mkdir(path_cat(app_dir, "backups", NULL));
  file_mkdir(path_cat(app_dir, "trash", NULL));
}

static Arr/*Json*/ *read_backups(Cgi *cgi) {
  Arr/*char*/ *fs = file_dir(path_cat(app_dir, "backups", NULL));

  Arr/*Json*/ *rs = arr_new();
  EACH(fs, char, f) {
    arr_add(rs, json_wstring(path_name(f)));
  }_EACH

	return rs;
}

static void filter_backups(Cgi *cgi) {
  Date t0 = date_now();
  Date t1 = date_add(t0, -7);
  Date t2 = date_new(date_day(t0), date_month(t0), date_year(t0) - 1);

  char *d1 = date_format(t1, "%Y%m%d");
  char *d2 = date_format(t2, "%Y%m%d");

  Arr/*char*/ *fs = file_dir(path_cat(app_dir, "backups", NULL));
  arr_sort_str(fs);
  char *previous = "        ";
  EACH(fs, char, f) {
    char *name = path_only_name(f);
    if (strcmp(name, d2) < 0) {
      if (!strcmp(str_sub(name, 0, 4), str_sub(previous, 0, 4))) {
        file_del(f);
      }
    } else if (strcmp(name, d1) < 0) {
      if (!strcmp(str_sub(name, 0, 6), str_sub(previous, 0, 6))) {
        file_del(f);
      }
    }
    previous = name;
  }_EACH
}

static Arr/*Json*/ *read_trash(Cgi *cgi) {
  Arr/*char*/ *fs = file_dir(path_cat(app_dir, "trash", NULL));

  Arr/*Json*/ *rs = arr_new();
  EACH(fs, char, f) {
    arr_add(rs, json_wstring(path_name(f)));
  }_EACH

	return rs;
}

static void to_trash(Cgi *cgi) {
  ext_zip(
    path_cat(app_dir, "data", NULL),
    path_cat(app_dir, "trash", str_printf("%s.zip", mk_date2()), NULL)
  );
}

static void clear_tmp(Cgi *cgi) {
  char *dir = path_cat(app_dir, "tmp", NULL);
  file_del(dir);
  file_mkdir(dir);
}

static char *unzip(Cgi *cgi) {
  char *fail = "";
  TRY {
    ext_unzip(
      path_cat(app_dir, "tmp", "back.zip", NULL),
      path_cat(app_dir, "tmp", NULL)
    );
  } CATCH(e) {
    fail = "restore:unzip";
  }_TRY
  if (*fail) {
    return fail;
  }

  char *source = path_cat(cgi_home(cgi), "tmp", "data", "version.txt", NULL);
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
  // ------------------------------------------------------- getConf
  if (!strcmp(rq, "getConf")) {
    char *data = file_read(path_cat(home, "data", "conf.db", NULL));

    Map/*Json*/ *m = map_new();
    jmap_pstring(m, "conf", data);
    return cgi_ok(cgi, m);

  // ------------------------------------------------------- setConf
  } else if (!strcmp(rq, "setConf")) {
    file_write(
      path_cat(home, "data", "conf.db", NULL),
      jmap_gstring(rqm, "conf")
    );
    return cgi_ok(cgi, map_new());

  // ------------------------------------------------------- readBackLists
  } else if (!strcmp(rq, "readBackLists")) {
    Map/*Json*/ *m = map_new();
    jmap_parray(m, "backups", read_backups(cgi));
    jmap_parray(m, "trash", read_trash(cgi));
    return cgi_ok(cgi, m);

  // ------------------------------------------------------- bestsTime
  } else if (!strcmp(rq, "bestsTime")) {
    char *file = path_cat(home, "fleas", "data", "bests1.db", NULL);
    struct stat *st = file_info(file);

    Map/*Json*/ *m = map_new();
    jmap_pint(m, "time", st->st_mtime);
    return cgi_ok(cgi, m);

  // ------------------------------------------------------- readBests
  } else if (!strcmp(rq, "readBests")) {
    char *fname = str_printf("bests%s.db", jmap_gstring(rqm, "ix"));
    char *file = path_cat(home, "fleas", "data", fname, NULL);

    char *bests;
    if (file_exists(file)) {
      bests = file_read(file);
    } else {
      bests = json_warray(arr_new());
    }

    Map/*Json*/ *m = map_new();
    jmap_pstring(m, "bests", bests);
    return cgi_ok(cgi, m);

  // ------------------------------------------------------- readTraces
  } else if (!strcmp(rq, "readTraces")) {
    char *ftraces = path_cat(home, "fleas", "data", "traces.db", NULL);

    Map/*Json*/ *m = map_new();
    if (!file_exists(ftraces)) {
      map_put(m, "traces", json_wnull());
    } else {
      map_put(m, "traces", file_read(ftraces));
    }
    return cgi_ok(cgi, m);

  // ------------------------------------------------------- readQuotes
  } else if (!strcmp(rq, "readQuotes")) {
    char *fname = str_printf("%s.db", jmap_gstring(rqm, "nick"));
    char *file = path_cat(home, "fleas", "bolsa_data", fname, NULL);

    Map/*Json*/ *m = map_new();
    jmap_pstring(m, "quotes", file_read(file));
    return cgi_ok(cgi, m);

  // ------------------------------------------------------- readFleas
  } else if (!strcmp(rq, "readFleas")) {
    Map/*Json*/ *m = map_new();

    char *ix = jmap_gstring(rqm, "ix");
    char *fname = str_printf("fleas%s.db", ix);
    bool ix_is_0 = *ix == '0';

    char *file = path_cat(home, "fleas", "data", fname, NULL);

    char *data;
    if (file_exists(file)) {
      data = file_read(file);
    } else {
      data = json_warray(arr_new());
    }

    jmap_pstring(m, "fleas", data);

    if (ix_is_0) {
      size_t cycle = jmap_guint(
        json_robject(file_read(path_cat(
          home, "fleas", "data", "conf.db", NULL
        ))),
        "cycle"
      );
      jmap_puint(m, "cycle", cycle);
    }

    return cgi_ok(cgi, m);

  // ------------------------------------------------------- chpass
  } else if (!strcmp(rq, "chpass")) {
    return cgi_change_pass(
      cgi,
      jmap_gstring(rqm, "user"),
      jmap_gstring(rqm, "pass"),
      jmap_gstring(rqm, "newPass")
    );

  // ------------------------------------------------------- backup
  } else if (!strcmp(rq, "backup")) {

    clear_tmp(cgi);
    char *name = str_printf("%s%s.zip", app_name, mk_date());

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


  // ------------------------------------------------------- autorestore
  } else if (!strcmp(rq, "autorestore")) {
    to_trash(cgi);

    file_del(path_cat(home, "data", NULL));
    ext_unzip(
      path_cat(home, "backups", jmap_gstring(rqm, "file"), NULL),
      home
    );

    return cgi_ok(cgi, map_new());

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
    ext_zip(
      path_cat(home, "data", NULL),
      path_cat(home, "backups", str_printf("%s.zip", mk_date()), NULL)
    );
    filter_backups(cgi);
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

    fleas_data_init(cgi);

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

