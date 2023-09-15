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
  while (list->next != NULL) { // changed from list != NULL
    printf("%s\n", list->val);
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

int main(void) {
  DIR *d;
  struct dirent *dir;
  d = opendir(".");

  listnode *head;
  listnode *current;
  head = (listnode *)malloc(sizeof(listnode));
  current = head;

  if (d) {
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_name[0] != '.' && dir->d_name[0] != ' ') {
        strncpy(current->val, dir->d_name, 255); // copy up to 255 characters
        current->val[255] = '\0'; // ensure null termination
        current->next = (listnode *)malloc(sizeof(listnode));
        current = current->next;
        }
    }
    current = NULL; // terminate the list
    closedir(d);
  }
  sort_lists(head);
  read_list(head);
  return (0);
}
