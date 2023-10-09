#include "sort_header.h"

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