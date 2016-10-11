#define _GNU_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>

#define EXIT_COMMAND "EXIT\n"
#define BUFFER_SIZE 3

char *command;

void execute(int signal) {
  execlp("/bin/sh", "/bin/sh", "-c", command, NULL);
  exit(0);
}

int main(int argc, char *argv[]){
  // set handler
  struct sigaction action;
  action.sa_flags = 0;
  action.sa_handler = &execute;
  sigaction(SIGINT, &action, NULL);

  size_t length = 0;
  int code;
  pid_t pids[BUFFER_SIZE];
  while(1){
    int command_count;
    for (command_count = 0; command_count != BUFFER_SIZE; command_count++) {
      code = getline(&command, &length, stdin);
      if(strcmp(command, EXIT_COMMAND) == 0) {
        int i;
        for (i = 0; i != command_count; i++) {
          kill(pids[i], SIGQUIT);
          free(command);
        }
        return 0;
      }
      pids[command_count] = fork();
      if (pids[command_count] == -1) {
        printf("Fail to fork.\n");
        return -1;
      }
      if (pids[command_count] == 0) {
        break;
      } else {
        // free(command);
      }
    }
    if (command_count != BUFFER_SIZE) {
      pause();
    } else {
      for (command_count = 0; command_count != BUFFER_SIZE; command_count++) {
        kill(pids[command_count], SIGINT);
      }
      int status = 0;
      for (command_count = 0; command_count != BUFFER_SIZE; command_count++) {
        wait(&status);
        if (WEXITSTATUS(status) != 0) {
          printf("Error occurred.\n");
          return -1;
        }
      }
    }
  }
  return 0;
}
