#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

double result;
pthread_mutex_t sresult = PTHREAD_MUTEX_INITIALIZER;

int N;

pthread_t *tids;
int *status;
pthread_mutex_t sstatus = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t send = PTHREAD_MUTEX_INITIALIZER;

void *thread_random(void*);
void *print_result(void *p);

int main(int argc, char *argv[]) {
  // print result
  pthread_mutex_lock(&send);
  pthread_t daemon_tid;
  if (pthread_create(&daemon_tid, NULL, &print_result, NULL) != 0) {
    perror("pthread_create");
    exit(0);
  }

  N = atoi(argv[1]);
  tids = (pthread_t*)calloc(N, sizeof(pthread_t));
  status = (int*)calloc(N, sizeof(int));

  int i;
  for (i = 0; i != N; i++) {
    if (pthread_create(tids + i, NULL, &thread_random, (void*)(status + i)) != 0) {
      perror("pthread_create");
      exit(0);
    }
  }

  if (pthread_join(daemon_tid, NULL) != 0) {
    perror("pthread_join");
    exit(0);
  }

  return 0;
}

void *print_result(void *p) {
  pthread_mutex_lock(&send);
  printf("result %f\n", result);
  return NULL;
}

int finished() {
  int i;
  int finished = 1;
  for (i = 0; i != N; i++) {
    if (!status[i]) {
      finished = 0;
      break;
    }
  }
  return finished;
}

void *thread_random(void *p) {
  pthread_detach(pthread_self());

  // generate value
  srand(getpid());
  double value;
  value = (double)rand() / RAND_MAX;

  // modify result
  pthread_mutex_lock(&sresult);
  result += value;
  pthread_mutex_unlock(&sresult);

  // modify status
  pthread_mutex_lock(&sstatus);
  *((int*)p) = 1;
  if (finished()) {
    pthread_mutex_unlock(&send);
  }
  pthread_mutex_unlock(&sstatus);
  return NULL;
}
