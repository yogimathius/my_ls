#include <dirent.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>

#ifndef STRUCT_FILENODE
#define STRUCT_FILENODE
typedef struct s_filenode {
  char val[256];
  struct timespec st_mtim;
  struct s_filenode *next;
  struct s_filenode *next_dir;
} filenode;
#endif

#ifndef STRUCT_DIRNODE
#define STRUCT_DIRNODE
typedef struct s_dirnode {
  char val[256];
  struct s_dirnode *next;
  struct s_filenode *next_file;
} dirnode;
#endif

void read_files(filenode *list, int sort_time) {
  while (list != NULL) {
    if (strlen(list->val) > 0) {
    //   printf("%s\n", list->val);
    //   if (sort_time) {
    //   printf("%s\n", list->val);

    //     printf("%lu\n", list->st_mtim.tv_nsec);
    //     printf("%lu\n", list->st_mtim.tv_sec);
    //   } else {
                printf("%s\n", list->val);

    //   }
    }
    list = list->next;
  }
}

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

int selection_sort_time(filenode *list) {
  filenode *current = list;
  struct stat file_stats_one;
  struct stat file_stats_two;
  int was_altered = 0;
  while (current != NULL) {
    int result_one = stat(current->val, &file_stats_one);

    filenode *min_node = current;
    struct timespec min_time = min_node->st_mtim;

    for (filenode *iter = current->next; iter != NULL; iter = iter->next) {
      int iter_result = stat(iter->val, &file_stats_two);

      struct timespec iter_time = file_stats_two.st_mtim;
      // if seconds are greater
      if (iter_time.tv_sec > min_time.tv_sec ||
          // if nanoseconds are greater
          (min_time.tv_sec == iter_time.tv_sec &&
           min_time.tv_nsec < iter_time.tv_nsec)) {
            //    printf("nanoseconds greater\n");
        min_node = iter;
        min_time = iter_time;
        was_altered = 1;
      } else if
          // if both are equal and strings are not equal
          (min_time.tv_nsec == iter_time.tv_sec &&
           min_time.tv_nsec == iter_time.tv_nsec) {
        // printf("times are equal\n");
        min_node = iter;
        min_time = iter_time;
        was_altered = 1;
      } else {
        //   printf("doesn't check out\n");
      }
      // If time is equal, and
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

  return was_altered;
}

void add_to_list(struct dirent *dir, DIR *d, filenode *next_file,
                 int show_hidden) {
  while ((dir = readdir(d)) != NULL) {
    if (show_hidden || (dir->d_name[0] != '.' && dir->d_name[0] != ' ')) {
      struct stat file_stats_one;
      int dir_stats = stat(dir->d_name, &file_stats_one);
      struct timespec iter_time = file_stats_one.st_mtim;

      strncpy(next_file->val, dir->d_name, 255);
      next_file->st_mtim = iter_time;

      next_file->next = (filenode *)malloc(sizeof(filenode));
      next_file = next_file->next;
      next_file->val[0] = '\0';
      next_file->next = NULL;
    }
  }
}

int is_batch_created(filenode *list) {
  filenode *current = list;

  int is_batch_created = 1;
  for (filenode *iter = current->next; iter != NULL; iter = iter->next) {
    if (strcmp(current->val, ".") != 0 && strcmp(current->val, "..") &&
        strcmp(iter->val, ".") && strcmp(iter->val, "..")) {
      if (current->st_mtim.tv_nsec != iter->st_mtim.tv_nsec ||
          current->st_mtim.tv_sec != iter->st_mtim.tv_sec) {
        is_batch_created = 0;
      }
    }
  }
  return is_batch_created;
}

filenode *reverse_linked_list(filenode *head) {
  filenode *temp = head;
  filenode *prev = NULL;
  filenode *next = NULL;
  filenode *first_dotfile = NULL;
  int show_dotfiles = 0;
    if (strcmp(temp->val, ".") == 0) {
        show_dotfiles = 1;
            //   prev = temp;
        first_dotfile = temp;
        // temp = temp->next;
        next = temp->next;
    //   prev = temp;
      temp = next;
    }
  while (temp->next != NULL) {
 if (strcmp(temp->val, ".") != 0) {

      next = temp->next;
      temp->next = prev;
      prev = temp;
      temp = next;
 }    //   printf("checks out %s\n", temp->val);
  }
  if (show_dotfiles) {
    //   printf("past while\n");
    first_dotfile->next = prev;
    head = first_dotfile;
  } else {
    head = prev;
  }
  return head;
}

void *check_list(dirnode *curr_dir, int show_hidden, int sort_time) {
  char *dir_to_check = curr_dir->val;
  DIR *d;
  struct dirent *dir;
  d = opendir(dir_to_check);
  strncpy(curr_dir->val, dir_to_check, 255);
  curr_dir->next_file = (filenode *)malloc(sizeof(filenode));
  filenode *head_file = curr_dir->next_file;
  filenode *next_file = head_file;

  if (d) {
    add_to_list(dir, d, next_file, show_hidden);
    closedir(d);
  }

  if (sort_time) {
    filenode *altered;
    altered = head_file;
    selection_sort_time(altered);
    if (is_batch_created(altered)) {
    //   printf("is batch created!!!!\n");
    //   head_file = sort_files(altered);
      head_file = reverse_linked_list(head_file);
    }
  } else {
    sort_files(head_file);
  }
  read_files(head_file, sort_time);
}

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

// Can you run ./my_ls -t and it prints the content of the current directory
// (sorted by time)?

// Can you run ./my_ls -ta and it prints the content of the marvel directory
// (sorted by time) + hidden files?

// Can you run ./my_ls -t -a and it prints the content of the marvel directory
// (sorted by time sec + nsec + alphanum ) + hidden files?