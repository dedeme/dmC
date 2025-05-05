
// time ---------------------------

// i, i -> i
Val __time_addDays (Val tm, Val days);

// i -> i
Val __time_day (Val tm);

// i, i -> i
Val __time_dfDays (Val tm1, Val tm2);

// i, i -> b
Val __time_eqDay (Val tm1, Val tm2);

// i, s -> s
Val __time_fmt (Val tm, Val template);

// i, s -> [i]
Val __time_fromClock (Val tm, Val template);

// s, s -> [i]
Val __time_fromEn (Val s, Val sep);

// s, s -> [i]
Val __time_fromIso (Val s, Val sep);

// s -> [i]
Val __time_fromStr (Val s);

// i -> i
Val __time_hour (Val tm);

// i -> i
Val __time_millisecond (Val tm);

// i -> i
Val __time_minute (Val tm);

// i -> i
Val __time_month (Val tm);

// i, i, i, i, i, i -> i
Val __time_new (Val day, Val month, Val year, Val hour, Val minute, Val second);

// i, i, i -> i
Val __time_newDate (Val day, Val month, Val year);

// -> i
Val __time_now (void);

// i -> i
Val __time_second (Val tm);

// i -> s
Val __time_toEn (Val tm);

// i -> s
Val __time_toIso (Val tm);

// i -> s
Val __time_toStr (Val tm);

// i -> i
Val __time_weekday (Val tm);

// i -> i
Val __time_year (Val tm);

// i -> i
Val __time_yearDay (Val tm);
