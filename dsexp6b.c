// Deletion of alternate node in circular linked list
#include <stdio.h>
#include <stdlib.h>
struct node
{
    int data;
    struct node *next;
};
struct node *tail = NULL;
struct node *create(struct node *head, int size);
void display(struct node *head);
int length_s(struct node *);
void altdel(struct node *head, int size);
int main()
{
    int size;
    struct node *head = NULL;
    printf("Creating a circular linked list.....\n");
    printf("Enter the size : ");
    scanf("%d", &size);
    head = create(head, size);
    printf("Displaying list.....\n");
    display(head);
    printf("Deleting Alternate nodes.....\n");
    altdel(head, size);
    display(head);
    printf("%d and %d", tail->next->data, head->data);
}

void altdel(struct node *head, int size)
{
    int count =1;
    int len =length_s(head);
    printf("Lenght = %d\n",len);
    if(head == NULL)
    {
        printf("Empty list\n");
        return;
    }
    if(len<2)
    {
        printf("only one element\n");
        return;
    }
    struct node *prev = head;
    struct node * nex = prev->next;
    while(len>0)
    {
        if(count%2 == 0)
        {
            prev->next = nex->next;
            free(nex);
            prev = prev->next;
            nex = prev->next;
        }
        len--;count++;
    }
}
void display(struct node *head)
{
    struct node *temp = head;
    // do
    // {
    //     printf("%d  ",temp->data);
    //     temp = temp->next;
    // }while(temp!=head);
    while (temp->next != head)
    {
        printf("%d  ", temp->data);
        temp = temp->next;
    }
    printf("%d  \n", temp->data);
}
struct node *create(struct node *head, int size)
{

    for (int i = 0; i < size; i++)
    {
        int value;
        struct node *newnode = (struct node *)malloc(sizeof(struct node));
        printf("Enter the data for node %d : ", i + 1);
        scanf("%d", &value);
        newnode->data = value;
        newnode->next = NULL;
        if (head == NULL)
            head = tail = newnode;
        else
        {
            tail->next = newnode;
            tail = newnode;
        }
        tail->next = head;
    }
    return head;
}
int length_s(struct node * head)
{
    struct node *temp = head;
    int count = 0;
    do
    {
        count++;
        temp = temp->next;
    } while (temp!=head);
    return count;
}