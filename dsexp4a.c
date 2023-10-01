//queues using Linked List ( Completed )
#include <stdio.h>
#include <stdlib.h>
struct node
{
    int data;
    struct node *next;
};
struct node * front = NULL;
struct node * rear = NULL;
void enqueue(int ele);
int dequeue();
void peek();
void display();
int main()
{
    int choice, ele, temp;
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
            
            temp = dequeue();
            if(temp == -1)
            {
                printf("Empty List !!\n");
            }
            else
            {
                printf("Dequeued element is %d\n",temp);
            }
            
            
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
void display()
{
    printf("In display.....\n");
    struct node * dis;;
    if(front == NULL && rear == NULL)
    {
        printf("List is Empty !!\n");
    }
    else
    {
        printf("In else part.....\n");
        dis = front;
        while (dis!=NULL)
        {
            printf("%d  ",dis->data);
            dis = dis -> next;
        }
    }
}
void peek()
{
    if(front == NULL && rear == NULL)
    {
        printf("List is Empty !!\n");
    }
    else
    {
        printf("Front element is %d\n",front->data);
    }
}
void enqueue(int ele)
{
    struct node * newnode = (struct node*)malloc(sizeof(struct node));
    newnode->data = ele;
    newnode->next = NULL;
    if(front == NULL && rear == NULL)
    {
        front = rear = newnode;
    }
    else
    {
        rear -> next = newnode;
        rear = newnode;
    }
}

int dequeue()
{
    if(front == NULL && rear == NULL)
    {
        return -1;
    }
    else
    {   
        int o;
        struct node * samp = front;
        o = front->data;
        front = front->next;
        free(samp);
        return o;
    }
}