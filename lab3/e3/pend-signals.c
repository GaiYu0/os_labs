#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler_alrm(int signal) {
  printf("Alarm\n");
}

void handler_int(int signal) {
  printf("Interruption\n");
}

void handler_quit(int signal) {
  printf("Quit\n");
}

int main() {
  sigset_t signal_set;
  sigemptyset(&signal_set);
  sigaddset(&signal_set, SIGINT);
  sigaddset(&signal_set, SIGQUIT);
  // SIG_ALRM
  struct sigaction action_alrm;
  action_alrm.sa_flags = 0;
  action_alrm.sa_handler = &handler_alrm;
  action_alrm.sa_mask = signal_set;
  sigaction(SIGALRM, &action_alrm, NULL);
  // SIG_INT
  struct sigaction action_int;
  action_int.sa_flags = 0;
  action_int.sa_handler = &handler_int;
  action_int.sa_mask = signal_set;
  sigaction(SIGINT, &action_int, NULL);
  // SIG_QUIT
  struct sigaction action_quit;
  action_quit.sa_flags = 0;
  action_quit.sa_handler = &handler_quit;
  action_quit.sa_mask = signal_set;
  sigaction(SIGQUIT, &action_quit, NULL);

  alarm(5);
  sigsuspend(&signal_set);

  return 0;
}
