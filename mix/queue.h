#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>

typedef uint32_t u32;
typedef struct Queue queue_t;

queue_t *createQueue();

int destroyQueue(queue_t *);

// retorna valor de la cabeza, o -1 si esta vacia.
int head(queue_t *);
bool isEmpty(queue_t *);


// la cola tiene que ser distinto de NULL.
void enqueue(u32, queue_t *);

// retorna -1 si la cola esta vacia.
int dequeue(queue_t *);

int getQueueSize(queue_t *);
