#include <stdio.h>
#include "task_queue.h"

Task heap[MAX_TASKS];
int size = 0;

void swap(Task *a, Task *b)
{
    Task temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (heap[parent].priority < heap[index].priority)
        {
            swap(&heap[parent], &heap[index]);
            index = parent;
        }
        else
            break;
    }
}

void heapify_down(int index)
{
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && heap[left].priority > heap[largest].priority)
        largest = left;

    if (right < size && heap[right].priority > heap[largest].priority)
        largest = right;

    if (largest != index)
    {
        swap(&heap[index], &heap[largest]);
        heapify_down(largest);
    }
}

void init_heap()
{
    size = 0;
}

void insert_task(Task task)
{
    heap[size] = task;
    heapify_up(size);
    size++;
}

Task extract_max()
{
    Task max = heap[0];
    heap[0] = heap[size - 1];
    size--;

    heapify_down(0);

    return max;
}

int is_empty()
{
    return size == 0;
}