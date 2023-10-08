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

void read_files(filenode *list) {
  while (list != NULL) {
    if (strlen(list->val) > 0) {
      printf("%s\n", list->val);
    }
    list = list->next;
  }
}

int check_values(char *current_val, char *next_val) {
  if (strcmp(current_val, next_val) > 0) {
    char temp_val[256];
    strncpy(temp_val, current_val, 255);
    temp_val[255] = '\0';

    strncpy(current_val, next_val, 255);
    current_val[255] = '\0';

    strncpy(next_val, temp_val, 255);
    next_val[255] = '\0';

    return 1;
  }
  return 0;
}

filenode *sort_files(filenode *list) {
  int swapped;

  do {
    swapped = 0;
    filenode *current = list;
    filenode *next = list->next;

    while (current->next != NULL) {
      swapped = check_values(current->val, next->val);

      current = current->next;
      next = current->next;
    }
  } while (swapped);
  return list;
}

dirnode *sort_dirs(dirnode *list) {
  int swapped;

  do {
    swapped = 0;
    dirnode *current = list;
    dirnode *next = list->next;

    while (current->next != NULL) {
      swapped = check_values(current->val, next->val);
      current = current->next;
      next = current->next;
    }
  } while (swapped);

  return list;
}

void *selection_sort_time(filenode *list) {
  filenode *current = list;
  struct stat file_stats_one;
  struct stat file_stats_two;

  while (current != NULL) {
    int result_one = stat(current->val, &file_stats_one);

    filenode *min_node = current;
    time_t min_time = file_stats_one.st_mtime;

    for (filenode *iter = current->next; iter != NULL; iter = iter->next) {
      int iter_result = stat(iter->val, &file_stats_two);

      time_t iter_time = file_stats_two.st_mtime;

      if (iter_time < min_time ||
          (min_time == iter_time && strcmp(min_node->val, iter->val) < 0)) {
        min_node = iter;
        min_time = iter_time;
      }
    }
    char temp_val[256];
    strncpy(temp_val, current->val, 255);
    temp_val[255] = '\0';

    strncpy(current->val, min_node->val, 255);
    current->val[255] = '\0';

    strncpy(min_node->val, temp_val, 255);
    min_node->val[255] = '\0';

    current = current->next;
  }
}

void add_to_list(struct dirent *dir, DIR *d, filenode *next_file,
                 int show_hidden) {
  while ((dir = readdir(d)) != NULL) {
    if (show_hidden || (dir->d_name[0] != '.' && dir->d_name[0] != ' ')) {
      struct stat file_stats_one;
      int dir_stats = stat(dir->d_name, &file_stats_one);
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

dirnode *check_list(char *dir_to_check, dirnode *curr_dir, int show_hidden,
                    int sort_time) {
  DIR *d;
  struct dirent *dir;
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

int main(int argc, char *argv[]) {
  int sort_time = 0;
  int show_hidden = 0;
  int dir_count = 0;
  dirnode *head;
  dirnode *current;
  head = (dirnode *)malloc(sizeof(dirnode));
  current = head;
  strncpy(current->val, ".", 255);

  if (argc > 1) {
    int directory_traversed = 0;
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-a") == 0) {
        show_hidden = 1;
      }
      if (strcmp(argv[i], "-t") == 0) {
        sort_time = 1;
      }
      if (strcmp(argv[i], "-ta") == 0) {
        show_hidden = 1;
        sort_time = 1;
      }
      if (argv[i][0] != '-') {
        dir_count++;
        if (dir_count > 1) {
          current->next = (dirnode *)malloc(sizeof(dirnode));
          current = current->next;
          strcat(current->val, argv[i]);
        } else {
          current->val[0] = '\0';
          strcat(current->val, argv[i]);
        }
      }
    }
  }

  sort_dirs(head);
  int curr = 1;
  while (head != NULL) {
    if (dir_count > 1) {
      if (curr > 1) {
        printf("\n");
      }
      curr++;
      printf("%s:\n", head->val);
    }
    check_list(head->val, head, show_hidden, sort_time);
    head = head->next;
  }

  return (0);
}

// Can you run ./my_ls -t and it prints the content of the current directory
// (sorted by time)?

// Can you run ./my_ls -ta and it prints the content of the marvel directory
// (sorted by time) + hidden files?

// Can you run ./my_ls -t -a and it prints the content of the marvel directory
// (sorted by time sec + nsec + alphanum ) + hidden files?