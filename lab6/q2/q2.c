#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

#define BUFFER_SIZE 256

// TODO

int main(int argc, char *argv[]) {
  if (argc != 3) {
    exit(1);
  }

  char *from, *to;
  int fd_from, fd_to;

  struct stat status;
  if (stat(argv[1], &status) == -1) {
    perror("stat");
    exit(1);
  }

  if ((status.st_mode & S_IFMT) != S_IFREG) {
    printf("source file is not a regular file\n");
    exit(1);
  }

  if ((fd_from = open(argv[1], O_RDONLY)) == -1) {
    perror("open source file");
    exit(1);
  }
  
  if (open(argv[2], O_CREAT | O_EXCL) == -1) {
    if (errno == EEXIST) {
      printf("existing destination file\n");
      exit(1);
    }
  }

  if ((fd_to = open(argv[2], O_CREAT | O_RDWR | O_TRUNC)) == -1) {
    perror("create destination file");
    exit(1);
  }

  char buffer[BUFFER_SIZE];
  int length = 1;
  while (length > 0) {
    length = read(fd_from, buffer, BUFFER_SIZE);
    write(fd_to, buffer, length);
  }

  close(fd_to);
  close(fd_from);

  return 0;
}
