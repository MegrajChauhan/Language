#ifndef _QUEUE_
#define _QUEUE_

#include "report.h"
#include "utils.h"
#include "defs.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct queue queue;
typedef struct _queue_node _queue_node;

struct _queue_node {
    void *data;
    _queue_node *next;
};

struct queue {
    _queue_node *front;
    _queue_node *rear;
    size_t size;
    __cleanup_func data_clean_func; // Optional cleanup function for data
};

queue *queue_create(__cleanup_func data_clean_func);

bool queue_enqueue(queue *q, void *data);

void *queue_dequeue(queue *q);

void *queue_peek(queue *q);

bool queue_is_empty(queue *q);

bool queue_circulate_head(queue *q);

void queue_destroy(queue *q);

#endif