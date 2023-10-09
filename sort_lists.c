#include "sort_header.h"

filenode *sort_files(filenode *list) {
  int swapped;

  do {
    swapped = 0;
    filenode *current = list;
    filenode *next = list->next;

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

dirnode *sort_dirs(dirnode *list) {
  int swapped;

  do {
    swapped = 0;
    dirnode *current = list;
    dirnode *next = list->next;

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