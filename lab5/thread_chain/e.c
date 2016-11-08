#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

pthread_t *tids;
void *create(void*);

int main(int argc, char *argv[]) {
  int N;
  N = atoi(argv[1]);
  int count;
  count = N;
  tids = (pthread_t*)calloc(N, sizeof(pthread_t));
  pthread_t tid;
  pthread_create(&tid, NULL, &create, (void*)(&count));
  if (pthread_join(tid, NULL) != 0) {
    perror("pthread_join");
    exit(0);
  }
  int i;
  for (i = N - 1; i != -1; i--) {
    if (pthread_join(tids[i], NULL) != 0) {
      perror("pthread_join");
      exit(0);
    }
  }
  return 0;
}

void *create(void *p) {
  int *n;
  n = (int*)p;
  if (*n == 0) {
    return NULL;
  }
  *n -= 1;
  if (pthread_create(tids + *n, NULL, &create, p) != 0) {
    perror("pthread_create");
    exit(0);
  } else {
    printf("created\n");
  }
  return NULL;
}
