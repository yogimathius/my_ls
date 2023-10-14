#include "sort_header.h"

void selection_sort_time(filenode *list) {
  filenode *current = list;
  struct stat file_stats_one;
  struct stat file_stats_two;
  while (current != NULL) {
    stat(current->val, &file_stats_one);

    filenode *min_node = current;
    struct timespec min_time = min_node->st_mtim;

    for (filenode *iter = current->next; iter != NULL; iter = iter->next) {
      stat(iter->val, &file_stats_two);

      struct timespec iter_time = file_stats_two.st_mtim;
      // if seconds are greater
      if (iter_time.tv_sec > min_time.tv_sec ||
          // if nanoseconds are greater
          (min_time.tv_sec == iter_time.tv_sec &&
           min_time.tv_nsec < iter_time.tv_nsec)) {
        min_node = iter;
        min_time = iter_time;
      } else if
          // if both are equal and strings are not equal
          (min_time.tv_nsec == iter_time.tv_sec &&
           min_time.tv_nsec == iter_time.tv_nsec) {
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