#include<dirent.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef TIMES
#define TIMES 16
#endif

int buffer_length;
char *buffer;
int traverse(char*, char*);
int search_file(int, char*);

int main(int argc, char *argv[]) {
  buffer_length = strlen(argv[1]) * TIMES;
  buffer = (char*)malloc(buffer_length + 1);
  traverse(argv[2], argv[1]);
  return 0;
}

int child_directory(char *directory) {
  return (strcmp(directory, ".") != 0) && (strcmp(directory, "..") != 0);
}

int traverse(char *path, char *target) {
  int fd, directory_fd;
  DIR *directory;
  if ((directory = opendir(path)) == NULL) {
    perror("opendir");
    exit(0);
  }
  directory_fd = dirfd(directory);
  struct dirent *entry;
  while ((entry = readdir(directory)) != NULL) {
    switch (entry->d_type) {
    case DT_DIR:
      if (child_directory(entry->d_name)) {
        traverse(entry->d_name, target);
      }
      break;
    case DT_REG:
      if ((fd = openat(directory_fd, entry->d_name, O_RDONLY)) == -1) {
        perror("openat");
        exit(0);
      }
      if (search_file(fd, target)) {
        printf("%s/%s\n", path, entry->d_name);
        close(fd);
      }
      close(fd);
      break;
    }
  }
  return 0;
}

int search_file(int fd, char *target) { // TODO
  int target_length;
  target_length = strlen(target);
  int length = 0;
  char *front;
  front = buffer;
  while(1) {
    if ((length = read(fd, front, buffer_length + buffer - front)) == -1) {
      perror("read");
      exit(0);
    }
    if (length == 0) {
      break;
    }
    front[length] = '\0';
    if (length + front - buffer < target_length) {
      front += length;
    } else {
      if (strstr(buffer, target) != NULL) {
        return 1;
      }
      int i, total, offset;
      total = front - buffer + length;
      offset = total - target_length + 1;
      for (i = 0; i != target_length - 1; i++) {
        buffer[i] = buffer[offset + i];
      }
      front = buffer + target_length - 1;
    }
  }
  return 0;
}
