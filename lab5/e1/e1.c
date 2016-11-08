#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

void *thread_control(void*);

int main(int argc, char *argv[]) {
  int N;
  N = atoi(argv[1]);
  int *order;
  order = (int*)malloc(N * sizeof(int));
  pthread_t *tids;
  tids = (pthread_t*)malloc(N * sizeof(pthread_t));
  int i;
  for (i = 0; i != N; i++) {
    order[i] = i;
    if (pthread_create(tids + i, NULL, &thread_control, (void*)(order + i)) != 0) {
      perror("pthread_create");
      exit(0);
    }
  }
  for (i = 0; i != N; i++) {
    if (pthread_join(tids[i], NULL) != 0) {
      perror("pthread_join");
      exit(0);
    }
    printf("thread %d terminated order %d\n", (int)tids[i], *status);
  }
  return 0;
}

void *thread_control(void *order) {
  pthread_t self;
  self = pthread_self();
  int *p;
  p = (int*)order;
  printf("thread %d order %d\n", (int)self, *p);
  *p = *p * 2;
  return order;
}
