#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main () {
  printf("PERE: %d\n", getpid());
  if (fork() != 0) {
    pause();
    pause();
    wait(0);
  } else {
    printf("FILS: %d\n", getpid());
    exit(0);
  }
  
  printf("%d> Fin\n", getpid());
  return 0;
}
