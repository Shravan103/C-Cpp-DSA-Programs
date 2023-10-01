// stack using linked list
#include <stdio.h>
#include <stdlib.h>
struct node
{
    int data;
    struct node *next;
};
struct node *top = NULL;
struct node *push(int ele)
{
    struct node *newnode = (struct node *)malloc(sizeof(struct node));
    newnode->data = ele;
    newnode->next = top;
    top = newnode;
}
struct node *pop()
{
    struct node *ptr = top;
    if (top == NULL)
    {
        printf("Underflow condition!!\n");
    }
    else
    {
        printf("popped element is %d", top->data);
        top = top->next;
        free(ptr);
    }
}
struct node *peek()
{
    if (top == NULL)
    {
        printf("Underflow condition!!\n");
    }
    else
    {
        printf("Top element is %d", top->data);
    }
}
struct node *display()
{
    if (top == NULL)
    {
        printf("Underflow condition!!\n");
    }
    else
    {
        struct node *ptr = top;
        while (ptr != NULL)
        {
            printf("%d  ", ptr->data);
            ptr = ptr->next;
        }
    }
}
int main()
{
    int choice, ele;
    while (1)
    {
        printf("\n1 push\n");
        printf("2 pop\n");
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
            push(ele);
            break;
        case 2:
            pop();
            break;
        case 3:
            peek();
            break;
        case 4:
            display();
            break;
        default:
            printf("Wrong choice\n");
            break;
        }
    }
}