#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define FILE_NAME "cache"

int main(int argc, char *argv[]) {
  int fd;
  if ((fd = open(FILE_NAME, O_APPEND | O_CREAT | O_RDWR | O_TRUNC, 0600)) == -1) {
    perror("create cache");
    exit(1);
  }
  int N;
  N = atoi(argv[1]);
  int i;
  for (i = 0; i != N; i++) {
    if (fork() == 0) {
      break;
    }
  }
  if (i == N) {
    for (i = 0; i != N; i++) {
      wait(NULL);
    }
    double value, total;
    lseek(fd, 0, SEEK_SET);
    for (i = 0; i != N; i++) {
      if ((read(fd, &value, sizeof(double)) != sizeof(double)) && (i != N - 1)) {
        printf("%d\n", i);
        perror("read");
        exit(1);
      }
      total += value;
    }
    printf("total %f\n", total);
  } else {
    double value;
    srand(getpid());
    value = ((double)rand()) / RAND_MAX;
    printf("%f\n", value);
    int local_fd;
    local_fd = dup(fd);
    write(local_fd, &value, sizeof(double));
    close(local_fd);
  }
  return 0;
}
