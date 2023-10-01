// Task sheduling using queues
#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
#define MAX 5


struct TASK
{
    int TaskId;
    char TaskTitle[30];
    int TaskDuration;
    int status; // 0 = not used , 1 = in queue , 2=completed , 
};

int rear = -1;
int front = -1;
int queue[MAX];

struct TASK t[10] = {{1,"task1",10,0} , {2,"task2",7,0}, {3,"task3",5,0}, {4,"task4",9,0},{5,"task5",8,0},
{6,"task6",4,0},{7,"task7",5,0},{8,"task8",3,0},{9,"task9",1,0},{10,"task10",2,0}};
void push(int item);
int isFull();
int run();
int display();
int schedule();
int main()
{
    int choice;
    while(1)
    {
        printf("1 schedule\n");
        printf("2 run\n");
        printf("3 display\n");
        printf("4 exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice)
        {
            case 1:
                schedule();
                break;
            case 2:
                run();
                break;
            case 3: 
                display();
                break;
            case 4:
                exit(1);
                break;
        }
    }
}
int schedule()
{
    //printf("scheduling.....\n");
    int id;
    int count = 0;
    if(isFull())
    {
        printf("Queue is full\n");
        for(int i = 0; i<10;i++)
        {
            if(queue[front] == t[i].TaskId)
                printf("Minimum time requred = %d secs\n",t[i].TaskDuration);
                break;
        }
        for(int i = front ;i <= rear; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if(queue[i] == t[j].TaskId)
                {
                    count = count + t[i].TaskDuration;
                }
            }
            
        }
        printf("Maximun time required = %d secs\n",count);
    }

    else
    {
        int p = 0;
        printf("Enter the ID of the task to be scheduled : ");
        scanf("%d", & id);
        for (int i = 0; i < 10; i++)
        {
            if(id == t[i].TaskId && t[i].status == 0 && !isFull())
            {
                t[i].status =1;
                push(id);
                p =1;
                break;
            }
        }
        if(p == 1)
            printf("Task has been added\n");
        else
            printf("Task cannot be added\n");
    }
}
int display()
{
    //printf("Displaying.....\n");
    // for(int i = front ; i<=rear ; i++)
    // {
    //printf("\n-> %d",queue[i]);
    // }
    for(int i = 0 ; i < 10 ; i++)
    {
        if(t[i].status == 1)
        {
            printf("\n-> %s\n",t[i].TaskTitle);
        }
    }
}
int run()
{
    //printf("Running.....\n");
    for (int i=0; i<=10;i++)
    {
        if(queue[front] == t[i].TaskId)
        {
            printf("........RUNNING TASK........\n");
            Sleep(t[i].TaskDuration);
            t[i].status = 2;
            front++;
            printf("........Task Completed..........\n");
            break;
        }
    }
}
void push(int item)
{
    //printf("Pushing item....\n");
    if(isFull())
    {
        printf("Overflow condition\n");
        return ;
    }
        
    if(front = -1)
    {
        front = 0;
    }
    rear++;
    queue[rear] = item;
}
int isFull()
{
    //printf("checking isFull()\n");
    if(rear == MAX-1)
        return 1;
    else 
        return 0;
}