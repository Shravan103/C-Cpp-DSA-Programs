#include <stdio.h>
#include <stdlib.h>

int main()
{
    //creation of LL
    struct node
    {
        int data;
        struct node *next;
    };

    int choice;

    struct node *head, *newnode, *temp;
    head = 0;
    while (choice)
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
        scanf("%d", &choice);
    }

    //Traversal of LL

    temp=head;
    while (temp != 0)
    {
        printf("Data: %d\n",temp->data);
        temp=temp->next;
    }
    getch();
}