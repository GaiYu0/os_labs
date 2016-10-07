#include<stdio.h>
#include<unistd.h>

#if RECURSION
int recursive(int n){
  if(!n) return 0;
  else{
    int pid;
    pid = fork();
    if(pid == 0){
      printf("child created\n");
      return 0;
    }else if(pid == -1){
      printf("cannot create process\n");
      return -1;
    }else
      return recursive(n - 1);
  }
}
#endif

int main(){
#if LOOP
  int i;
  int pid;
  for(i = 0; i != N_CHILDREN; ++i){
    pid = fork();
    if(pid == 0){
      printf("child %d created\n", i);
      break;
    }else if(pid == -1){
      printf("cannot create process\n");
      break;
    }
  }
#elif RECURSION
  recursive(N_CHILDREN);
#endif
  return 0;
}
