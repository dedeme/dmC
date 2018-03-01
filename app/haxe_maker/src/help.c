/*
 * Copyright 06-Mar-2017 ºDeme
 * GNU General Public License - V3 <http://www.gnu.org/licenses/>
 */

#include <dm/dm.h>
#include "../include/help.h"

void help () {
  puts("\nUse:\n"
    "haxe_maker options\n"
    "Options can be:\n"
    "  -init -> Initialize the application.\n"
    "  -i18n -> Make english and spanish dictionaries.\n"
    "  -dir=relativePath -> Create a directory (e.g. dir=auth).\n"
    "  -comp -> Compile the application.\n"
    "  -s -> Server application (Default is client application.\n"
    "  -app=name -> application name (e.g. -app=HDoc).\n"
    "  -www=directory -> www path (e.g. -www=/deme/www)\n"
    "  -cgi=directory -> www path (e.g. -cgi=/deme/wwwcgi)\n"
    "Option -init must go with -app and -www.\n"
    "Option -init -s must go with -app, -www and -cgi.\n"
    "Option -i18n must go alone.\n"
    "Option -comp must no go with anyother option.\n"
    "Option -dir only can optionally go with -s.\n"
    "Options '-init', '-i18n', '-comp' and '-dir' are incompatibles.\n"
  );
}

