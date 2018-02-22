/*
 * Copyright 08-Mar-2017 ºDeme
 * GNU General Public License - V3 <http://www.gnu.org/licenses/>
 */

///
char *www_index();
///
char *www_styles();
///
char *www_dir_index(char *level, char *title);
///
char *www_img_index();
/// 'dir' is relative to 'src'
char *files_build(char *path_level, char *dir, char *file);
///
char *files_main(char *name);
/// 'dir' is relative to 'src'
char *files_buildS(char *path_level, char *dir, char *file);
///
char *files_mainS(char *name);
