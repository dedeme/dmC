// Copyright 22-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "comp/cmd.h"
#include "kut/sys.h"

Rs *cmd_run (Arr *command) {
  return sys_cmd(command);
}
