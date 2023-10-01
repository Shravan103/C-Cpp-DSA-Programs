#include <stdio.h>
struct node
{
    int data;
    struct node *next;
};
struct node *head, *temp,*newnode;

int beg()
{
    
}

int main()
{
    int choice0,choice;

    head = 0;
    while (choice0)
    {
        newnode = (struct node *)malloc(sizeof(struct node));
        printf("Enter the Data: ");
        scanf("%d", &newnode->data);
        newnode->next = 0;
        if (head == 0)
        {
            head = temp = newnode;
        }
        else
        {
            temp->next = newnode;
            temp = newnode;
        }
        printf("Do you wish to continue: ");
        scanf("%d", &choice0);
    }
    do
    {
        printf("1 insert at begining \n2 insert at end \n3 insert at given position\n0 exit");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            beg();
            break;

        case 2:
            end();
            break;

        case 3:
            in_pos();
            break;
        }

    } while (choice != 0);
}