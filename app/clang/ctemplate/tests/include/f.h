data

/*--*/

/// Commented line 1
/// Commented line 2
typedef struct f_Sell Sell;

///
size_t sell_nick(Sell *this);

///
char *sell_tmp(Sell *this);

///
void *(*sell_fn(Sell *this))(Sell *this, int n);

///
void sell_set_fn(Sell *this, void *(*value)(Sell *this, int n));

///
Js *sell_to_js(Sell *this);

///
typedef struct f_Buy Buy;

///
Buy *buy_new(size_t nick, double quote, Opt-Person *friend);

/// Market quote
double buy_quote(Buy *this);

///
void buy_set_quote(Buy *this, double value);

///
Opt-Person *buy_friend(Buy *this);

///
Js *buy_to_js(Buy *this);

///
Buy *buy_fom_js(Js *js);

/*--*/

data


