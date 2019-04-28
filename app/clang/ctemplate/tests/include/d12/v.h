data

/*--*/

///
typedef struct v_D12V D12V;

///
int d12V_age(D12V *this);

///
List *d12V_group(D12V *this);

///
float d12V_cash(D12V *this);

///
Map *d12V_historic(D12V *this);

///
char *d12V_tmp(D12V *this);

/// Some people.
///   Arr[Person]
Arr *d12V_group2(D12V *this);

///
double d12V_cash2(D12V *this);

///
Darr *d12V_historic(D12V *this);

/// A good friend
Person *d12V_friend(D12V *this);

///
void d12V_set_friend(D12V *this, Person *value);

///
int d12V_big(D12V *this);

///
void d12V_set_big(D12V *this, int value);

/// Map[Darr]
Map *d12V_historic2(D12V *this);

///
void d12V_set_historic2(D12V *this, Map *value);

///
int (*d12V_k2(D12V *this))(D12F2 *this);

///
Rs *(*d12V_test(D12V *this))(D12F2 *this, int money);

///
void d12V_set_test(D12V *this, Rs *(*value)(D12F2 *this, int money));

///
Js *d12V_to_js(D12V *this);

///
D12V *d12V_from_js(Js *js);

/*--*/

data
