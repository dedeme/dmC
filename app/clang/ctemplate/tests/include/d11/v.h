data

/*--*/

///
typedef struct v_D11V D11V;

///
int d11V_age(D11V *this);

///
char *d11V_tmp(D11V *this);

/// Some people
Arr *d11V_group(D11V *this);

///
double d11V_cash(D11V *this);

///
Darr *d11V_historic(D11V *this);

/// A good friend
Person *d11V_friend(D11V *this);

///
int d11V_big(D11V *this);

///
Js *d11V_to_js(D11V *this);

///
D11V *d11V_from_js(Js *js);

/*--*/

data
