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

#ifndef STRUCT_LISTNODE_ARRAY
#define STRUCT_LISTNODE_ARRAY
typedef struct s_listnode_array {
  int size;
  listnode **array;
} listnode_array;
#endif

void read_list(listnode *list) {
  while (list != NULL) { // changed from list != NULL
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

void add_to_list(char *arg, listnode *current_list) {
    DIR *d;
    struct dirent *dir;
    d = opendir(arg);

    if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (dir->d_name[0] != '.' && dir->d_name[0] != ' ') {
        strncpy(current_list->val, dir->d_name, 255); 
        current_list->val[255] = '\0';                
        current_list->next = (listnode *)malloc(sizeof(listnode));
        current_list = current_list->next;
      }
    }
    current_list = NULL; // terminate the list
    closedir(d);
  }
}

int main(int argc, char *argv[]) {
  int current_dir = 0;
  int curr_with_files = 0;
  int lookup_dirs = 0;
  int time_sorted = 0;
  int show_hidden_files = 0;

  listnode *head;
  listnode *current;
  head = (listnode *)malloc(sizeof(listnode));
  current = head;
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-f") == 0) {
      }
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
        add_to_list(argv[i], current);
      }
    }
  } else {
    add_to_list(".", current);
  }
  sort_lists(head);
  read_list(head);
  return (0);
}
