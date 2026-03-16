#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "task_queue.h"

#define WORKER_COUNT 4

pthread_mutex_t lock;
pthread_cond_t cond;

void* worker_function(void* arg) {

    while (1) {

        pthread_mutex_lock(&lock);

        while (is_empty()) {
            pthread_cond_wait(&cond, &lock);
        }

        Task task = extract_max();

        pthread_mutex_unlock(&lock);

        printf("Worker Thread Executing Task ID: %d (Priority: %d)\n",
               task.id, task.priority);

        sleep(task.execution_time);

        printf("Finished Task ID: %d\n", task.id);
    }

    return NULL;
}

int main() {

    init_heap();

    pthread_t workers[WORKER_COUNT];

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    // Create worker threads
    for (int i = 0; i < WORKER_COUNT; i++) {
        pthread_create(&workers[i], NULL, worker_function, NULL);
    }

    // Simulate incoming tasks
    for (int i = 1; i <= 5; i++) {

        Task t;
        t.id = i;
        t.priority = rand() % 100;   // random priority
        t.execution_time = 2;

        pthread_mutex_lock(&lock);

        insert_task(t);
        printf("Added Task ID: %d Priority: %d\n", t.id, t.priority);

        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&lock);

        sleep(1);
    }

    // Keep main alive
    while (1);

    return 0;
}