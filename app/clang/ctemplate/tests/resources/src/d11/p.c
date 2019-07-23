Some text

/* .
= D11P
  tmp: time_t
  # Some people
  group: List - Iarr
  cash: float
  historic: Map - Darr
  wait: long
*/

some text

/*--*/

typedef struct p_D11P D11P;

struct p_D11P {
  time_t tmp;
  List *group;
  float cash;
  Map *historic;
  long wait;
};

static D11P *d11P_new (
  Gc *gc,
  time_t tmp,
  List *group,
  float cash,
  Map *historic,
  long wait
) {
  D11P *this = gc_add(gc, malloc(sizeof(D11P)));
  this->tmp = tmp;
  this->group = group;
  this->cash = cash;
  this->historic = historic;
  this->wait = wait;
  return this;
}

/*--*/

some text



