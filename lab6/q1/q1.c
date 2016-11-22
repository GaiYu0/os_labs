#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main() {
  int fd1, fd2, fd3;
  if ((fd1 = open("./file1", O_CREAT | O_RDWR | O_TRUNC, 0600)) == -1) {
    perror("open fd1");
    exit(1);
  }
  if (write(fd1, "ABCDE", strlen("ABCDE")) == -1) {
    perror("write fd1");
    exit(1);
  }
  if (fork() == 0) {
    if ((fd2 = open("./file1", O_RDWR)) == -1) {
      perror("open fd2");
      exit(1);
    }
    if (write(fd1, "123", strlen("123")) == -1) {
      perror("write fd2");
      exit(1);
    }
    if (write(fd2, "45", strlen("45")) == -1) {
      perror("write fd2");
      exit(1);
    }
    close(fd2);
  } else {
    fd3 = dup(fd1);
    if (lseek(fd3, 0, SEEK_SET) == -1) {
      perror("lseek fd3");
      exit(1);
    }
    if (write(fd3, "fg", strlen("fg")) == -1) {
      perror("write fd3");
      exit(1);
    }
    if (write(fd1, "hi", strlen("hi")) == -1) {
      perror("write fd1");
      exit(1);
    }
    wait(NULL);
    close(fd1);
    close(fd3);
  }
  return 0;
}
