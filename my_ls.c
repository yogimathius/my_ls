#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <inttypes.h>

int main(void) {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
          struct stat buf;
          stat(dir->d_name, &buf);

          printf("======found directories======\n");
          printf("name: %s\n", dir->d_name);
          printf("Number of links: %hu\n", buf.st_nlink);
          printf("ino: %llu\n", dir->d_ino);
          printf("namlen: %hu\n", dir->d_namlen);
          printf("reclen: %hu\n", dir->d_reclen);
          printf("type: %hhu\n", dir->d_type);

        }
        closedir(d);
    }
    return(0);
}
