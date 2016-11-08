#define _GNU_SOURCE
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

#define SHM "/random-process-shm:0"
int main(int argc, char *argv[]) {
  int n;
  n = atoi(argv[1]);
  
  int fd;
  fd = shm_open(SHM, O_RDWR | O_CREAT, 0600);
  if (fd == -1) {
    perror("shm_open");
    exit(1);
  }
  if (ftruncate(fd, n * sizeof(int)) == -1) {
    perror("ftruncate");
    exit(1);
  }

  int i;

  int *pointer;
  pointer = (int*)mmap(NULL, n * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (pointer == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }

  for (i = 0; i != n; i++) {
    if (fork() == 0) {
      break;
    }
  }
  
  if (i == n) { // parent
    int status;
    for (i = 0; i != n; i++)
      wait(&status);
  } else { // child

    // TODO random
    // *pointer = (int)(10 * (float)rand() / RAND_MAX);
    *(pointer + i) = i;
    munmap(pointer, n * sizeof(int));
    shm_unlink(SHM);
    exit(0);
  }

  int result = 0;
  int value = 0;
  for (i = 0; i != n; i++) {
    value = *(pointer++);
    printf("%d\n", value);
    result += value;
  }
  printf("result %d\n", result);

  munmap(pointer, n * sizeof(int));
  shm_unlink(SHM);
  return 0;
}
