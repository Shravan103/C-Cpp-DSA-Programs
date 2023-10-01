// queue using arrays
#include <stdio.h>
#include<stdlib.h>
#define N 5
int queue[N];
int front = -1;
int rear = -1;
int enqueue(int ele)
{
    if (rear == N - 1)
    {
        printf("Overflow condition!!\n");
    }
    else if (front == -1 && rear == -1)
    {
        front = rear = 0;
        queue[rear] = ele;
    }
    else
    {
        rear++;
        queue[rear] = ele;
    }
}
int dequeue()
{
    if (front == -1 && rear == -1)
    {
        printf("Underflow condition!!\n");
    }
    else if (front == rear)
    {
        front = rear = -1;
    }
    else
    {
        printf("dequeued element is %d", queue[front]);
        front++;
    }
}
int peek()
{
    if (front == -1 && rear == -1)
    {
        printf("Underflow condition!!\n");
    }
    else
    {
        printf("Front element is %d", queue[front]);
    }
}
int display()
{
    if (front == -1 && rear == -1)
    {
        printf("Underflow condition!!\n");
    }
    else
    {
        for (int i = front; i <= rear; i++)
        {
            printf("%d  ", queue[i]);
        }
    }
}
int main()
{
    int choice, ele;
    while (1)
    {
        printf("\n1 enqueue\n");
        printf("2 dequeue\n");
        printf("3 peek\n");
        printf("4 display\n");
        printf("5 exit\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter element for push : ");
            scanf("%d", &ele);
            enqueue(ele);
            break;
        case 2:
            dequeue();
            break;
        case 3:
            peek();
            break;
        case 4:
            display();
            break;
        case 5:
            exit(1);
        default:
            printf("Wrong choice\n");
            break;
        }
    }
}