// Copyright 02-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/head.h"
#include "lexer/token.h"
#include "lexer/block.h"

Txpos *head_local(Txpos *tx) {
  Txpos *r;
  if (txpos_eq(tx, r = token_directive(tx, "_local"))) {
    return tx;
  }
  return r;
}

Txpos *head_generics(Arr/*char*/ **generics, Txpos *tx) {
  Txpos *r;
  if (txpos_eq(tx, r = token_cconst(tx, '<'))) {
    return tx;
  }
  tx = r;

  return token_list(generics, tx, '>', (Txpos *(*)(void **, Txpos *))token_id);
}

static Txpos *read_as_id(Tp_str_str **tp, Txpos *tx) {
  Txpos *r;

  char *id;
  if (txpos_eq(tx, r = token_id(&id, tx)))
    TH(tx) "Expected an identifer" _TH
  tx = r;

  char *id2;
  if (txpos_eq(tx, r = token_id(&id2, tx))) {
    *tp = tp_str_str_new(id, "");
  } else  {
    if (strcmp(id2, "as"))
      TH(tx) "Expected 'as'" _TH
    tx = r;

    if (txpos_eq(tx, r = token_id(&id2, tx)))
      TH(tx) "Expected an identifer" _TH
    tx = r;

    *tp = tp_str_str_new(id, id2);
  }

  return tx;
}

static Txpos *read_import(Import **import, Txpos *tx) {
  Txpos *r;
  Txpos *start = tx;

  char *path;
  if (txpos_eq(tx, r = token_path(&path, tx)))
    TH(tx) "Expectec a path" _TH
  tx = r;

  if (txpos_neq(tx, r = token_cconst(tx, '*'))) {
    tx = r;

    if (txpos_eq(tx, r = token_cconst(tx, '('))) {
      *import = import_new_id(txpos_pos(start), path, "*");
    } else {
      tx = r;

      Arr/*char*/ *ids;
      tx = token_list(
        &ids, tx, ')', (Txpos *(*)(void **, Txpos *))token_id
      );

      *import = import_new_subs(txpos_pos(start), path, ids);
    }
  } else if (txpos_neq(tx, r = token_cconst(tx, '('))) {
    tx = r;

    Arr/*Tp_str_str*/ *renames;
    tx = token_list(
      &renames, tx, ')', (Txpos *(*)(void **, Txpos *))read_as_id
    );

    *import = import_new_adds(txpos_pos(start), path, renames);
  } else {
    char *id;
    if (txpos_neq(tx, r = token_id(&id, tx))) {
      if (strcmp(id, "as"))
       TH(tx) "Expected 'as'" _TH
      tx = r;

      if (txpos_eq(tx, r = token_id(&id, tx)))
        TH(tx) "Expected an identifier" _TH
      tx = r;

      *import = import_new_id(txpos_pos(start), path, id);
    } else {
      *import = import_new_single(txpos_pos(start), path);
    }
  }
  return tx;
}

Txpos *head_imports(Arr/*Import*/ **imports, Txpos *tx) {
  Txpos *r;
  if (txpos_eq(tx, r = token_directive(tx, "_import"))) {
    return tx;
  }
  tx = r;

  if (txpos_eq(tx, r = token_cconst(tx, '{')))
    TH(tx) "Expected '{'" _TH
  tx = r;

  return token_list(
    imports, tx, '}', (Txpos *(*)(void **, Txpos *))read_import
  );
}

static Txpos *read_as_type(Tp_str_type **tp, Txpos *tx) {
  Txpos *r;

  char *id;
  if (txpos_eq(tx, r = token_id(&id, tx)))
    TH(tx) "Expected an identifer" _TH
  tx = r;

  char *id2;
  if (txpos_eq(tx, r = token_id(&id2, tx))) {
    TH(tx) "Expected 'as'" _TH
  }
  if (strcmp(id2, "as"))
    TH(tx) "Expected 'as'" _TH
  tx = r;

  Type *type;
  if (txpos_eq(tx, r = block_type(&type, tx)))
    TH(tx) "Expected a type" _TH
  tx = r;

  *tp = tp_str_type_new(id, type);

  return tx;
}

static Txpos *read_implement(Implement **implement, Txpos *tx) {
  Txpos *r;
  Txpos *start = tx;

  char *path;
  if (txpos_eq(tx, r = token_path(&path, tx)))
    TH(tx) "Path is missing" _TH
  tx = r;

  if (txpos_eq(tx, r = token_cconst(tx, '('))) {
    *implement = implement_new(txpos_pos(start), path, arr_new());
  } else {
    tx = r;

    Arr/*Tp_str_type*/ *generics;
    tx = token_list(
      &generics, tx, ')', (Txpos *(*)(void **, Txpos *))read_as_type
    );
    *implement = implement_new(txpos_pos(start), path, generics);
  }

  return tx;
}

Txpos *head_extends(Implement **export, Txpos *tx) {
  Txpos *r;
  if (txpos_eq(tx, r = token_directive(tx, "_extends"))) {
    return tx;
  }
  tx = r;

  if (txpos_eq(tx, r = read_implement(export, tx)))
    TH(tx) "'_extends' path is missing" _TH

  return r;
}

Txpos *head_implements(Arr/*Implement*/ **implements, Txpos *tx) {
  Txpos *r;
  if (txpos_eq(tx, r = token_directive(tx, "_implements"))) {
    return tx;
  }
  tx = r;

  if (txpos_eq(tx, r = token_cconst(tx, '{')))
    TH(tx) "'_implements' paths are missing" _TH
  tx = r;

  Arr/*Implements*/ *imps;
  tx = token_list(
    &imps, tx, '}', (Txpos *(*)(void **, Txpos *))read_implement
  );
  *implements = imps;

  return tx;
}
