// Copyright 27-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Company data.<p>
/// Working:
/// <table><td><tr>
///   * Before process closes 'co_new' must be called.
///   * Every day a close is added with 'co_add'. This function returns 'true'
///     if calling co_up or co_down can be done (There are not values <= 0).
///   * When 'co_buying' is 'true' a buy is expected, otherwise is expected
///     a 'sell'. At the beginning a 'sell' is expected.
///   * After call 'co_add' wiht 'true' result 'co_up' should be called is
///     'co_buying' is 'true', otherwise co_down should be called.
///   * <u>If 'co_up' or 'co_down' return 'true', 'co_buying' must be changed</u>
///     <u>and 'co_set_ref' must set up with '*co_closes' value</u>.
/// </table>

#ifndef CO_H
  #define CO_H

#include "dmc/std.h"

/*--*/

///
typedef struct co_Co Co;

///
void co_free(Co *this);

///
double *co_closes(Co *this);

///  Last close
double *co_last_close(Co *this);

///  Buying status
int co_buying(Co *this);

///  Reference to calculate strips
double co_ref(Co *this);

///  Buying status
void co_set_buying(Co *this, int v);

///  Reference to calculate strips
void co_set_ref(Co *this, double v);

/*--*/

#endif

///
Co *co_new(int ndays);

/// Adds a close and returns ('true'/'false') if closes (are/are not) available
/// to calculate co_up or co_down.
int co_add(Co *this, double close);

/// Returns the percentual difference with strip if (*this->last_close -
/// this->ref) / this->ref is greater than strip, or -1 otherwise.
double co_up(Co *this, double strip);

/// Returns the percentual difference with strip if (this->ref -
/// *this->last_close) / this->ref is greater than strip, or -1 otherwise.
double co_down(Co *this, double strip);
