// Copyright 31-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Constants.

#ifndef DATA_CTS_H
  #define DATA_CTS_H

	//Application ----------------------------------------------------------------

	/// Application name
	#define APP_NAME "cmarket"

	/// Data version
	#define VERSION "202306"

	/// Data directory.
	#define DATA_PATH "/dm/dmWeb/app/KtWeb/dmcgi/CMarket"

	/// Number of quotes used to calculate companies volume.
  #define QUOTES_VOLUME 60

	// Server --------------------------------------------------------------------

	/// External program to read web pages.
	#define WGET "Wget"

	/// External program to read web pages.
	#define PUPPETEER "Puppeteer"

	/// Minimum entries for a valid reading of historic quotes.
	#define HISTORIC_MINIMUM_ENTRIES 4

	/// Server configuration stopped
	#define SERVER_STOPED 0

	/// Server configuration active
	#define SERVER_ACTIVE 1

	/// Server configuration selected
	#define SERVER_SELECTED 2

	// Net -----------------------------------------------------------------------

	/// Wget user agent configuration.
	/// Necessary for YAHOO.
	/// If it fails, every value can be replaced with ""
	#define WGET_UA1 "--user-agent"
	#define WGET_UA2 "Mozilla"
	#define WGET_UA3 "--load-cookies=/home/deme/.mozilla/firefox/bfrqeymk.default/cookies.sqlite"

	// Scheduler -----------------------------------------------------------------

	/// Time to scheduler sleep (milliseconds)
	#define SCHEDULER_SLEEP_TIME 900000 // 15'

	/// Time to scheduler sleep (milliseconds)
  #define SCHEDULER_WATCHING_TIME 120000 // 2'

	/// External servers (Infobolsa, Finanzas, etc.) delay in SECONDS
	#define SERVER_DELAY 900 // 15 minutes

	/// Activity state
	#define ACT_SLEEPING "Sleeping"

	/// Activity state
	#define ACT_ACTIVE "Active"

	/// Hour to start actHistoric fase
	#define ACT_HISTORIC_START 2

	// Models - Investors --------------------------------------------------------

	/// Number of investors (from 0 to 2)
	#define INVESTORS 3

	/// Number of quotes in historic tables
	#define HISTORIC_QUOTES 610

	/// Number of quotes for accounting charts.
  #define ACCOUNTING_QUOTES 250

	/// Fleas initial capital for each cycle
	#define INITIAL_CAPITAL 100000.0

	/// Bet
	#define BET 10000.0

	/// Minimun cash to bet
	#define MIN_TO_BET 11000.0

	/// No lost multiplicator
	#define NO_LOST_MULTIPLICATOR 1.05

	/// Rebuy limit
	#define REBUY_LIMIT 300.0

#endif
