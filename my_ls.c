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
  struct s_listnode *next;
} listnode;
#endif

#ifndef STRUCT_DIRNODE
#define STRUCT_DIRNODE
typedef struct s_dirnode {
  char dir_name[256];
  struct s_dirnode *next_dir;
  struct s_listnode *current_file;
} dirnode;
#endif

void read_list(listnode *list) {
  while (list != NULL) {
    if (strlen(list->val) > 0) {
      printf("%s\n", list->val);
    }
    list = list->next;
  }
}

int is_sorted(listnode *param) {
  listnode *temp = param;
  listnode *next_temp = temp->next;

  while (temp->next != NULL) {
    if (temp == NULL) {
    }

    if (temp->val[0] > next_temp->val[0]) {
      return 0;
    }

    temp = temp->next;
    if (temp->next == NULL) {
      return 1;
    }
    next_temp = temp->next;
  }
  return 1;
}

listnode *sort_lists(listnode *list) {
  if (is_sorted(list)) {
    return list;
  }

  listnode *current = list;
  listnode *next = list->next;
  int swapped = 0; // flag to check if any swaps were made

  while (current->next != NULL) {
    if (strcmp(current->val, next->val) > 0) {
      char temp_val[256];
      strncpy(temp_val, current->val, 255);
      temp_val[255] = '\0'; // ensure null termination

      strncpy(current->val, next->val, 255);
      current->val[255] = '\0'; // ensure null termination

      strncpy(next->val, temp_val, 255);
      next->val[255] = '\0'; // ensure null termination

      swapped = 1; // a swap was made
    }
    current = current->next;
    next = current->next;
  }

  if (swapped) { // only call sort_lists again if a swap was made
    return sort_lists(list);
  } else {
    return list;
  }
}

dirnode *add_to_list(char *arg, dirnode *current_list, int show_hidden_files) {
  DIR *d;
  struct dirent *dir;
  d = opendir(arg);
  strncpy(current_list->dir_name, arg, 255);
  current_list->current_file = (listnode *)malloc(sizeof(listnode));
  listnode *head_file = current_list->current_file;
  listnode *current_file = head_file;

  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (show_hidden_files ||
          (dir->d_name[0] != '.' && dir->d_name[0] != ' ')) {
        strncpy(current_file->val, dir->d_name, 255);
        current_file->next = (listnode *)malloc(sizeof(listnode));
        current_file = current_file->next;
        current_file->val[0] = '\0';
        current_file->next = NULL;
      }
    }
    closedir(d);
  }

  sort_lists(head_file);
  read_list(head_file);

  return current_list;
}

int main(int argc, char *argv[]) {
  int time_sorted = 0;
  int show_hidden_files = 0;
  int dir_count = 0;
  dirnode *head;
  dirnode *current;
  head = (dirnode *)malloc(sizeof(dirnode));
  current = head;
  if (argc > 1) {
    int directory_traversed = 0;
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-a") == 0) {
        show_hidden_files = 1;
      }
      if (strcmp(argv[i], "-t") == 0) {
        time_sorted = 1;
      }
      if (strcmp(argv[i], "-ta") == 0) {
        show_hidden_files = 1;
        time_sorted = 1;
      }
      if (argv[i][0] != '-') {
        dir_count++;
        if (dir_count > 1) {
          current = current->next_dir;
        }
        current = add_to_list(argv[i], current, show_hidden_files);
        directory_traversed = 1;
      }
    }
    if (show_hidden_files && !directory_traversed) {
      current = add_to_list(".", current, show_hidden_files);
    }
  } else {
    current = add_to_list(".", current, show_hidden_files);
  }

  return (0);
}
