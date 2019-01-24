// Copyright 11-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Program entry.<p>
/// Market_data read data of Madrid Market from several web servers.<p>
/// It works like a daemon, starting with 'market_data start' and stopping
/// with 'market_data stop'.<p>
/// If 'market_data start' is called when an instance of program is working,
/// this instance is stopped and a new instance is run.<p>
/// Data is stored in .dmCApp and is read by DailyMarket (a Client-Server
/// program)

#ifndef MAIN_H
  #define MAIN_H

#include "dmc/std.h"

///
int main(int argc, char* args[]);

#endif
