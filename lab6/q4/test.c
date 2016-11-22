#include<dirent.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

#define BUFFER_SIZE 256

int main() {
  int fd, directory_fd;
  DIR *directory;
  directory = opendir(".");
  directory_fd = dirfd(directory);
  struct dirent *entry;
  char buffer[BUFFER_SIZE];
  int length = 1;
  while ((entry = readdir(directory)) != NULL) {
    if (entry->d_type != DT_REG) {
      continue;
    }
    fd = openat(directory_fd, entry->d_name, O_RDONLY);
    while (length != 0) {
      printf("%d\n", length);
      if ((length = read(fd, buffer, BUFFER_SIZE * sizeof(char))) == -1) {
        perror("read");
        printf("%s\n", entry->d_name);
        exit(1);
      }
      write(STDOUT_FILENO, buffer, length * sizeof(char));
    }
  }
  return 0;
}
