#include "sort_header.h"

void *selection_sort_time(filenode *list) {
  filenode *current = list;
  struct stat file_stats_one;
  struct stat file_stats_two;

  while (current != NULL) {
    stat(current->val, &file_stats_one);

    filenode *min_node = current;
    time_t min_time = file_stats_one.st_mtime;

    for (filenode *iter = current->next; iter != NULL; iter = iter->next) {
      stat(iter->val, &file_stats_two);

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
  return 0;
}