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

void read_files(listnode *list) {
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

listnode *sort_files(listnode *list) {
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

  if (swapped) { // only call sort_files again if a swap was made
    return sort_files(list);
  } else {
    return list;
  }
}

int is_sorted_dir(dirnode *param) {
  dirnode *temp = param;
  dirnode *next_temp = temp->next_dir;

  while (temp->next_dir != NULL) {

    if (temp->dir_name[0] > next_temp->dir_name[0]) {
      return 0;
    }

    for (int i = 0; i < strlen(temp->dir_name); i++) {
      for (int j = 0; j < strlen(next_temp->dir_name); j++) {
        if (temp->dir_name[i] > next_temp->dir_name[j]) {
          return 0;
        }
      }
    }

    temp = temp->next_dir;
    if (temp->next_dir == NULL) {
      return 1;
    }
    next_temp = temp->next_dir;
  }
  return 1;
}

dirnode *sort_dirs(dirnode *list) {
  if (is_sorted_dir(list)) {
    return list;
  }

  dirnode *current = list;
  dirnode *next = list->next_dir;
  int swapped = 0; // flag to check if any swaps were made

  while (current->next_dir != NULL) {
    if (strcmp(current->dir_name, next->dir_name) > 0) {
      char temp_val[256];
      strncpy(temp_val, current->dir_name, 255);
      temp_val[255] = '\0'; // ensure null termination

      strncpy(current->dir_name, next->dir_name, 255);
      current->dir_name[255] = '\0'; // ensure null termination

      strncpy(next->dir_name, temp_val, 255);
      next->dir_name[255] = '\0'; // ensure null termination

      swapped = 1; // a swap was made
    }
    current = current->next_dir;
    next = current->next_dir;
  }

  if (swapped) { // only call sort_files again if a swap was made
    return sort_dirs(list);
  } else {
    return list;
  }
}

void add_to_list(struct dirent *dir, DIR *d, listnode *current_file,
                 int show_hidden_files) {
  while ((dir = readdir(d)) != NULL) {
    if (show_hidden_files || (dir->d_name[0] != '.' && dir->d_name[0] != ' ')) {
      strncpy(current_file->val, dir->d_name, 255);
      current_file->next = (listnode *)malloc(sizeof(listnode));
      current_file = current_file->next;
      current_file->val[0] = '\0';
      current_file->next = NULL;
    }
  }
}

dirnode *check_list(char *directory_to_check, dirnode *current_list,
                    int show_hidden_files) {
  DIR *d;
  struct dirent *dir;
  d = opendir(directory_to_check);
  strncpy(current_list->dir_name, directory_to_check, 255);
  current_list->current_file = (listnode *)malloc(sizeof(listnode));
  listnode *head_file = current_list->current_file;
  listnode *current_file = head_file;

  if (d) {
    add_to_list(dir, d, current_file, show_hidden_files);
    closedir(d);
  }

  sort_files(head_file);
  read_files(head_file);

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
  strncpy(current->dir_name, ".", 255);

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
    check_list(head->dir_name, head, show_hidden_files);
    head = head->next_dir;
  }

  return (0);
}
