#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
struct node
{
    char info;
    struct node *link;
} *top = NULL;
char postfix[30];
char cop[30];
void push(char item);
char pop();
char peek();
int isEmpty();
void display();
void copy();
int op(char item);
int main()
{
    int choice;
    char item;
    int a, b, temp;
    int result;
    char ele;
    char symbol;
    while (1)
    {
        printf("1.Push\n");
        printf("2.Pop\n");
        printf("3.Display the top element\n");
        printf("4.Display all the stack elements\n");
        printf("5.Postfix to infix form\n");
        printf("6.Quit\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Enter the item to be pushed: ");
            getchar();
            scanf("%c", &item);
            push(item);
            break;
        case 2:
            item = pop();
            printf("Popped item is : %c\n ", item);
            break;
        case 3:
            printf("Item at the top is : %c\n ", peek());
            break;
        case 4:
            display();
            break;
        case 5:
            printf("\nEnter the expression in postfix form : ");
            scanf("%s", postfix);
            int i = 0;
            while (i < strlen(postfix))
            {
                ele = postfix[i];
                if (op(ele) == 0)
                {
                    push(ele);
                    push(' ');
                }
                if (op(ele) == 1)
                {
                    struct node *p = top->link;
                    while (p != NULL)
                    {
                        if (p->info == ' ')
                        {
                            p->info = ele;
                            break;
                        }
                        p = p->link;
                    }
                }
                i++;
            }
            copy();
            break;
        case 6:
            exit(1);
        default:
            printf("Wrong choice\n");
        }
    }
    return 0;
}
void push(char item)
{
    struct node *tmp;
    tmp = (struct node *)malloc(sizeof(struct
                                       node));
    if (tmp == NULL)
    {
        printf("Stack Overflow\n");
        return;
    }
    tmp->info = item;
    tmp->link = top;
    top = tmp;
}
char pop()
{
    struct node *tmp;
    char item;
    if (isEmpty())
    {
        printf("Stack Underflow\n");
        exit(1);
    }
    tmp = top;
    item = tmp->info;
    top = top->link;
    free(tmp);
    return item;
}
void display()
{
    struct node *ptr;
    ptr = top;
    if (isEmpty())
    {
        printf("Stack is empty\n");
        return;
    }
    printf("Stack elements :\n\n");
    while (ptr != NULL)
    {
        printf("%c ", ptr->info);
        ptr = ptr->link;
    }
    printf("\n");
}
char peek()
{
    if (isEmpty())
    {
        printf("Stack Underflow\n");
        exit(1);
    }
    return top->info;
}
int isEmpty()
{
    if (top == NULL)
        return 1;
    else
        return 0;
}
int op(char ele)
{
    if ((ele >= 'A' && ele <= 'Z') || (ele >= 'a' && ele <= 'z'))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
void copy()
{
    struct node *p = top;
    int i = 0;
    while (p != NULL)
    {
        cop[i] = p->info;
        p = p->link;
        i++;
    }
    int l = strlen(cop) - 1;
    while (l >= 0)
    {
        printf("%c ", cop[l]);
        l--;
    }
    printf("\n");
}