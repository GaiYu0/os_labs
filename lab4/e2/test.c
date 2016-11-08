#define _GNU_SOURCE
#include<fcntl.h>
#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

#define SEM "/sem"

int main() {
  printf("%d %d\n", (int)sizeof(int), (int)sizeof(int*));
  return 0;
}
