#include <stdio.h>
#include <stdlib.h>
#include "../src/coroutine_int.h"

int main(){
    struct task_struct task[10];
    struct heap heap;
    struct task_struct *tmp;

    heap_init(&heap);
    for(int i = 0; i < 10; i++){
        task[i].tfd = i;
        printf("insert %d, return %d\n", i, heap_insert(&heap, task + i));
    }

    for(int i = 0; i < 10; i++){
        tmp = heap_delete(&heap);
        if(tmp) printf("delete %d\n", tmp->tfd);
        else printf("delete failed\n");
    }
    return 0;
}