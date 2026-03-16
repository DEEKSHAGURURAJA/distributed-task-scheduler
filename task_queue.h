#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#define MAX_TASKS 100

typedef struct {
    int id;
    int priority;
    int execution_time;
} Task;

void init_heap();
void insert_task(Task task);
Task extract_max();
int is_empty();

#endif