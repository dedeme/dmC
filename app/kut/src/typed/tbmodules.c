// Copyright 27-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "typed/tbmodules.h"

static TbmodRs *fail (char *msg) {
  TbmodRs *this = MALLOC(TbmodRs);
  this->code = msg;
  this->tret = ttype_ERROR;
  return this;
}

static TbmodRs *ok (char *code, Ttype tret) {
  TbmodRs *this = MALLOC(TbmodRs);
  this->code = code;
  this->tret = tret;
  return this;
}

// <char>
TbmodRs *tbmodules_get (
  char *module, char *function, int as_expression, char *type, char *pars
) {
  TbmodRs *check(
    int is_function, char *tpars, char *tret, char *cfunction
  ) {
    if (is_function) {
      if (as_expression) {
        if (strcmp(tpars, type))
          return fail(str_f(
            "Function %s.%s expects parameters type '%s', but '%s' was found",
            module, function, tpars, type
          ));
        return ok(cfunction, ttype_from_sym(*tret));
      }
      if (strcmp(tpars, type))
        return fail(str_f(
          "Function %s.%s expects arguments type '%s', but '%s' was found",
          module, function, tpars, type
        ));
      return ok(cfunction, ttype_NO_RETURN);
    }
    if (as_expression)
      return fail(str_f("%s.%s does not return any value", module, function));
    if (strcmp(tpars, type)) {
      return fail(str_f(
        "Function %s.%s expects parameters type '%s', but '%s' was found",
        module, function, tpars, type
      ));
    }
    return ok(cfunction, ttype_NO_RETURN);
  }

  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  if (!strcmp(module, "arr")) {  // ---------------------------------------- arr
    if (!strcmp(function, "cat"))
      return (as_expression || !*type)
        ? check(FALSE, str_f("%c%c", *type, *type), "", "--??")
        : check(FALSE, str_f("%c%c", *type, *type), "",
          str_f("tarr_cat%c(%s)", *type, pars)
        );
    if (!strcmp(function, "clear"))
      return (as_expression || !*type)
        ? check(FALSE, str_f("%c*", *type), "", "--??")
        : check(FALSE, str_f("%c", *type), "",
            str_f("tarr_clear%c(%s)", *type, pars))
        ;
    if (!strcmp(function, "contains"))
      return check(TRUE, str_f("%c%c", *type,
          *type == 'I' ? 'i' : *type == 'F' ? 'f': 's'
        ), "b", str_f("tarr_contains%c(%s)", *type, pars)
      );
    if (!strcmp(function, "copy"))
      return check(TRUE, str_new_c(*type), str_new_c(*type),
        str_f("tarr_copy%c(%s)", *type, pars)
      );
    if (!strcmp(function, "drop"))
      return check(TRUE, str_f("%ci", *type), str_new_c(*type),
        str_f("tarr_drop%c(%s)", *type, pars)
      );
    if (!strcmp(function, "duplicates"))
      return check(TRUE, str_new_c(*type), str_new_c(*type),
        str_f("tarr_duplicates%c(%s)", *type, pars)
      );
    if (!strcmp(function, "index"))
      return check(TRUE, str_f("%c%c", *type,
          *type == 'I' ? 'i' : *type == 'F' ? 'f': 's'
        ), "i", str_f("tarr_index%c(%s)", *type, pars)
      );
    if (!strcmp(function, "insert"))
      return (as_expression || !*type)
        ? check(FALSE, str_f("%ci*", *type), "", "--??")
        : check(FALSE, str_f("%ci%c", *type,
          *type == 'I' ? 'i' : *type == 'F' ? 'f': 's'
          ), "", str_f("tarr_insert%c(%s)", *type, pars)
        );
    if (!strcmp(function, "insertArr"))
      return (as_expression || !*type)
        ? check(FALSE, str_f("%ci*", *type), "", "--??")
        : check(FALSE, str_f("%ci%c", *type, *type),
          "", str_f("tarr_insert_array%c(%s)", *type, pars)
        );
    if (!strcmp(function, "insertRange"))
      return (as_expression || !*type)
        ? check(FALSE, str_f("%ci*ii", *type), "", "--??")
        : check(FALSE, str_f("%ci%cii", *type, *type),
          "", str_f("tarr_insert_range%c(%s)", *type, pars)
        );
    if (!strcmp(function, "join"))
      return check(TRUE, "Ss", "s", str_f("tarr_join(%s)", pars));
    if (!strcmp(function, "new")) {
      if (str_cindex("ifs", *type) == -1)
        return fail("Type arrays only can be of 'i', 'f' or 's'");
      return check(TRUE, str_f("%ci", *type),
        *type == 'i'? "I" : *type == 'f' ? "F" : "S",
        str_f("tarr_new%c(%s)", *type, pars)
      );
    }
    if (!strcmp(function, "peek"))
      return check(TRUE, str_new_c(*type),
        str_f("%c", *type == 'I' ? 'i' : *type == 'F' ? 'f': 's'),
        str_f("tarr_peek%c(%s)", *type, pars)
      );
    if (!strcmp(function, "pop"))
      return check(TRUE, str_new_c(*type),
        str_f("%c", *type == 'I' ? 'i' : *type == 'F' ? 'f': 's'),
        str_f("tarr_pop%c(%s)", *type, pars)
      );
    if (!strcmp(function, "push"))
      return (as_expression || !*type)
        ? check(FALSE, str_f("%c*", *type), "", "--??")
        : check(FALSE, str_f("%c%c", *type,
          *type == 'I' ? 'i' : *type == 'F' ? 'f': 's'
          ), "", str_f("tarr_push%c(%s)", *type, pars)
        );
    if (!strcmp(function, "remove"))
      return check(TRUE, str_f("%ci", *type),
          str_f("%c", *type == 'I' ? 'i' : *type == 'F' ? 'f': 's'),
          str_f("tarr_remove%c(%s)", *type, pars)
        );
    if (!strcmp(function, "removeRange"))
      return check(TRUE, str_f("%cii", *type),
          str_new_c(*type), str_f("tarr_remove_range%c(%s)", *type, pars)
        );
    if (!strcmp(function, "reverse"))
      return check(TRUE, str_new_c(*type), str_new_c(*type),
        str_f("tarr_reverse%c(%s)", *type, pars)
      );
    if (!strcmp(function, "reverseIn"))
      return (as_expression || !*type)
        ? check(FALSE, str_f("%c", *type), "", "--??")
        : check(FALSE, str_f("%c", *type),
          "", str_f("tarr_reverse_in%c(%s)", *type, pars)
        );
    if (!strcmp(function, "setArr"))
      return (as_expression || !*type)
        ? check(FALSE, str_f("%ci*", *type), "", "--??")
        : check(FALSE, str_f("%ci%c", *type, *type),
          "", str_f("tarr_set_array%c(%s)", *type, pars)
        );

    if (!strcmp(function, "setRange"))
      return (as_expression || !*type)
        ? check(FALSE, str_f("%ci*ii", *type), "", "--??")
        : check(FALSE, str_f("%ci%cii", *type, *type),
          "", str_f("tarr_set_range%c(%s)", *type, pars)
        );
    if (!strcmp(function, "shift"))
      return check(TRUE, str_new_c(*type),
        str_f("%c", *type == 'I' ? 'i' : *type == 'F' ? 'f': 's'),
        str_f("tarr_shift%c(%s)", *type, pars)
      );
    if (!strcmp(function, "shuffle"))
      return (as_expression || !*type)
        ? check(FALSE, str_f("%c*", *type), "", "--??")
        : check(FALSE, str_f("%c", *type), "",
            str_f("tarr_shuffle%c(%s)", *type, pars))
        ;
    if (!strcmp(function, "size"))
      return check(TRUE, str_new_c(*type), "i",
        str_f("tarr_size%c(%s)", *type, pars)
      );
    if (!strcmp(function, "sort"))
      return (as_expression || !*type)
        ? check(FALSE, str_f("%cb", *type), "", "--??")
        : check(FALSE, str_f("%cb", *type),
          "", str_f("tarr_sort%c(%s)", *type, pars)
        );
    if (!strcmp(function, "take"))
      return check(TRUE, str_f("%ci", *type), str_new_c(*type),
        str_f("tarr_take%c(%s)", *type, pars)
      );
    if (!strcmp(function, "unshift"))
      return (as_expression || !*type)
        ? check(FALSE, str_f("%c*", *type), "", "--??")
        : check(FALSE, str_f("%c%c", *type,
          *type == 'I' ? 'i' : *type == 'F' ? 'f': 's'
          ), "", str_f("tarr_unshift%c(%s)", *type, pars)
        );
    return fail(str_f("Typed function '%s.%s' not found", module, function));
  }
  if (!strcmp(module, "b64")) {  // ---------------------------------------- b64
    if (!strcmp(function, "decode"))
      return check(TRUE, "s", "s", str_f("b64_decode(%s)", pars));
    if (!strcmp(function, "decodeBytes"))
      return check(TRUE, "s", "o", str_f("b64_decode_bytes(%s)", pars));
    if (!strcmp(function, "encode"))
      return check(TRUE, "s", "s", str_f("b64_encode(%s)", pars));
    if (!strcmp(function, "encodeBytes"))
      return check(TRUE, "o", "s", str_f("b64_encode_bytes(%s)", pars));
    return fail(str_f("Typed function '%s.%s' not found", module, function));
  }
  if (!strcmp(module, "bytes")) {  // ------------------------------------ bytes
    if (!strcmp(function, "add"))
      return check(TRUE, "oo", "o", str_f("bytes_add(%s)", pars));
    if (!strcmp(function, "drop"))
      return check(TRUE, "oi", "o", str_f("bytes_drop(%s)", pars));
    if (!strcmp(function, "fromArr"))
      return check(TRUE, "I", "o", str_f("bytes_from_arr(%s)", pars));
    if (!strcmp(function, "fromStr"))
      return check(TRUE, "s", "o", str_f("bytes_from_str(%s)", pars));
    if (!strcmp(function, "get"))
      return check(TRUE, "oi", "i", str_f("bytes_get(%s)", pars));
    if (!strcmp(function, "new"))
      return check(TRUE, "i", "o", str_f("bytes_new(%s)", pars));
    if (!strcmp(function, "set"))
      return (as_expression || !*type)
        ? check(FALSE, "oii", "", "--??")
        : check(FALSE, "oii", "",  str_f("bytes_set(%s)", pars));
        ;
    if (!strcmp(function, "size"))
      return check(TRUE, "o", "i", str_f("bytes_size(%s)", pars));
    if (!strcmp(function, "take"))
      return check(TRUE, "oi", "o", str_f("bytes_take(%s)", pars));
    if (!strcmp(function, "toArr"))
      return check(TRUE, "o", "I", str_f("bytes_to_arr(%s)", pars));
    if (!strcmp(function, "toStr"))
      return check(TRUE, "o", "s", str_f("bytes_to_str(%s)", pars));
    return fail(str_f("Typed function '%s.%s' not found", module, function));
  }
  if (!strcmp(module, "cryp")) {  // -------------------------------------- cryp
    if (!strcmp(function, "decode"))
      return check(TRUE, "ss", "s", str_f("cryp_decode(%s)", pars));
    if (!strcmp(function, "encode"))
      return check(TRUE, "ss", "s", str_f("cryp_encode(%s)", pars));
    if (!strcmp(function, "genK"))
      return check(TRUE, "i", "s", str_f("cryp_genk(%s)", pars));
    if (!strcmp(function, "key"))
      return check(TRUE, "si", "s", str_f("cryp_key(%s)", pars));
    return fail(str_f("Typed function '%s.%s' not found", module, function));
  }
  if (!strcmp(module, "dic")) {  // --------------------------------------- dic
    if (!strcmp(function, "copy"))
      return check(TRUE, "D", "D", str_f("tarr_copyS(%s)", pars));
    if (!strcmp(function, "fromArr"))
      return check(TRUE, "S", "D", str_f("dic_from_arr(%s)", pars));
    if (!strcmp(function, "get"))
      return check(TRUE, "Ds", "S", str_f("dic_get(%s)", pars));
    if (!strcmp(function, "hasKey"))
      return check(TRUE, "Ds", "b", str_f("dic_has_key(%s)", pars));
    if (!strcmp(function, "keys"))
      return check(TRUE, "D", "S", str_f("dic_keys(%s)", pars));
    if (!strcmp(function, "put"))
      return (as_expression || !*type)
        ? check(FALSE, "Dss", "", "--??")
        : check(FALSE, "Dss", "", str_f("dic_put(%s)", pars));
        ;
    if (!strcmp(function, "remove"))
      return (as_expression || !*type)
        ? check(FALSE, "Ds", "", "--??")
        : check(FALSE, "Ds", "", str_f("dic_remove(%s)", pars));
        ;
    if (!strcmp(function, "size"))
      return check(TRUE, "D", "i", str_f("dic_size(%s)", pars));
    if (!strcmp(function, "toArr"))
      return check(TRUE, "D", "S", str_f("tarr_copyS(%s)", pars));
    if (!strcmp(function, "values"))
      return check(TRUE, "D", "S", str_f("dic_values(%s)", pars));
    return fail(str_f("Typed function '%s.%s' not found", module, function));
  }
  if (!strcmp(module, "file")) {  // -------------------------------------- file
    if (!strcmp(function, "aopen"))
      return check(TRUE, "s", "o", str_f("file_aopen(%s)", pars));
    if (!strcmp(function, "base"))
      return check(TRUE, "s", "s", str_f("file_base(%s)", pars));
    if (!strcmp(function, "canonical"))
      return check(TRUE, "s", "s", str_f("file_canonical(%s)", pars));
    if (!strcmp(function, "cat"))
      return check(TRUE, "S", "s", str_f("file_cat(%s)", pars));
    if (!strcmp(function, "cd"))
      return (as_expression || !*type)
        ? check(FALSE, "s", "", "--??")
        : check(FALSE, "s", "", str_f("file_cd(%s)", pars));
        ;
    if (!strcmp(function, "clean"))
      return check(TRUE, "s", "s", str_f("file_clean(%s)", pars));
    if (!strcmp(function, "close"))
      return (as_expression || !*type)
        ? check(FALSE, "o", "", "--??")
        : check(FALSE, "o", "", str_f("file_close(%s)", pars));
        ;
    if (!strcmp(function, "copy"))
      return (as_expression || !*type)
        ? check(FALSE, "ss", "", "--??")
        : check(FALSE, "ss", "", str_f("file_copy(%s)", pars));
        ;
    if (!strcmp(function, "del"))
      return (as_expression || !*type)
        ? check(FALSE, "s", "", "--??")
        : check(FALSE, "s", "", str_f("file_del(%s)", pars));
        ;
    if (!strcmp(function, "dir"))
      return check(TRUE, "s", "S", str_f("file_dir(%s)", pars));
    if (!strcmp(function, "exists"))
      return check(TRUE, "s", "b", str_f("file_exists(%s)", pars));
    if (!strcmp(function, "isDirectory"))
      return check(TRUE, "s", "b", str_f("file_is_directory(%s)", pars));
    if (!strcmp(function, "isLink"))
      return check(TRUE, "s", "b", str_f("file_is_link(%s)", pars));
    if (!strcmp(function, "isRegular"))
      return check(TRUE, "s", "b", str_f("file_is_regular(%s)", pars));
    if (!strcmp(function, "extension"))
      return check(TRUE, "s", "s", str_f("file_extension(%s)", pars));
    if (!strcmp(function, "mkdir"))
      return (as_expression || !*type)
        ? check(FALSE, "s", "", "--??")
        : check(FALSE, "s", "", str_f("file_mkdir(%s)", pars));
        ;
    if (!strcmp(function, "mklink"))
      return (as_expression || !*type)
        ? check(FALSE, "ss", "", "--??")
        : check(FALSE, "ss", "", str_f("file_mklink(%s)", pars));
        ;
    if (!strcmp(function, "parent"))
      return check(TRUE, "s", "s", str_f("file_parent(%s)", pars));
    if (!strcmp(function, "read"))
      return check(TRUE, "s", "s", str_f("file_read(%s)", pars));
    if (!strcmp(function, "readBin"))
      return check(TRUE, "oi", "o", str_f("file_read_bin(%s)", pars));
    if (!strcmp(function, "readLine"))
      return check(TRUE, "o", "S", str_f("file_read_line(%s)", pars));
    if (!strcmp(function, "rename"))
      return (as_expression || !*type)
        ? check(FALSE, "ss", "", "--??")
        : check(FALSE, "ss", "", str_f("file_rename(%s)", pars));
        ;
    if (!strcmp(function, "ropen"))
      return check(TRUE, "s", "o", str_f("file_ropen(%s)", pars));
    if (!strcmp(function, "size"))
      return check(TRUE, "s", "i", str_f("file_size(%s)", pars));
    if (!strcmp(function, "tm"))
      return check(TRUE, "s", "i", str_f("file_tm(%s)", pars));
    if (!strcmp(function, "tmp"))
      return check(TRUE, "ss", "s", str_f("file_tmp(%s)", pars));
    if (!strcmp(function, "wd"))
      return check(TRUE, "", "s", "file_wd()");
    if (!strcmp(function, "wopen"))
      return check(TRUE, "s", "o", str_f("file_wopen(%s)", pars));
    if (!strcmp(function, "write"))
      return (as_expression || !*type)
        ? check(FALSE, "ss", "", "--??")
        : check(FALSE, "ss", "", str_f("file_write(%s)", pars));
        ;
    if (!strcmp(function, "writeBin"))
      return (as_expression || !*type)
        ? check(FALSE, "oo", "", "--??")
        : check(FALSE, "oo", "", str_f("file_write_bin(%s)", pars));
        ;
    if (!strcmp(function, "writeText"))
      return (as_expression || !*type)
        ? check(FALSE, "os", "", "--??")
        : check(FALSE, "os", "", str_f("file_write_text(%s)", pars));
        ;
    return fail(str_f("Typed function '%s.%s' not found", module, function));
  }
  if (!strcmp(module, "js")) {  // ------------------------------------------ js
    if (!strcmp(function, "isNull"))
      return check(TRUE, "s", "b", str_f("js_is_null(%s)", pars));
    if (!strcmp(function, "rb"))
      return check(TRUE, "s", "b", str_f("js_rb(%s)", pars));
    if (!strcmp(function, "ri"))
      return check(TRUE, "s", "i", str_f("js_ri(%s)", pars));
    if (!strcmp(function, "rf"))
      return check(TRUE, "s", "f", str_f("js_rf(%s)", pars));
    if (!strcmp(function, "rs"))
      return check(TRUE, "s", "s", str_f("js_rs(%s)", pars));
    if (!strcmp(function, "ra"))
      return check(TRUE, "s", "S", str_f("js_ra(%s)", pars));
    if (!strcmp(function, "ro"))
      return check(TRUE, "s", "D", str_f("js_ro(%s)", pars));
    if (!strcmp(function, "wb"))
      return check(TRUE, "b", "s", str_f("js_wb(%s)", pars));
    if (!strcmp(function, "wi"))
      return check(TRUE, "i", "s", str_f("js_wi(%s)", pars));
    if (!strcmp(function, "wf"))
      return check(TRUE, "f", "s", str_f("js_wf(%s)", pars));
    if (!strcmp(function, "wf2"))
      return check(TRUE, "fi", "s", str_f("js_wf2(%s)", pars));
    if (!strcmp(function, "ws"))
      return check(TRUE, "s", "s", str_f("js_ws(%s)", pars));
    if (!strcmp(function, "wa"))
      return check(TRUE, "S", "s", str_f("js_wa(%s)", pars));
    if (!strcmp(function, "wo"))
      return check(TRUE, "D", "s", str_f("js_wo(%s)", pars));
    return fail(str_f("Typed function '%s.%s' not found", module, function));
  }
  if (!strcmp(module, "math")) {  // -------------------------------------- math
    if (!strcmp(function, "abs"))
      return check(TRUE, "f", "f", str_f("math_abs(%s)", pars));
    if (!strcmp(function, "acos"))
      return check(TRUE, "f", "f", str_f("acos(%s)", pars));
    if (!strcmp(function, "acosh"))
      return check(TRUE, "f", "f", str_f("acosh(%s)", pars));
    if (!strcmp(function, "asin"))
      return check(TRUE, "f", "f", str_f("asin(%s)", pars));
    if (!strcmp(function, "asinh"))
      return check(TRUE, "f", "f", str_f("asinh(%s)", pars));
    if (!strcmp(function, "atan"))
      return check(TRUE, "f", "f", str_f("atan(%s)", pars));
    if (!strcmp(function, "atanh"))
      return check(TRUE, "f", "f", str_f("atanh(%s)", pars));
    if (!strcmp(function, "ceil"))
      return check(TRUE, "f", "f", str_f("ceil(%s)", pars));
    if (!strcmp(function, "cos"))
      return check(TRUE, "f", "f", str_f("cos(%s)", pars));
    if (!strcmp(function, "cosh"))
      return check(TRUE, "f", "f", str_f("cosh(%s)", pars));
    if (!strcmp(function, "e"))
      return check(TRUE, "", "f", "M_E()");
    if (!strcmp(function, "eq"))
      return check(TRUE, "fff", "b", str_f("math_eq_gap(%s)", pars));
    if (!strcmp(function, "exp"))
      return check(TRUE, "f", "f", str_f("exp(%s)", pars));
    if (!strcmp(function, "exp2"))
      return check(TRUE, "f", "f", str_f("exp2(%s)", pars));
    if (!strcmp(function, "floor"))
      return check(TRUE, "f", "f", str_f("floor(%s)", pars));
    if (!strcmp(function, "fromEn"))
      return check(TRUE, "s", "F", str_f("math_from_en(%s)", pars));
    if (!strcmp(function, "fromHex"))
      return check(TRUE, "s", "I", str_f("math_from_hex(%s)", pars));
    if (!strcmp(function, "fromIso"))
      return check(TRUE, "s", "F", str_f("math_from_iso(%s)", pars));
    if (!strcmp(function, "ftoi"))
      return check(TRUE, "f", "i", str_f("((int64_t)(%s))", pars));
    if (!strcmp(function, "ftos"))
      return check(TRUE, "f", "s", str_f("math_ftos(%s)", pars));
    if (!strcmp(function, "isDigits"))
      return check(TRUE, "s", "b", str_f("math_digits(%s)", pars));
    if (!strcmp(function, "itof"))
      return check(TRUE, "i", "f", str_f("((double)(%s))", pars));
    if (!strcmp(function, "itoh"))
      return check(TRUE, "i", "s", str_f("str_f(\"%%x\", %s)", pars));
    if (!strcmp(function, "itos"))
      return check(TRUE, "i", "s", str_f("str_f(\"%%ld\", %s)", pars));
    if (!strcmp(function, "log"))
      return check(TRUE, "f", "f", str_f("log(%s)", pars));
    if (!strcmp(function, "log10"))
      return check(TRUE, "f", "f", str_f("log10(%s)", pars));
    if (!strcmp(function, "log2"))
      return check(TRUE, "f", "f", str_f("log2(%s)", pars));
    if (!strcmp(function, "max"))
      return check(TRUE, "ff", "f", str_f("math_max(%s)", pars));
    if (!strcmp(function, "maxFloat"))
      return check(TRUE, "", "f", "DBL_MAX");
    if (!strcmp(function, "maxInt"))
      return check(TRUE, "", "i",
        "(sizeof(int64_t) == sizeof(long long int)"
        "? LLONG_MAX"
        ": LONG_MAX)"
      );
    if (!strcmp(function, "min"))
      return check(TRUE, "ff", "f", str_f("math_min(%s)", pars));
    if (!strcmp(function, "minInt"))
      return check(TRUE, "", "i",
        "(sizeof(int64_t) == sizeof(long long int)"
        "? LLONG_MIN"
        ": LONG_MIN)"
      );
    if (!strcmp(function, "pi"))
      return check(TRUE, "", "f", "M_PI()");
    if (!strcmp(function, "pow"))
      return check(TRUE, "ff", "f", str_f("pow(%s)", pars));
    if (!strcmp(function, "pow10"))
      return check(TRUE, "i", "f", str_f("math_pow10(%s)", pars));
    if (!strcmp(function, "rnd"))
      return check(TRUE, "", "f", str_f("math_rnd(%s)", pars));
    if (!strcmp(function, "rndi"))
      return check(TRUE, "i", "i", str_f("math_rndi(%s)", pars));
    if (!strcmp(function, "round"))
      return check(TRUE, "fi", "f", str_f("math_round(%s)", pars));
    if (!strcmp(function, "sin"))
      return check(TRUE, "f", "f", str_f("sin(%s)", pars));
    if (!strcmp(function, "sinh"))
      return check(TRUE, "f", "f", str_f("sinh(%s)", pars));
    if (!strcmp(function, "sqrt"))
      return check(TRUE, "f", "f", str_f("sqrt(%s)", pars));
    if (!strcmp(function, "stof"))
      return check(TRUE, "s", "F", str_f("math_stof(%s)", pars));
    if (!strcmp(function, "stoi"))
      return check(TRUE, "s", "I", str_f("math_stoi(%s)", pars));
    if (!strcmp(function, "tan"))
      return check(TRUE, "f", "f", str_f("tan(%s)", pars));
    if (!strcmp(function, "tanh"))
      return check(TRUE, "f", "f", str_f("tanh(%s)", pars));
    if (!strcmp(function, "trunc"))
      return check(TRUE, "f", "f", str_f("trunc(%s)", pars));
    return fail(str_f("Typed function '%s.%s' not found", module, function));
  }
  if (!strcmp(module, "regex")) {  // ------------------------------------ regex
    if (!strcmp(function, "matches"))
      return check(TRUE, "ss", "S", str_f("regex_matches(%s)", pars));
    if (!strcmp(function, "replace"))
      return check(TRUE, "sss", "s", str_f("regex_replace(%s)", pars));
    return fail(str_f("Typed function '%s.%s' not found", module, function));
  }
  if (!strcmp(module, "str")) {  // ---------------------------------------- str
    if (!strcmp(function, "cmp"))
      return check(TRUE, "ss", "i", str_f("strcoll(%s)", pars));
    if (!strcmp(function, "ends"))
      return check(TRUE, "ss", "b", str_f("str_ends(%s)", pars));
    if (!strcmp(function, "fmt"))
      return check(TRUE, "sS", "s", str_f("str_fmt(%s)", pars));
    if (!strcmp(function, "fromIso"))
      return check(TRUE, "s", "s", str_f("str_from_iso(%s)", pars));
    if (!strcmp(function, "fromRunes"))
      return check(TRUE, "S", "s", str_f("tarr_join(%s, \"\")", pars));
    if (!strcmp(function, "fromUtf16"))
      return check(TRUE, "I", "s", str_f("str_from_utf16(%s)", pars));
    if (!strcmp(function, "index"))
      return check(TRUE, "ss", "i", str_f("str_index(%s)", pars));
    if (!strcmp(function, "indexFrom"))
      return check(TRUE, "ssi", "i", str_f("str_index_from(%s)", pars));
    if (!strcmp(function, "lastIndex"))
      return check(TRUE, "ss", "i", str_f("str_last_index(%s)", pars));
    if (!strcmp(function, "len"))
      return check(TRUE, "s", "i", str_f("((int64_t)strlen(%s))", pars));
    if (!strcmp(function, "ltrim"))
      return check(TRUE, "s", "s", str_f("str_ltrim(%s)", pars));
    if (!strcmp(function, "replace"))
      return check(TRUE, "sss", "s", str_f("str_replace(%s)", pars));
    if (!strcmp(function, "rtrim"))
      return check(TRUE, "s", "s", str_f("str_rtrim(%s)", pars));
    if (!strcmp(function, "split"))
      return check(TRUE, "ss", "S", str_f("str_split(%s)", pars));
    if (!strcmp(function, "splitTrim"))
      return check(TRUE, "ss", "S", str_f("str_split_trim(%s)", pars));
    if (!strcmp(function, "starts"))
      return check(TRUE, "ss", "b", str_f("str_starts(%s)", pars));
    if (!strcmp(function, "toLower"))
      return check(TRUE, "s", "s", str_f("str_to_lower(%s)", pars));
    if (!strcmp(function, "toRunes"))
      return check(TRUE, "s", "S", str_f("str_to_runes(%s)", pars));
    if (!strcmp(function, "toUpper"))
      return check(TRUE, "s", "s", str_f("str_to_upper(%s)", pars));
    if (!strcmp(function, "toUtf16"))
      return check(TRUE, "s", "I", str_f("str_to_utf16(%s)", pars));
    if (!strcmp(function, "trim"))
      return check(TRUE, "s", "s", str_f("str_trim(%s)", pars));
    return fail(str_f("Typed function '%s.%s' not found", module, function));
  }
  if (!strcmp(module, "sys")) {  // ---------------------------------------- sys
    if (!strcmp(function, "assert"))
      return (as_expression || !*type)
        ? check(FALSE, "b", "", "--??")
        : check(FALSE, "b", "", str_f("sys_assert(%s, @#@)", pars))
      ;
    if (!strcmp(function, "cmd"))
      return check(TRUE, "sS", "S", str_f("sys_cmd(%s)", pars));
    if (!strcmp(function, "environ"))
      return check(TRUE, "", "D", "sys_environ()");
    if (!strcmp(function, "exit"))
      return (as_expression || !*type)
        ? check(FALSE, "i", "", "--??")
        : check(FALSE, "i", "", str_f("exit(%s)", pars));
        ;
    if (!strcmp(function, "home"))
      return check(TRUE, "", "s", "sys_home()");
    if (!strcmp(function, "getLocale"))
      return check(TRUE, "", "s", "sys_get_locale()");
    if (!strcmp(function, "pid"))
      return check(TRUE, "", "i", "getpid()");
    if (!strcmp(function, "print"))
      return (as_expression || !*type)
        ? check(FALSE, "*", "", "--??")
        : check(FALSE, str_new_c(*type), "",
            str_f("sys_print%c(%s)", *type, pars)
          )
      ;
    if (!strcmp(function, "printError"))
      return (as_expression || !*type)
        ? check(FALSE, "*", "", "--??")
        : check(FALSE, str_new_c(*type), "",
            str_f("sys_print_error%c(%s)", *type, pars)
          )
      ;
    if (!strcmp(function, "println"))
      return (as_expression || !*type)
        ? check(FALSE, "*", "", "--??")
        : check(FALSE, str_new_c(*type), "",
            str_f("sys_println%c(%s)", *type, pars)
          )
      ;
    if (!strcmp(function, "readLine"))
      return check(TRUE, "", "s", "sys_read_line()");
    if (!strcmp(function, "setLocale"))
      return (as_expression || !*type)
        ? check(FALSE, "s", "", "--??")
        : check(FALSE, "s", "", str_f("sys_set_locale(%s)", pars))
      ;
    if (!strcmp(function, "sleep"))
      return (as_expression || !*type)
        ? check(FALSE, "i", "", "--??")
        : check(FALSE, "i", "", str_f("sys_sleep(%s)", pars));
        ;
    if (!strcmp(function, "test")){
      return (as_expression || !*type)
        ? check(FALSE, "**", "", "--??")
        : check(FALSE, str_f("%c%c", *type, *type), "",
            str_f("sys_test%c(%s, @#@)", *type, pars)
          )
      ;
    }
    if (!strcmp(function, "throw")){
      return (as_expression || !*type)
        ? check(FALSE, "*", "", "--??")
        : check(FALSE, "s", "",
            str_f("sys_throw(\"sys.throw\", %s, @#@)", pars)
          )
      ;
    }
    if (!strcmp(function, "toStr"))
      return check(TRUE, str_new_c(*type), "s",
        str_f("built_to_str%c(%s)", *type, pars)
      );
    if (!strcmp(function, "used"))
      return ok("sys_used()", ttype_NO_RETURN);
    if (!strcmp(function, "user"))
      return check(TRUE, "", "s", "sys_user()");
    return fail(str_f("Typed function '%s.%s' not found", module, function));
  }
  if (!strcmp(module, "time")) {  // -------------------------------------- time
    if (!strcmp(function, "addDays"))
      return check(TRUE, "ii", "i", str_f("time_add_days(%s)", pars));
    if (!strcmp(function, "day"))
      return check(TRUE, "i", "i", str_f("time_day(%s)", pars));
    if (!strcmp(function, "dfDays"))
      return check(TRUE, "ii", "i", str_f("time_df_day(%s)", pars));
    if (!strcmp(function, "eqDay"))
      return check(TRUE, "ii", "b", str_f("time_eq_day(%s)", pars));
    if (!strcmp(function, "fmt"))
      return check(TRUE, "is", "s", str_f("time_fmt(%s)", pars));
    if (!strcmp(function, "fromClock"))
      return check(TRUE, "is", "I", str_f("time_from_clock(%s)", pars));
    if (!strcmp(function, "fromEn"))
      return check(TRUE, "ss", "I", str_f("time_from_en(%s)", pars));
    if (!strcmp(function, "fromIso"))
      return check(TRUE, "ss", "I", str_f("time_from_iso(%s)", pars));
    if (!strcmp(function, "fromStr"))
      return check(TRUE, "s", "I", str_f("time_from_str(%s)", pars));
    if (!strcmp(function, "hour"))
      return check(TRUE, "i", "i", str_f("time_hour(%s)", pars));
    if (!strcmp(function, "millisecond"))
      return check(TRUE, "i", "i", str_f("time_millisecond(%s)", pars));
    if (!strcmp(function, "minute"))
      return check(TRUE, "i", "i", str_f("time_minute(%s)", pars));
    if (!strcmp(function, "month"))
      return check(TRUE, "i", "i", str_f("time_month(%s)", pars));
    if (!strcmp(function, "new"))
      return check(TRUE, "iiiiii", "i", str_f("time_new_time(%s)", pars));
    if (!strcmp(function, "newDate"))
      return check(TRUE, "iii", "i", str_f("time_new(%s)", pars));
    if (!strcmp(function, "now"))
      return check(TRUE, "", "i", "time_now()");
    if (!strcmp(function, "second"))
      return check(TRUE, "i", "i", str_f("time_second(%s)", pars));
    if (!strcmp(function, "toEn"))
      return check(TRUE, "i", "s", str_f("time_to_en(%s)", pars));
    if (!strcmp(function, "toIso"))
      return check(TRUE, "i", "s", str_f("time_to_iso(%s)", pars));
    if (!strcmp(function, "toStr"))
      return check(TRUE, "i", "s", str_f("time_to_str(%s)", pars));
    if (!strcmp(function, "weekday"))
      return check(TRUE, "i", "i", str_f("time_weekday(%s)", pars));
    if (!strcmp(function, "year"))
      return check(TRUE, "i", "i", str_f("time_year(%s)", pars));
    if (!strcmp(function, "yearDay"))
      return check(TRUE, "i", "i", str_f("time_year_day(%s)", pars));
    return fail(str_f("Typed function '%s.%s' not found", module, function));
  }
  return fail(str_f("Typed module '%s' not found", module));
}
