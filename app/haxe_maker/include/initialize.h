/*
 * Copyright 06-Mar-2017 ºDeme
 * GNU General Public License - V3 <http://www.gnu.org/licenses/>
 */

/**
Initializes an application.<p>
It does next actions:<ol>
<li>Create 'www', 'www/img' and 'www/main' directories and 'www/index.html',
'www/img/index.html' and 'www/main/index.html' files.
<li>Create 'i18n', 'src', 'src/lib' and 'src/main' directories and
'src/main/Main.hx' and 'src/main/build.hxml' files.
<li>If application is a server one, create 'scripts' and
'scripts/main'  directories and 'src/main/MainS.hx' and 'src/main/buildS.hxml'
files.
<li>make links in 'www' and 'cgi'</ol>
  is_server: It application is a server one.
  app      : Application name. It is used to create directories.
  dir      : Application main directory
  www      : Web parent directory
  cgi      : Cgi main directory
*/
void initialize(int is_server, char *app, char *dir, char *www, char *cgi);
