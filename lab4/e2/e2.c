#define _GNU_SOURCE
#include<fcntl.h>
#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

#define N 8

#define SHM "/shm:0"
#define PRODUCER "/sem-producer"
#define CONSUMER "/sem-consumer"
#define QUEUE "/sem-queue"

typedef struct queue_t {
  int array[N];
  int head;
  int length;
} queue_t;

int initialize(queue_t* queue) {
  int i;
  for (i = 0; i != N; i++) {
    queue->array[i] = 0;
  }
  queue->head = 0;
  queue->length = 0;
  return 0;
}

int enqueue(queue_t *queue, int value) {
  int offset;
  offset = (queue->head + queue->length) % N;
  queue->array[offset] = value;
  (queue->length)++;
  return 0;
}

int dequeue(queue_t *queue) {
  int value;
  value = queue->array[queue->head];
  queue->head = (queue->head + 1) % N;
  (queue->length)--;
  return value;
}

sem_t *producer, *consumer, *queue;
int *p, **head, *length;

void cleanup(int signal) {
  sem_close(producer);
  sem_close(consumer);
  sem_close(queue);
  sem_unlink(PRODUCER);
  sem_unlink(CONSUMER);
  sem_unlink(QUEUE);

  munmap(p, N * sizeof(int));
  munmap(head, sizeof(int*));
  munmap(length, sizeof(int));
  shm_unlink(SHM);
}

int main(int argc, char *argv[]) {
  struct sigaction action;
  action.sa_handler = &cleanup;
  sigaction(SIGINT, &action, NULL);

  int n;
  n = atoi(argv[1]);
  
  producer = sem_open(PRODUCER, O_CREAT | O_RDWR, 0666, N);
  consumer = sem_open(CONSUMER, O_CREAT | O_RDWR, 0666, 0);
  queue = sem_open(QUEUE, O_CREAT | O_RDWR, 0666, 1);

  int fd;
  fd = shm_open(SHM, O_RDWR | O_CREAT, 0600);
  ftruncate(fd, sizeof(queue_t));

  queue_t *q;
  q = (queue_t*)mmap(NULL, sizeof(queue_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  initialize(q);
  
  int i;
  for (i = 0; i != n; i++) {
    if (fork() == 0) {
      break;
    }
  }
  
  // parent
  if (i == n) {
    int result = 0;
    int value = 0;
    for (i = 0; i != n * n; i++) {
      sem_wait(consumer);
      sem_wait(queue);
      value = dequeue(q);
      sem_post(queue);
      sem_post(producer);
      result += value;
    }

    printf("result %d\n", result);

    int status;
    for (i = 0; i != n; i++) {
      wait(&status);
    }
  // child
  } else {
    for (i = 0; i != n; i++) {
      // TODO random
      // *p = (int)(10 * (float)rand() / RAND_MAX);
      sem_wait(producer);
      sem_wait(queue);
      enqueue(q, i);
      sem_post(queue);
      sem_post(consumer);
    }
  }

  cleanup(SIGINT);

  return 0;
}
