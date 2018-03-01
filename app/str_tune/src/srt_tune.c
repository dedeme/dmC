// Copyright 26-Dec-2017 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dm/dm.h>

void help () {
  puts(
    "str_tune. Version 201802\n\n"
    "This program modifies a file called 'source.str' placed in the\n"
    "current directory.\n\n"
    "Use:\n"
    "srt_tune begin_milliseconds end_milliseconds\n"
    "  begin_milliseconds Increment first subtitle (It can be negative)\n"
    "  end_milliseconds Increment last subtitle (It can be negative)\n"
    "For example:\n"
    "str_tune -1200 2000"
  );
}

int toMilliseconds (char *t) {
  Arr *s_ms = str_csplit(t, ',');
  Arr *h_m_s = str_csplit(arr_get(s_ms, 0), ':');
  int r = atoi(arr_get(h_m_s, 0)) * 3600000 +
    atoi(arr_get(h_m_s, 1)) * 60000 +
    atoi(arr_get(h_m_s, 2)) * 1000 +
    atoi(arr_get(s_ms, 1));
  return r;
}

char *add (int t, int inc) {
  int tms = t + inc;
  int h = tms / 3600000;
  int rh = tms % 3600000;
  int m = rh / 60000;
  int rm = rh % 60000;
  int s = rm / 1000;
  int ms = rm % 1000;

  return str_printf("%02d:%02d:%02d,%03d", h, m, s, ms);
}

int main (int argc, char **argv) {
  if (argc != 3) {
    help();
    return 0;
  }

  int bms = atoi(argv[1]);
  int ems = atoi(argv[2]);
  int ims = ems - bms;

  if (!file_exists("source.srt")) {
    puts ("File 'source.srt' does not exist");
    return 0;
  }

  char *lastTime = str_copy("");
  LckFile *lk = file_ropen("source.txt");
  for (;;) {
    char *line = file_read_line(lk);
    if (*line) {
      int ix = str_index(line, "-->");
      if (ix != -1) {
        lastTime = str_trim(str_sub(line, ix + 3, strlen(line)));
      }
    } else {
      break;
    }
  }
  file_close(lk);

  int lastTimeMs = toMilliseconds(lastTime);

  LckFile *wlk = file_wopen("target.srt");
  LckFile *rlk = file_ropen("source.txt");
  for (;;) {
    char *line = file_read_line(rlk);
    if (!*line) {
      break;
    }

    int ix = str_index(line, "-->");
    if (ix == -1) {
      file_write_text(wlk, line);
      continue;
    }

    int tm1ms = toMilliseconds(str_trim(str_sub(line, 0, ix)));
    char *tm1 = add(tm1ms, bms + (int)(ims * (tm1ms / (float)lastTimeMs)));

    int tm2ms = toMilliseconds(str_trim(str_sub(line, ix + 3, strlen(line))));
    char *tm2 = add(tm2ms, bms + (int)(ims * (tm2ms / (float)lastTimeMs)));

    file_write_text(wlk, str_printf("%s --> %s\n", tm1, tm2));
  }
  file_close(rlk);
  file_close(wlk);

  return 0;
}

