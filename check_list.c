#include "sort_header.h"

void read_files(filenode *list) {
  while (list != NULL) {
    if (strlen(list->val) > 0) {
      printf("%s\n", list->val);
    }
    list = list->next;
  }
}

void add_to_list(struct dirent *dir, DIR *d, filenode *next_file,
                 int show_hidden) {
  while ((dir = readdir(d)) != NULL) {
    if (show_hidden || (dir->d_name[0] != '.' && dir->d_name[0] != ' ')) {
      struct stat file_stats_one;
      stat(dir->d_name, &file_stats_one);
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
    first_dotfile = temp;
    next = temp->next;
    temp = next;
  }
  while (temp->next != NULL) {
    if (strcmp(temp->val, ".") != 0) {

      next = temp->next;
      temp->next = prev;
      prev = temp;
      temp = next;
    } 
  }
  if (show_dotfiles) {
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
  struct dirent *dir = malloc(sizeof(struct dirent));
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
      head_file = reverse_linked_list(head_file);
    }
  } else {
    sort_files(head_file);
  }
  read_files(head_file);
  return NULL;
}