data

/*--*/

/// Commented line 1
/// Commented line 2
///   Arguments:
///     nick: size_t
///   Variables:
///     tmp: char*
///   Functions:
///     tmp: char*
typedef struct f_Sell Sell;

///
size_t sell_nick (Sell *this);

///
char *sell_tmp (Sell *this);

///
void *(*sell_fn (Sell *this))(Sell *this, int n);

///
void sell_set_fn (Sell *this, void *(*value)(Sell *this, int n));

///
Js *sell_to_js (Sell *this);

///
///   Arguments:
///     nick: size_t
///     quote: double4
///     friend: Opt-Person
typedef struct f_Buy Buy;

///
Buy *buy_new (size_t nick, double4 *quote, Opt *friend);

/// Market quote
double4 *buy_quote (Buy *this);

///
void buy_set_quote (Buy *this, double4 *value);

///
Opt *buy_friend (Buy *this);

///
Js *buy_to_js (Buy *this);

///
Buy *buy_from_js (Js *js);

/*--*/

data


