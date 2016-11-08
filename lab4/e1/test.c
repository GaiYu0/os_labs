#define _GNU_SOURCE
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

#define SHM "/random-process-shm:0"

int main() {
  int fd;
  fd = shm_open(SHM, O_RDWR | O_CREAT, 0600);
  if (fd == -1) {
    perror("shm_open");
    exit(1);
  }
  if (ftruncate(fd, sizeof(int)) == -1) {
    perror("ftruncate");
    exit(1);
  }
  int *p;
  p = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  pid_t pid;
  pid = fork();
  if (pid == 0) {
    *p = 1024;
  } else {
    int status;
    wait(&status);
    printf("%d\n", *p);
  }
  munmap(p, sizeof(int));
  shm_unlink(SHM);

  return 0;
}
