#include "sort_header.h"

void read_files(filenode *list) {
  while (list != NULL) {
    if (strlen(list->val) > 0) {
      printf("%s\n", list->val);
    }
    list = list->next;
  }
}

void add_to_list(struct dirent *dir, DIR *d, filenode *next_file,
                 int show_hidden) {
  while ((dir = readdir(d)) != NULL) {
    if (show_hidden || (dir->d_name[0] != '.' && dir->d_name[0] != ' ')) {
      struct stat file_stats_one;
      stat(dir->d_name, &file_stats_one);
      time_t iter_time = file_stats_one.st_mtime;

      strncpy(next_file->val, dir->d_name, 255);
      next_file->st_mtim = iter_time;

      next_file->next = (filenode *)malloc(sizeof(filenode));
      next_file = next_file->next;
      next_file->val[0] = '\0';
      next_file->next = NULL;
    }
  }
}

dirnode *check_list(dirnode *curr_dir, int show_hidden, int sort_time) {
  char *dir_to_check = curr_dir->val;
  DIR *d;
  struct dirent *dir = malloc(sizeof(*dir));
  d = opendir(dir_to_check);
  strncpy(curr_dir->val, dir_to_check, 255);
  curr_dir->next_file = (filenode *)malloc(sizeof(filenode));
  filenode *head_file = curr_dir->next_file;
  filenode *next_file = head_file;

  if (d) {
    add_to_list(dir, d, next_file, show_hidden);
    closedir(d);
  }

  if (sort_time) {
    selection_sort_time(head_file);
  } else {
    sort_files(head_file);
  }
  read_files(head_file);

  return curr_dir;
}