// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

///String utilities

#ifndef DM_STR_H
  #define DM_STR_H

#include <stdbool.h>

typedef struct achar_Achar Achar;
typedef struct opt_Opt Opt;

/// str_len reuturns 's' length.
size_t str_len(char *s);

/// str_cmp reuturns '0' if s1 == s2, '> 0' if s1 > s2 and '< 0' if s1 < s2 .
int str_cmp(char *s1, char *s2);

/// str_eq returns 'true' if str1 == str2
bool str_eq(char *str1, char *str2);

/// str_starts indicates if str starts with substr
bool str_starts(char *str, char *substr);

/// str_ends indicates if str ends with substr
bool str_ends(char *str, char *substr);

/// str_cindex returns first position of 'ch' in 'str' or -1 if 'ch' is missing.
int str_cindex (char *str, char ch);

/// str_index returns first position of 'substr' in 'str' or -1 if 'substr'
/// is missing.
int str_index(char *str, char *substr);

/// str_last_cindex returns last position of 'ch' in 'str' or -1 if 'ch' is
/// missing.
int str_last_cindex(char *str, char ch);

/// str_last_index returns last position of 'substr' in 'str' or -1 if 'substr'
/// is missing.
int str_last_index(char *str, char *substr);

/// str_copy returns a copy of 'str'.
char *str_copy(char *source);

/// str_cat is a string concatenation.<br>
/// Variable argumens must finish with NULL.<br>
char *str_cat(char *s, ...);

/// str_sub returns a substring of 'str'.<br>
/// If 'begin' or 'end' are negatives they are subtracted from 'strlen(s)'.
/// <br>
/// If 'begin' or 'end' are out of bounds function returns "".
char *str_sub(char *str, int begin, int end);

/// str_sub_end is equals to str_sub(s, begin, strlen(s))
char *str_sub_end(char *str, int begin);

/// str_ltrim returns a new string removing spaces (' ', '\\f', '\\n', '\\r',
/// '\\t' and '\\v') at left.
char *str_ltrim(char *str);

/// str_rtrim returns a new string removing spaces (' ', '\\f', '\\n', '\\r',
/// '\\t' and '\\v') at right.
char *str_rtrim(char *str);

/// str_trim returns a new allocated string removing spaces (' ', '\\f',
/// '\\n', '\\r', '\\t' and '\\v') at left and right.
char *str_trim(char *str);

/// str_csplit splits 'str' in an Achar.<br>
/// For example (using ';' as separator):
///   "" -> []
///   ";" -> [""]
///   "a;" -> [a]
///   "a;bc" -> ["a", "bc"]
Achar *str_csplit (char *str, char sep);

/// str_csplit is similar to <tt>str_csplit</tt> but trimming elements
Achar *str_csplit_trim (char *str, char sep);

/// str_split splits 'str' in an Achar.<br>
/// For example (using ";" as separator):
///   "" -> []
///   ";" -> [""]
///   "a;" -> [a]
///   "a;bc" -> ["a", "bc"]
/// If 'sep' is '' return will have only one string equals to 'str'.
Achar *str_split (char *str, char *sep);

/// str_split_trim is similar to <tt>str_split</tt> but trimming elements
Achar *str_split_trim (char *str, char *sep);

/// str_cjoin joins elements of 'a', separated by 'sep'.
char *str_cjoin (Achar *a, char sep);

/// str_join joins elements of 'a', separated by 'sep'.
char *str_join (Achar *a, char *sep);

/// str_creplace replace 'old' by 'new' in 's'. 's' is unmodified
char *str_creplace(char *s, char old, char new);

/// str_replace replace 'old' by 'new' in 's'. 's' is unmodified.<br>
/// If 'old' is "", returns a copy of 's'.
char *str_replace(char *s, char *old, char *new);

/// str_printf returns a string with format similar to 'printf'
char *str_printf(char *format, ...);

/// str_runes returns utf8 caracter number or -1 if 's' is not a valid ut8 string.
int str_runes (char *s);

/// str_next_rune reads first rune of 's'.
///   s      : Text to read
///   return : Next rune or "" if end of 's' is reached
char *str_next_rune (char *s);

/// str_to_unicode codifies a string in Unicode. Returns NULL if there is
/// an error
Opt/*unsigned*/ *str_to_unicode (char *s);

/// str_from_unicaode decodifies a string from Unicode. Returns NULL if there
/// is an error
Opt/*char*/ *str_from_unicode (unsigned *u);

/// str_from_iso decodifies a string from ISO-8859-1
char *str_from_iso (char *s);

/// str_to_upper returns 's' converted to uppercase. It is necessary set
/// language previously.
/// For example:
///   sys_locale("es_ES.utf8");
///   printf("->%s\n", str_to_upper ("cañón"));
char *str_to_upper (char *s);

/// str_to_lower returns 's' converted to lowercase. It is necessary set
/// language previously.
/// For example:
///   sys_locale("es_ES.utf8");
///   printf("->%s\n", str_to_lower ("CAÑON"));
char *str_to_lower (char *s);

/// str_to_escape replaces " by \" and \ by \\ and insert the result inside
/// quotes.
char *str_to_escape (char *s);

/// str_from_escape restores the string escaped with 'escape'. If 's' does not
/// come from 'escape' the result is indefined.
char *str_from_escape (char *s);

#endif
