#include "sort_header.h"
#include <dirent.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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
    check_list(head, show_hidden, sort_time);
    head = head->next;
  }

  return (0);
}