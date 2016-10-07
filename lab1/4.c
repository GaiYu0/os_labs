#define _GNU_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

#define EXIT_COMMAND "EXIT\n"
#define COMMAND_LENGTH 16

int main(int argc, char *argv[]){
  char *command;
  command = (char*)malloc(COMMAND_LENGTH);
  size_t length;
  length = 0;
  int code;
  while(1){
    code = getline(&command, &length, stdin);
    if(strcmp(command, EXIT_COMMAND) == 0) break;
    asprintf(&command, "%s%s", command, argv[0]);
    execlp("/bin/sh", "/bin/sh", "-c", command, NULL);
  }
  return 0;
}
