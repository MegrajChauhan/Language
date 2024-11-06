#include "queue.h"

queue *queue_create(__cleanup_func data_clean_func)
{
    queue *q = (queue *)malloc(sizeof(queue));
    if (!q)
    {
        report_internal_error("Failed to create queue.");
        return NULL;
    }
    q->front = q->rear = NULL;
    q->size = 0;
    q->data_clean_func = data_clean_func;
    return q;
}

bool queue_enqueue(queue *q, void *data)
{
    check_source(q);

    _queue_node *new_node = (_queue_node *)malloc(sizeof(_queue_node));
    if (!new_node)
    {
        report_internal_error("Failed to allocate memory for new queue node.");
        return false;
    }
    new_node->data = data;
    new_node->next = NULL;

    if (q->rear)
        q->rear->next = new_node;
    else
        q->front = new_node;
    q->rear = new_node;
    q->size++;
    return true;
}

void *queue_dequeue(queue *q)
{
    check_source(q);

    if (!q->front)
        return NULL; // Queue is empty

    _queue_node *node = q->front;
    void *data = node->data;
    q->front = node->next;
    if (!q->front)
        q->rear = NULL;
    free(node);
    q->size--;
    return data;
}

void *queue_peek(queue *q)
{
    check_source(q);
    return q->front ? q->front->data : NULL;
}

bool queue_is_empty(queue *q)
{
    check_source(q);
    return q->size == 0;
}

bool queue_circulate_head(queue *q)
{
    // move the head to the next item in queue
    check_source(q);

    if (!q->front || !q->front->next)
        return true; // when using dequeue to get the item, NULL will tell that there is nothing left
    _queue_node *front = q->front;
    _queue_node *rear = q->rear;
    rear->next = front;
    q->front = front->next;
    front->next = NULL;
    return true;
}

void queue_destroy(queue *q)
{
    check_source(q);

    _queue_node *node = q->front;
    while (node)
    {
        _queue_node *next = node->next;
        if (q->data_clean_func)
        {
            q->data_clean_func(node->data);
        }
        free(node);
        node = next;
    }
    free(q);
}
