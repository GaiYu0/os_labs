#define _XOPEN_SOURCE 700

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main () {
  pause ();
  printf ("1er signal\n");
  pause ();
  printf ("2e signal\n");
  return 0;
}

/*
struct sigaction action;

void handler(int signal) {
  printf("\nterminated\n");
  action.sa_handler = SIG_IGN;
  sigaction(SIGINT, &action, NULL);
}

int main() {
  // signal set
  sigset_t signal_set;
  sigemptyset(&signal_set);
  // action
  action.sa_flags = 0;
  action.sa_handler = &handler;
  action.sa_mask = signal_set;
  // register
  sigaction(SIGINT, &action, NULL);
  //pause
  pause();
  return 0;
}
*/
