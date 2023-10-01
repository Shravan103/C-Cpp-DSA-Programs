// queues using linked list (incomplete)
#include <stdio.h>
#include <stdlib.h>
struct node
{
    int data;
    struct node *next;
};
struct node *front = NULL;
struct node *rear = NULL;
struct node *enqueue(int data)
{
    
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