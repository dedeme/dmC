// Copyright 13-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>


#include "JsDoc.h"
#include "utils.h"
#include <dm.h>

static char *app_name = "JsDoc";
static char *app_dir = "dmcgi/JsDoc";
static time_t expiration = 3600;


CgiRp *app_process(Cgi *cgi, char *session_id, Map/*Json*/ *m) {
  char *page =jmap_gstring(m, "page");

//            ____________
  if (!strcmp(page, "Main")) {
    char *rq = jmap_gstring(m, "rq");
    if (!strcmp(rq, "get")) {
      return send_conf(cgi);
    } else if (!strcmp(rq, "set")) {
      return set_conf(cgi, jmap_garray(m, "conf"));
    } else if (!strcmp(rq, "logout")) {
      return cgi_del_session(cgi, session_id);
    } else {
      return cgi_error(
        cgi, str_printf("'%s': Unknown option in page Main", rq)
      );
    }
//                   _____________
  } else if (!strcmp(page, "Paths")) {
    char *rq = jmap_gstring(m, "rq");
    if (!strcmp(rq, "get")) {
      return send_paths(cgi);
    } else if (!strcmp(rq, "setConf")) {
      return set_conf(cgi, jmap_garray(m, "conf"));
    } else if (!strcmp(rq, "setPaths")) {
      return set_paths(cgi, jmap_garray(m, "paths"));
    } else if (!strcmp(rq, "chpass")) {
      return cgi_change_pass(
        cgi,
        jmap_gstring(m, "user"),
        jmap_gstring(m, "pass"),
        jmap_gstring(m, "newPass")
      );
    } else {
      return cgi_error(
        cgi, str_printf("'%s': Unknown option in page Paths", rq)
      );
    }
//                   _____________
  } else if (!strcmp(page, "Index")) {
    char *rq = jmap_gstring(m, "rq");
    if (!strcmp(rq, "path")) {
      return send_index_tree(cgi, jmap_gstring(m, "path"));
    } else {
      return cgi_error(
        cgi, str_printf("'%s': Unknown option in page Index", rq)
      );
    }
//                   ______________
  } else if (!strcmp(page, "Module")) {
    char *rq = jmap_gstring(m, "rq");
    if (!strcmp(rq, "code")) {
      return send_file(cgi, jmap_gstring(m, "path"));
    } else {
      return cgi_error(
        cgi, str_printf("'%s': Unknown option in page Module", rq)
      );
    }
//  ____
  } else {
    return cgi_error(cgi, str_printf("'%s': Unknown page", page));
  }
}

int main (int argc, char **argv) {
  exc_init();

  TRY {
    if (argc != 2) {
      THROW "argc must be 2" _THROW
    }
    Cgi *cgi = cgi_new(app_dir, expiration);

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

