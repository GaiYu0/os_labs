#define _GNU_SOURCE
#include<errno.h>
#include<fcntl.h>
#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

#define LIMIT 1E8
void calculate1() {
  printf("calculate1\n");
  int i;
  for (i = 0; i != LIMIT; i++);
}
void calculate2() {
  printf("calculate2\n");
  int i;
  for (i = 0; i != LIMIT; i++);
}

#define S1 "/sem-1"
#define S2 "/sem-2"

int main(int argc, char *argv[]) {
  int n;
  n = atoi(argv[1]) - 1;

  sem_t *s1, *s2;
  
  s1 = sem_open(S1, O_CREAT | O_RDWR, 0666, 0);
  s2 = sem_open(S2, O_CREAT | O_RDWR, 0666, 0);

  int i;
  for (i = 0; i != n; i++) {
    if (fork() == 0) {
      break;
    }
  }

  if (i == n) { // parent
    calculate1();
    for (i = 0; i != n; i++) {
      sem_wait(s1);
    }
    for (i = 0; i != n; i++) {
      sem_post(s2);
    }
    calculate2();
    
    int status;
    for (i = 0; i != n; i++) {
      wait(&status);
    }
  } else {
    calculate1();
    sem_post(s1);
    sem_wait(s2);
    calculate2();
  }

  sem_close(s1);
  sem_close(s2);
  sem_unlink(S1);
  sem_unlink(S2);

  return 0;
}
