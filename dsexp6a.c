// swapping of adjacent nodes by rearranging the links
#include <stdio.h>
#include <stdlib.h>
struct node
{
    struct node *prev;
    int data;
    struct node *next;
};
struct node *create(struct node *head, int size);
void display(struct node *head);
struct node *swap(struct node *head, int size);
int main()
{
    int size;
    struct node *head = NULL;
    printf("Enter the size of the node : ");
    scanf("%d", &size);
    head = create(head, size);
    printf("List before swapping :\n");
    display(head);
    printf("\nList after swapping :\n");
    head = swap(head, size);
    display(head);
}
struct node *swap(struct node *head, int size)
{

    if (size > 1)
    {
        if (size == 2)
        {
            struct node * x = head, *y;
            y = x->next;
            y->prev = x->prev;
            y->next = x;
            x->prev = y;
            x->next = NULL;
            head = y;
        }
        else
        {
            struct node *p = head, *q, *r, *t;
            q = p->next;
            p->next = q->next;
            q->next->prev = p;
            q->prev = p->prev;
            q->next = p;
            p->prev = q;
            head = q;
            p = p->next;
            while (p!=NULL && p->next!=NULL)
            {
                if(p->next->next == NULL)
                {
                    q = p->next;
                    r = p->prev;
                    q->prev = r;
                    p->next = q->next;
                    q->next = p;
                    p->prev = q;
                    r->next = q;
                }
                else
                {
                    r = p->prev;
                    q = p->next;
                    t = q->next;
                    q->prev = r;
                    p->next = t;
                    q->next = p;
                    p->prev = q;
                    r->next = q;
                    t->prev = p;
                    p = p->next;
                }
                
            }
            
        }
    }
    return head;
}
struct node *create(struct node *head, int size)
{
    int data;
    for (int i = 0; i < size; i++)
    {
        struct node *temp;
        printf("Enter the data for node %d : ", i + 1);
        scanf("%d", &data);
        struct node *newnode = (struct node *)malloc(sizeof(struct node));
        newnode->data = data;
        newnode->prev = NULL;
        newnode->next = NULL;
        if (head == NULL)
            head = temp = newnode;
        else
        {
            temp->next = newnode;
            newnode->prev = temp;
            temp = newnode;
        }
    }
    return head;
}
void display(struct node *head)
{
    struct node *tempo = head;
    while (tempo != NULL)
    {
        printf("%d  ", tempo->data);
        tempo = tempo->next;
    }
}