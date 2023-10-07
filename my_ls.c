#include <dirent.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef STRUCT_LISTNODE
#define STRUCT_LISTNODE
typedef struct s_listnode {
  char val[256];
  time_t st_mtim;
  struct s_listnode *next;
} listnode;
#endif

#ifndef STRUCT_DIRNODE
#define STRUCT_DIRNODE
typedef struct s_dirnode {
  char dir_name[256];
  struct s_dirnode *next_dir;
  struct s_listnode *curr_file;
} dirnode;
#endif

void read_files(listnode *list) {
  while (list != NULL) {
    if (strlen(list->val) > 0) {
      printf("%s\n", list->val);
    }
    list = list->next;
  }
}

listnode *sort_files(listnode *list) {
  int swapped;

  do {
    swapped = 0;
    listnode *current = list;
    listnode *next = list->next;

    while (current->next != NULL) {
      if (strcmp(current->val, next->val) > 0) {
        char temp_val[256];
        strncpy(temp_val, current->val, 255);
        temp_val[255] = '\0';

        strncpy(current->val, next->val, 255);
        current->val[255] = '\0';

        strncpy(next->val, temp_val, 255);
        next->val[255] = '\0';

        swapped = 1;
      }
      current = current->next;
      next = current->next;
    }
  } while (swapped);
  return list;
}

void *selection_sort_time(listnode *list) {
  listnode *current = list;
  struct stat file_stats_one;
  struct stat file_stats_two;

  while (current != NULL) {
    int result_one = stat(current->val, &file_stats_one);

    listnode *min_node = current;
    time_t min_time = file_stats_one.st_mtime;

    for (listnode *iter = current->next; iter != NULL; iter = iter->next) {
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

dirnode *sort_dirs(dirnode *list) {
  int swapped;

  do {
    swapped = 0;
    dirnode *current = list;
    dirnode *next = list->next_dir;

    while (current->next_dir != NULL) {
      if (strcmp(current->dir_name, next->dir_name) > 0) {
        char temp_val[256];
        strncpy(temp_val, current->dir_name, 255);
        temp_val[255] = '\0';

        strncpy(current->dir_name, next->dir_name, 255);
        current->dir_name[255] = '\0';

        strncpy(next->dir_name, temp_val, 255);
        next->dir_name[255] = '\0';

        swapped = 1;
      }
      current = current->next_dir;
      next = current->next_dir;
    }
  } while (swapped);

  return list;
}

void add_to_list(struct dirent *dir, DIR *d, listnode *curr_file,
                 int show_hidden) {
  while ((dir = readdir(d)) != NULL) {
    if (show_hidden || (dir->d_name[0] != '.' && dir->d_name[0] != ' ')) {
      struct stat file_stats_one;
      int dir_stats = stat(dir->d_name, &file_stats_one);
      time_t iter_time = file_stats_one.st_mtime;

      strncpy(curr_file->val, dir->d_name, 255);
      curr_file->st_mtim = iter_time;

      curr_file->next = (listnode *)malloc(sizeof(listnode));
      curr_file = curr_file->next;
      curr_file->val[0] = '\0';
      curr_file->next = NULL;
    }
  }
}

dirnode *check_list(char *dir_to_check, dirnode *curr_dir, int show_hidden,
                    int sort_time) {
  DIR *d;
  struct dirent *dir;
  d = opendir(dir_to_check);
  strncpy(curr_dir->dir_name, dir_to_check, 255);
  curr_dir->curr_file = (listnode *)malloc(sizeof(listnode));
  listnode *head_file = curr_dir->curr_file;
  listnode *curr_file = head_file;

  if (d) {
    add_to_list(dir, d, curr_file, show_hidden);
    closedir(d);
  }
    sort_files(head_file);

  if (sort_time) {
    selection_sort_time(head_file);
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
  strncpy(current->dir_name, ".", 255);

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
          current->next_dir = (dirnode *)malloc(sizeof(dirnode));
          current = current->next_dir;
          strcat(current->dir_name, argv[i]);
        } else {
          current->dir_name[0] = '\0';
          strcat(current->dir_name, argv[i]);
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
      printf("%s:\n", head->dir_name);
    }
    check_list(head->dir_name, head, show_hidden, sort_time);
    head = head->next_dir;
  }

  return (0);
}

// Can you run ./my_ls -t and it prints the content of the current directory
// (sorted by time)?

// Can you run ./my_ls -ta and it prints the content of the marvel directory
// (sorted by time) + hidden files?

// Can you run ./my_ls -t -a and it prints the content of the marvel directory
// (sorted by time sec + nsec + alphanum ) + hidden files?