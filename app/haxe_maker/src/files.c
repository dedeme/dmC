/*
 * Copyright 06-Mar-2017 ºDeme
 * GNU General Public License - V3 <http://www.gnu.org/licenses/>
 */

#include <dm/dm.h>
#include "../include/files.h"

char *www_index() {
  return "<body onload=\"location = 'main/index.html';\">\n";
}

char *www_styles() {
  return ""
    "BODY {\n"
    "  font-family: sans;font-size: 14px;\n"
    "  margin-right: 50px; margin-left: 50px;\n"
    "  margin-top: 5px; background-color: #f0f1f2\n"
    "}\n"
    "A{\n"
    "  text-decoration: none; font-family: sans;\n"
    "  color: #000080; font-weight: normal;font-size:14px;\n"
    "}\n"
    "TT{color:#400030; font-size: 14px}\n"
    "pre {\n"
    "  background-color: rgb(245, 245, 248);\n"
    "  border: 1px solid rgb(110,130,150);\n"
    "  padding: 4px; border-radius: 10px;\n"
    "  font-size:12px;\n"
    "}\n"
    "TABLE.main {\n"
    "  border-collapse : collapse;\n"
    "  border : 0px;\n"
    "  width : 100%\n"
    "}\n"
    "TABLE.border {border: 1px solid rgb(110,130,150);}\n"
    "TD {font-family: sans;font-size: 14px;\n"
    "  margin-right: 5px; margin-left: 5px;}\n"
    "TD.border {border: 1px solid rgb(110,130,150);}\n"
    "TD.prel {\n"
    "  font-family: monospace;font-size: 12px;\n"
    "  background-color: rgb(215, 215, 215);color: #998866;\n"
    "  border-right: solid 1px; border-left: solid 1px;\n"
    "  border-top-left-radius: 10px;\n"
    "  border-top:1px solid rgb(110,130,150);\n"
    "  border-bottom-left-radius: 10px;\n"
    "  border-bottom:1px solid rgb(110,130,150);\n"
    "  padding: 4px;\n"
    "  text-align: right;\n"
    "}\n"
    "TD.prer {\n"
    "  font-family: monospace;font-size: 12px;\n"
    "  background-color: rgb(245, 245, 248);\n"
    "  border-right: solid 1px;\n"
    "  border-top-right-radius: 10px;\n"
    "  border-top:1px solid rgb(110,130,150);\n"
    "  border-bottom-right-radius: 10px;\n"
    "  border-bottom:1px solid rgb(110,130,150);\n"
    "  padding: 4px;\n"
    "}\n"
    "\n"
    ".link{\n"
    "  text-decoration: none; font-family: sans;\n"
    "  color: #000080; font-weight: normal;font-size:14px;\n"
    "}\n"
    ".frame {\n"
    "  background-color: rgb(250, 250, 250);\n"
    "  border: 1px solid rgb(110,130,150);\n"
    "  font-family: sans;font-size: 14px;\n"
    "  padding: 4px;border-radius: 4px;\n"
    "}\n"
    ".frame2 {\n"
    "  background-color: rgb(250, 250, 230);\n"
    "  border: 1px solid rgb(110,130,150);\n"
    "  font-family: sans;font-size: 14px;\n"
    "  padding: 4px;border-radius: 4px;\n"
    "}\n"
    ".frame3 {\n"
    "  background-color: rgb(240, 245, 250);\n"
    "  border: 1px solid rgb(110,130,150);\n"
    "  font-family: sans;font-size: 14px;\n"
    "  padding: 4px;border-radius: 4px;\n"
    "}\n"
  ;
}

char *www_dir_index(char *level, char *title) {
  char *r = ""
    "<!doctype html>\n"
    "<html>\n"
    "  <head>\n"
    "  <title>__TITLE__</title>\n"
    "  <meta http-equiv='Content-Type' content='text/html; charset=UTF-8' />\n"
    "  <meta http-equiv='Cache-Control'\n"
    "    content='no-cache, no-store, must-revalidate' />\n"
    "  <meta http-equiv='Pragma' content='no-cache' />\n"
    "  <meta http-equiv='Expires' content='0' />\n"
    "  <meta name='lang' content='es' />\n"
    "  <meta name='author' content='ºDeme.' />\n"
    "  <link rel='icon' type='image/png' href='__LEVEL__img/favicon.png' />\n"
    "  <link rel='stylesheet' href='__LEVEL__styles.css' type='text/css' />\n"
    "  </head>\n"
    "  <body>\n"
    "  </body>\n"
    "  <script type='text/javascript' src='index.js'></script>\n"
    "</html>\n"
  ;
  return str_replace(
    str_replace(r, "__TITLE__", title),
    "__LEVEL__", level);
}

char *www_img_index() {
  return "<body onload=\"location = '../index.html';\">\n";
}

char *files_build(char *path_level, char *dir, char *file) {
  char *r = ""
    "-cp __PATH_LEVEL__src/lib\n"
    "-cp __PATH_LEVEL__src/__DIR__\n"
    "-cp /deme/dmHaxe17/lib/basic_0.1.0/src\n"
    "-cp /deme/dmHaxe17/lib/server_2.0.0/src\n"
    "-cp /deme/dmHaxe17/lib/widgets_0.0.1/src\n"
    "\n"
    "-js __PATH_LEVEL__www/__DIR__/index.js\n"
    "-dce full\n"
    "\n"
    "-main __FILE__\n"
  ;
  return str_replace(
    str_replace(
      str_replace(r, "__PATH_LEVEL__", path_level),
      "__DIR__", dir),
    "__FILE__", file);
}

char *files_main(char *name) {
  char *r = ""
    "/*\n"
    " * Copyright __DATE__ ºDeme\n"
    " * GNU General Public License - V3 <http://www.gnu.org/licenses/>\n"
    " */\n"
    "\n"
    "/// Entry class\n"
    "class __CLASS__ {\n"
    "  /// Entry point\n"
    "  public static function main() {\n"
    "    js.Browser.alert(\"here\");\n"
    "  }\n"
    "}\n"
  ;
  return str_replace(
    str_replace(r, "__CLASS__", name),
    "__DATE__", date_format(date_now(), "%d-%b-%Y"));
}

char *files_buildS(char *path_level, char *dir, char *file) {
  char *r = ""
    "-cp __PATH_LEVEL__src/lib\n"
    "-cp __PATH_LEVEL__src/__DIR__\n"
    "-cp /deme/dmHaxe17/lib/basic_0.1.0/src\n"
    "-cp /deme/dmHaxe17/lib/server_2.0.0/src\n"
    "\n"
    "-js __PATH_LEVEL__scripts/__DIR__/index.js\n"
    "-D shallow-expose\n"
    "-dce full\n"
    "\n"
    "-main __FILE__\n"
  ;
  return str_replace(
    str_replace(
      str_replace(r, "__PATH_LEVEL__", path_level),
      "__DIR__", dir),
    "__FILE__", file);
}

char *files_mainS(char *name) {
  char *r = ""
    "/*\n"
    " * Copyright __DATE__ ºDeme\n"
    " * GNU General Public License - V3 <http://www.gnu.org/licenses/>\n"
    " */\n"
    "\n"
    "/// Entry class\n"
    "class __CLASS__ {\n"
    "  /// Function enumeration to avoid their removal\n"
    "  public static function main() {\n"
    "    var fs = [\n"
    "      init\n"
    "    ];\n"
    "  }\n"
    "  @:expose(\"init\")\n"
    "  /// Initialization function\n"
    "  public static function init (data : String):String {\n"
    "    return \"\";\n"
    "  }\n"
    "}\n"
  ;
  return str_replace(
    str_replace(r, "__CLASS__", name),
    "__DATE__", date_format(date_now(), "%d-%b-%Y"));
}

