#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#define MAX 5
struct TASK
{
    int TaskId;
    char TaskTitle[30];
    int TaskDuration;
    int status; // status 1:Queued ,2:completed, 3:Idle
};
int rear = -1;
int front = -1;
int queue_arr[MAX];

void push(int item)
{
    if (isfull())
    {
        printf("Queue Overflow condition\n");
        return;
    }
    if (front == -1)
    {
        front = 0;
    }
    rear = rear + 1;
    queue_arr[rear] = item;
}
int isfull()
{
    if (rear == MAX - 1)
        return 1;
    else
        return 0;
}
struct TASK t[10] = {{1, "task1", 10, 0}, {2, "task2", 5, 0}, {3, "task3", 6, 0}, {4, "task4", 7, 0}, {5, "task5", 11, 0}, {6, "task6", 20, 0}, {7, "task7", 5, 0}, {8, "task8", 5, 0}, {9, "task9", 5, 0}, {10, "task10", 5, 0}};
void run()
{
    for (int i = 0; i < 10; i++)
    {
        if (queue_arr[front] == t[i].TaskId)
        {
            printf("\n........running.......");
            sleep(t[i].TaskDuration);
            t[i].status = 2;
            front = front + 1;
            printf("\nTask has been completed");
            break;
        }
    }
}
void display()
{
    int count = 0;
    for (int i = front; i <= rear; i++)
    {
        count++;
    }
    printf("\nTotal Queued tasks are: %d", count);
    for (int i = 0; i < 10; i++)
    {
        if (t[i].status == 1)
            printf("\n o %s", t[i].TaskTitle);
    }
}
void schedule()
{
    int id;
    int count = 0;
    if (isfull())
    {
        printf("\nQueue is Full");
        for (int i = 0; i < 10; i++)
        {
            if (queue_arr[front] == t[i].TaskId)
            {
                printf("\nMinimum time required is : %dsec", t[i].TaskDuration);
                break;
            }
        }
        for (int i = front; i <= rear; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if (queue_arr[i] == t[j].TaskId)
                {
                    count = count + t[j].TaskDuration;
                }
            }
        }
        printf("\nMax time required is : %dsec", count);
    }
    else
    {
        int p = 0;
        printf("\nEnter the id of the task to be scheduled : ");
        scanf("%d", &id);
        for (int i = 0; i < 10; i++)
        {
            if (id == t[i].TaskId && t[i].status == 0 && !isfull())
            {
                t[i].status = 1;
                push(id);
                p = 1;
                break;
            }
        }
        if (p == 1)
            printf("\nTask has been added");
        else
            printf("\nTask cannot be added");
    }
}

int main()
{
    int choice;
    int id;
    while (1)
    {
        printf("\nEnter the choice : \n");
        printf("\n1.schedule a task");
        printf("\n2.run the task");
        printf("\n3.Display the queued up details of the tasks :");
        printf("\n4.Exit\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            schedule();
            break;
        case 3:
            display();
            break;
        case 2:
            run();
            break;
        case 4:
            exit(1);
            break;
        }
    }
    return 0;
}