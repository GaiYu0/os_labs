#define _GNU_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>

#define COMMAND_START "gcc -o program"

int main(int argc, char* argv[]){
  int i;
  pid_t pid;
  for(i = 0; i != argc - 1; ++i){
    pid = fork();
    if(pid == 0){
      char *command;
      asprintf(&command, "gcc -c -o %s.o %s.c", argv[i + 1], argv[i + 1]);
      exit(system(command));
    }else if(pid == -1){
      printf("cannot create process\n");
      return -1;
    }
  }
  int status;
  for(i = 0; i != argc - 1; ++i){
    wait(&status);
    if(WEXITSTATUS(status) != 0){
      printf("compilation error\n");
      return -1;
    }
  }
  char *buffer;
  buffer = (char*)malloc(512);
  strcpy(buffer, COMMAND_START);
  char *start;
  for(i = 0, start = buffer + sizeof(COMMAND_START) - 1; i != argc - 1; ++i){
    sprintf(start, " %s.o", argv[i + 1]);
    start += strlen(argv[i + 1]) + 3;
  }
  int code;
  code = system(buffer);
  printf("%s\n", buffer);
  free(buffer);
  printf("gcc exit code %d\n", code);
  return system("./program");
}
