// Copyright 12-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Kutt type data.

#ifndef DATA_TYPE_H
  #define DATA_TYPE_H

#include "kut/arr.h"
#include "kut/map.h"
#include "kut/rs.h"

typedef struct wrCtx_WrCtx WrCtx;

/// Record structure.
typedef struct {
  char *id; // one of: b, i, f, s, <id>, (id), a, r, d, t, 0, 1 or
            // upercases [A-Z].
  Arr *subtypes; // Arr<Type>
} Type;

/// Create a type.
///   id: Type id. Can be: b, i, f, s, <id>, (id), a, r, d, t, 0, 1 or
///       upercases [A-Z].
///       'id' stands by identifier.
///       '0' is procedure type and '1' function type.
///       [A-Z] stand by generics.
///       (id) is a definition where id can be:
///         module.symbol: Qualified symbol of a module different to the
///                        current one.
///         symbol       : Symbol of the current module. If the current module
///                        has not such symbol, the pair module.symbol will be
///                        be deduced.
///   subtypes: Elements of container or generics of definition.
Type *type_new (char *id, Arr *subtypes);

/// Returns the unique object of the type.
Type *type_bool (void);

/// Returns the unique object of the type.
Type *type_int (void);

/// Returns the unique object of the type.
Type *type_float (void);

/// Returns the unique object of the type.
Type *type_string (void);

/// Returns the identifier of an object.
char *type_oid (Type *this);

/// Returns the module of a definition.
///   If definition has not module, the function returns "".
///   Otherwise the function returns the local module identifier.
char *type_dmd (Type *this);

/// Returns the identifier of a definition.
char *type_did (Type *this);

/// Return the type of an array.
Type *type_atype (Type *this);

/// Return the type of an iterator.
Type *type_rtype (Type *this);

/// Return the type of a dictionary.
Type *type_dtype (Type *this);

/// Returns the type of a position in tuple.
/// NOTE: Limits are no checked.
///   this: Type (must be a tuple).
///   i   : Position of subtype in tuple.
Type *type_ttype (Type *this, int i);

/// Returns the number of parameters of a function or -1 if 'this' is not
/// a function.
int type_fnpars (Type *this);

/// Returns the type of a function parameter.
/// NOTE: Limits are no checked.
///   this: Type (must be a function).
///   i   : Position of parameter.
Type *type_fptype (Type *this, int i);

/// Returns the type of a function return:
///   this: Type (must be a function).
///   [type] If function returns something.
///   [] If function returns nothing.
/// Return Opt<type>
Opt *type_frtype (Type *this);

/// Returns C type of a funciton
///   this: Type (must be a function).
char *type_fctype (Type *this);

/// Returns 'true' if type 'this' is generic.
int type_is_generic (Type *this);

/// Checks an user type and returns a canonical one.
/// Returns Rs<Type>
Rs *type_read (char *tx);

/// Returns the canonical representation of 'this'.
char *type_to_str (Type *this);

/// Returns C Val of type.
/// NOTE: array and dictionary must have indicated the element type.
///   this  : Type to convert.
///   ctx   : Writer context.
char *type_to_c (Type *this, WrCtx *ctx);

///
char *type_to_js (Type *this);

///
Type *type_from_js (char *js);

/// Returns string with generics of 'this'.
char *type_get_generics (Type *this);

/// Replace generics of 'this' with the map Gs (Map<type>).
Type *type_replace_generics (Type *this, Map *gs);

/// Returns the real right part of a definition form its real left part,
/// replacing generics.
/// NOTE: Is expected that 'real_left' and 'left_def' are definition types and
///       have the same number of subtypes.
///   real_left: Real left part.
///   left_def: Left part of type definition.
///   right_ref: Right part of type definition.
Type *type_replace_def_generics (
  Type *real_left, Type *left_def, Type *right_def
);

/// Returns TRUE it 'tp1' and 'tp2' are the same type
/// rs_error can result if an error is produced by types expansion.
///   ctx: Writer context.
///   tp1: Type.
///   tp2: Type.
Rs *type_eq (WrCtx *ctx, Type *tp1, Type *tp2);

/// Returns TRUE if the paremeter type 'expected' can allow the value
/// type 'actual' as reification.
/// Reification is the result of replace generics in 'pt' by other types.
/// 'allow' is called when a function is called or a value is inserted in a
/// container.
/// rs_error can result if an error is produced by types expansion.
/// The expansion is not applied to subtypes.
///   ctx     : Writer context.
///   generics: Map<Type> {generic::type} with mandatory generic replacements.
///               This dictionary grows up with each new 'pt' of generic type.
///   expected: Type of function parameter or container element.
///   actual  : Type of value to pass in functions or to insert.
Rs *type_allow (WrCtx *ctx, Map *generics, Type *expected, Type *actual);

/// Returns TREU if the paremeter type 'actual' can be returned as
/// 'expected'.
/// rs_error can result if an error is produced by types expansion.
///   ctx     : Writer context.
///   expected: Type expected to be returned.
///   actual  : Type of actual value.
Rs *type_allow_return (WrCtx *ctx, Type *expected, Type *actual);

/// Returns an empty string if the type definition match the generics string.
/// Otherwise returns an error.
///   gs  : Generics string for a new type (without blanks).
///   this: Type to check.
char *type_check_def (char *gs, Type *this);

#endif
