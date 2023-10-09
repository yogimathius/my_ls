/* primary_header.h */
#ifndef SORT_HEADER_H
#define SORT_HEADER_H

#include <dirent.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef STRUCT_FILENODE
#define STRUCT_FILENODE
typedef struct s_filenode {
  char val[256];
  time_t st_mtim;
  struct s_filenode *next;
  struct s_filenode *next_dir;
} filenode;
#endif

#ifndef STRUCT_DIRNODE
#define STRUCT_DIRNODE
typedef struct s_dirnode {
  char val[256];
  struct s_dirnode *next;
  struct s_filenode *next_file;
} dirnode;
#endif

filenode *sort_files(filenode *list);

dirnode *sort_dirs(dirnode *list);

void *selection_sort_time(filenode *list);

dirnode *check_list(dirnode *curr_dir, int show_hidden, int sort_time);

#endif /* SORT_HEADER_H */