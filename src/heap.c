#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "coroutine_int.h"

#define hp_parent(n) ((n - 1) / 2)
#define hp_left(n) (2 * n + 1)
#define hp_right(n) (2 * n + 2)

static inline void swap(struct hp_node **a, struct hp_node **b){
    struct hp_node *tmp = *a;
    *a = *b;
    *b = tmp;
}

void heap_init(struct heap *heap){
    srand(time(NULL));
    heap->size = 0;
    for(int i = 0; i < RINGBUFFER_SIZE; i++){
        heap->node[i] = NULL;
    }
}

int heap_insert(struct heap *heap, struct task_struct *task, int pri){
    if(!heap || !task) return -EAGAIN;
    if(heap->size == RINGBUFFER_SIZE) return -EAGAIN;
    heap->node[heap->size] = (struct hp_node *)calloc(1, sizeof(struct hp_node));
    heap->node[heap->size]->pri = pri;
    // printf("task->tfd = %d\n", task->tfd);
    // heap->node[heap->size]->pri = task->tfd;
    heap->node[heap->size]->task = task;
    int idx = heap->size++;
    struct hp_node *tmp = NULL;
    while(heap->node[hp_parent(idx)]->pri < heap->node[idx]->pri){
        tmp = heap->node[hp_parent(idx)];
        heap->node[hp_parent(idx)] = heap->node[idx];
        heap->node[idx] = tmp;
        idx = hp_parent(idx);
    }
    return 0;
}

struct task_struct *heap_delete(struct heap *heap){
    if(!heap) return NULL;
    if(heap->size == 0) return NULL;
    struct task_struct *task = heap->node[0]->task;
    int pri = heap->node[0]->pri, idx = 0;
    free(heap->node[0]);
    heap->node[0] = heap->node[--heap->size];
    heap->node[heap->size] = NULL;
    while(1){
        int tmp = idx;
        if(hp_left(tmp) < heap->size && heap->node[hp_left(tmp)]->pri > heap->node[idx]->pri){
            idx = hp_left(tmp);
        }
        if(hp_right(tmp) < heap->size && heap->node[hp_right(tmp)]->pri > heap->node[idx]->pri){
            idx = hp_right(tmp);
        }
        if(tmp != idx) swap(&heap->node[tmp], &heap->node[idx]);
        else break;
    }
    return task;
}
