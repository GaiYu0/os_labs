#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
  int i;
  int pids[N_CHILDREN];
  for(i = 0; i != N_CHILDREN; ++i){
    pids[i] = fork();
    if(pids[i] == 0){
      printf("child %d created\n", i);
      return 0;
    }else if(pids[i] == -1){
      printf("cannot create process\n");
      return -1;
    }
  }
  int status;
  pid_t pid;
  for(i = 0, status = 0; i != N_CHILDREN; ++i){
    pid = wait(&status);
    if(i == 0) printf("one of the children terminated\n");
    if(pid == pids[N_CHILDREN - 1]) printf("the last created child terminated\n");
  }
  printf("all children terminated\n");
  return 0;
}
