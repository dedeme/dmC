typedef struct tarr_Arri Arri;
typedef struct tarr_Arrf Arrf;
typedef struct tarr_Arrs Arrs;
typedef struct tarr_Arr Arr;

static int64_t *tarr_beginI (Arri *this);
static double *tarr_beginF (Arrf *this);
static char **tarr_beginS (Arrs *this);

static void tarr_catI (Arri *this, Arri *other);
static void tarr_catF (Arrf *this, Arrf *other);
static void tarr_catS (Arrs *this, Arrs *other);

static void tarr_clearI (Arri *this);
static void tarr_clearF (Arrf *this);
static void tarr_clearS (Arrs *this);

static Arri *tarr_concatI (Arri *this, Arri *other);
static Arrf *tarr_concatF (Arrf *this, Arrf *other);
static Arrs *tarr_concatS (Arrs *this, Arrs *other);

static int tarr_containsI (Arri *this, int64_t e);
static int tarr_containsF (Arrf *this, double e);
static int tarr_containsS (Arrs *this, char *e);

static Arri *tarr_copyI (Arri *this);
static Arrf *tarr_copyF (Arrf *this);
static Arrs *tarr_copyS (Arrs *this);

static Arri *tarr_dropI (Arri *this, int64_t n);
static Arrf *tarr_dropF (Arrf *this, int64_t n);
static Arrs *tarr_dropS (Arrs *this, int64_t n);

// Removes duplicates from 'this' and returns them.
static Arri *tarr_duplicatesI (Arri *this);
static Arrf *tarr_duplicatesF (Arrf *this);
static Arrs *tarr_duplicatesS (Arrs *this);

static int64_t *tarr_endI (Arri *this);
static double *tarr_endF (Arrf *this);
static char **tarr_endS (Arrs *this);

static int tarr_empty (Arr *this);
static int tarr_emptyI (Arri *this);
static int tarr_emptyF (Arrf *this);
static int tarr_emptyS (Arrs *this);

static int64_t tarr_getI (Arri *this, int64_t ix);
static double tarr_getF (Arrf *this, int64_t ix);
static char *tarr_getS (Arrs *this, int64_t ix);

static int64_t tarr_indexI (Arri *this, int64_t e);
static int64_t tarr_indexF (Arrf *this, double e);
static int64_t tarr_indexS (Arrs *this, char *e);

static void tarr_insertI (Arri *this, int64_t ix, int64_t e);
static void tarr_insertF (Arrf *this, int64_t ix, double e);
static void tarr_insertS (Arrs *this, int64_t ix, char *e);

static void tarr_insert_arrayI (Arri *this, int64_t ix, Arri *other);
static void tarr_insert_arrayF (Arrf *this, int64_t ix, Arrf *other);
static void tarr_insert_arrayS (Arrs *this, int64_t ix, Arrs *other);

static void tarr_insert_rangeI (
  Arri *this, int64_t ix, Arri *other, int64_t begin, int64_t end
);
static void tarr_insert_rangeF (
  Arrf *this, int64_t ix, Arrf *other, int64_t begin, int64_t end
);
static void tarr_insert_rangeS (
  Arrs *this, int64_t ix, Arrs *other, int64_t begin, int64_t end
);

// Index correction for slices.
static int64_t tarr_ix_correctI (Arri *this, int64_t n);
static int64_t tarr_ix_correctF (Arrf *this, int64_t n);
static int64_t tarr_ix_correctS (Arrs *this, int64_t n);

static char *tarr_join (Arrs *this, char *sep);

static Arr *tarr_new_ (void);
static Arri *tarr_new_i (void);
static Arrf *tarr_new_f (void);
static Arrs *tarr_new_s (void);

static Arri *tarr_new_fromi (int64_t *es, int64_t n);
static Arrf *tarr_new_fromf (double *es, int64_t n);
static Arrs *tarr_new_froms (char **es, int64_t n);

static Arri *tarr_newi (int64_t e, int64_t n);
static Arrf *tarr_newf (double e, int64_t n);
static Arrs *tarr_news (char *e, int64_t n);

static void *tarr_peek (Arr *this);
static int64_t tarr_peekI (Arri *this);
static double tarr_peekF (Arrf *this);
static char *tarr_peekS (Arrs *this);

static void *tarr_pop (Arr *this);
static int64_t tarr_popI (Arri *this);
static double tarr_popF (Arrf *this);
static char *tarr_popS (Arrs *this);

static void tarr_push (Arr *this, void *e); // Shallow
static void tarr_pushI (Arri *this, int64_t e);
static void tarr_pushF (Arrf *this, double e);
static void tarr_pushS (Arrs *this, char *e);

static int64_t tarr_removeI (Arri *this, int64_t ix);
static double tarr_removeF (Arrf *this, int64_t ix);
static char *tarr_removeS (Arrs *this, int64_t ix);

static Arri *tarr_remove_rangeI (Arri *this, int64_t begin, int64_t end);
static Arrf *tarr_remove_rangeF (Arrf *this, int64_t begin, int64_t end);
static Arrs *tarr_remove_rangeS (Arrs *this, int64_t begin, int64_t end);

static Arri *tarr_reverseI (Arri *this);
static Arrf *tarr_reverseF (Arrf *this);
static Arrs *tarr_reverseS (Arrs *this);

static void tarr_reverse_inI (Arri *this);
static void tarr_reverse_inF (Arrf *this);
static void tarr_reverse_inS (Arrs *this);

static void tarr_setI (Arri *this, int64_t ix, int64_t e);
static void tarr_setF (Arrf *this, int64_t ix, double e);
static void tarr_setS (Arrs *this, int64_t ix, char *e);

static void tarr_set_arrayI (Arri *this, int64_t ix, Arri *other);
static void tarr_set_arrayF (Arrf *this, int64_t ix, Arrf *other);
static void tarr_set_arrayS (Arrs *this, int64_t ix, Arrs *other);

static void tarr_set_rangeI (
  Arri *this, int64_t ix, Arri *other, int64_t begin, int64_t end
);
static void tarr_set_rangeF (
  Arrf *this, int64_t ix, Arrf *other, int64_t begin, int64_t end
);
static void tarr_set_rangeS (
  Arrs *this, int64_t ix, Arrs *other, int64_t begin, int64_t end
);

static int64_t tarr_shiftI (Arri *this);
static double tarr_shiftF (Arrf *this);
static char *tarr_shiftS (Arrs *this);

static void tarr_shuffleI (Arri *this);
static void tarr_shuffleF (Arrf *this);
static void tarr_shuffleS (Arrs *this);

static int tarr_sizeI (Arri *this);
static int tarr_sizeF (Arrf *this);
static int tarr_sizeS (Arrs *this);

static void tarr_sortI (Arri *this, int is_ascendant);
static void tarr_sortF (Arrf *this, int is_ascendant);
static void tarr_sortS (Arrs *this, int is_ascendant);

static Arri *tarr_takeI (Arri *this, int64_t n);
static Arrf *tarr_takeF (Arrf *this, int64_t n);
static Arrs *tarr_takeS (Arrs *this, int64_t n);

static void tarr_unshiftI (Arri *this, int64_t e);
static void tarr_unshiftF (Arrf *this, double e);
static void tarr_unshiftS (Arrs *this, char *e);
