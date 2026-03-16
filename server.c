#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "task_queue.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define WORKERS 4

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* worker_thread(void* arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock);

        while (is_empty())
        {
            pthread_cond_wait(&cond, &lock);
        }

        Task task = extract_max();

        pthread_mutex_unlock(&lock);

        printf("Worker executing Task ID: %d Priority: %d\n",
               task.id, task.priority);

        sleep(task.execution_time);

        printf("Finished Task ID: %d\n", task.id);
    }
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;

    init_heap();

    pthread_t workers[WORKERS];

    for (int i = 0; i < WORKERS; i++)
    {
        pthread_create(&workers[i], NULL, worker_thread, NULL);
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 3);

    printf("Server started on port %d...\n", PORT);

    while (1)
    {
        new_socket = accept(server_fd, NULL, NULL);

        char buffer[BUFFER_SIZE] = {0};

        read(new_socket, buffer, BUFFER_SIZE);

        Task task;

        sscanf(buffer, "%d|%d|%d",
               &task.id,
               &task.priority,
               &task.execution_time);

        pthread_mutex_lock(&lock);

        insert_task(task);

        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&lock);

        printf("Received Task %d with Priority %d\n",
               task.id, task.priority);

        close(new_socket);
    }

    return 0;
}