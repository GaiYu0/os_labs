#include<signal.h>
#include<stdio.h>
#include<stdlib.h>

#define MAX_SIGINT 4

int global_count = 0;
int signal_count[NSIG];

void print_statistics() {
  printf("%d signals received in total.\n", global_count);
  printf("signal\tcount\n");
  int i;
  for (i = 0; i != NSIG; i++) {
    printf("%d\t%d\n", i, signal_count[i]);
  }
}

void handler(int signal) {
  global_count++;
  signal_count[signal]++;
  if (signal_count[SIGINT] == MAX_SIGINT) {
    print_statistics();
    exit(0);
  }
}

int main() {
  sigset_t mask;
  sigfillset(&mask);
  struct sigaction action;
  action.sa_flags = 0;
  action.sa_mask = mask;
  int i;
  for (i = 0; i != NSIG; i++) {
    action.sa_handler = &handler;
    sigaction(i, &action, NULL);
  }
  while(1) {
    pause();
  }
  return 0;
}
