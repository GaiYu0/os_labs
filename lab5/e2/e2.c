#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

double result;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *thread_random(void*);

int main(int argc, char *argv[]) {
  int N;
  N = atoi(argv[1]);
  pthread_t *tids;
  tids = (pthread_t*)malloc(N * sizeof(pthread_t));
  int i;
  for (i = 0; i != N; i++) {
    if (pthread_create(tids + i, NULL, &thread_random, NULL) != 0) {
      perror("pthread_create");
      exit(0);
    }
  }
  for (i = 0; i != N; i++) {
    if (pthread_join(tids[i], NULL) != 0) {
      perror("pthread_join");
      exit(0);
    }
    printf("thread %d terminated\n", (int)tids[i]);
  }
  printf("result %f\n", result);
  return 0;
}

void *thread_random(void *p) {
  srand(getpid());
  double value;
  value = (double)rand() / RAND_MAX;
  pthread_mutex_lock(&mutex);
  result += value;
  pthread_mutex_unlock(&mutex);
  return NULL;
}
