//all operations on singly linked list
#include <stdio.h>
#include <stdlib.h>
struct node
{
    int data;
    struct node *next;
};

struct node *temp_tail = NULL;
struct node *create(struct node *head)
{
    int num, data;
    printf("Enter the number of elements for creation : ");
    scanf("%d", &num);
    for (int i = 0; i < num; i++)
    {
        printf("Enter data %d = ", i + 1);
        scanf("%d", &data);
        struct node *newnode = (struct node *)malloc(sizeof(struct node));
        newnode->data = data;
        newnode->next = NULL;
        if (head == NULL)
        {
            head = temp_tail = newnode;
        }
        else
        {
            temp_tail->next = newnode;
            temp_tail = temp_tail->next;
        }
    }
    return head;
}
int display(struct node *head)
{
    struct node *head_copy = head;
    if (head_copy == NULL)
    {
        printf("Empty list !!");
    }
    else
    {
        while (head_copy != NULL)
        {
            printf("%d  ", head_copy->data);
            head_copy = head_copy->next;
        }
    }
}
struct node *insert_beg(struct node *head, int data)
{
    struct node *newnode = (struct node *)malloc(sizeof(struct node));
    newnode->data = data;
    newnode->next = head;
    head = newnode;
    return head;
}
struct node *insert_bef(struct node *head, int data, int pos)
{
    struct node *temp = head;
    struct node *newnode = (struct node *)malloc(sizeof(struct node));
    newnode->data = data;
    int i = 1;
    while (i < pos - 1)
    {
        temp = temp->next;
        i++;
    }
    newnode->next = temp->next;
    temp->next = newnode;
    return head;
}
struct node *insert_aft(struct node *head, int data, int pos)
{
    struct node *temp = head;
    struct node *newnode = (struct node *)malloc(sizeof(struct node));
    newnode->data = data;
    int i = 1;
    while (i < pos)
    {
        temp = temp->next;
        i++;
    }
    newnode->next = temp->next;
    temp->next = newnode;
    return head;
}
struct node *insert_end(struct node *head, int data)
{
    struct node *newnode = (struct node *)malloc(sizeof(struct node));
    newnode->data = data;
    newnode->next = NULL;
    if (head == NULL && temp_tail == NULL)
    {
        printf("empty list!!......inserting.......");
        head = temp_tail = newnode;
    }
    else
    {
        temp_tail->next = newnode;
        temp_tail = newnode;
    }
    return head;
}
int count(struct node *head)
{
    struct node *ptr = head;
    int count = 0;
    if (ptr == NULL)
    {
        return -1;
    }
    else
    {
        while (ptr != NULL)
        {
            ptr = ptr->next;
            count++;
        }
        return count;
    }
}

int main()
{
    int choice, data, pos;
    struct node *head = NULL;

    head = create(head);
    while (1)
    {
        printf("\n1 display \n");
        printf("2 insert begining \n");
        printf("3 insert before node \n");
        printf("4 insert after node \n");
        printf("5 insert end \n");
        printf("6 delete begining \n");
        printf("7 delete at position \n");
        printf("8 delete end \n");
        printf("9 serach \n");
        printf("10 count \n");
        printf("0 exit \n");
        printf("Enter choice : ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            display(head);
            break;
        }
        case 2:
        {
            printf("Enter the element to be inserted at begining : ");
            scanf("%d", &data);
            head = insert_beg(head, data);
            break;
        }
        case 3:
        {
            printf("Enter data to be inserted before node : ");
            scanf("%d", &data);
            printf("Enter the pos before which node is to be inserted");
            scanf("%d", &pos);
            head = insert_bef(head, data, pos);
            break;
        }
        case 4:
        {
            printf("Enter data to be inserted after node : ");
            scanf("%d", &data);
            printf("Enter the pos after which node is to be inserted");
            scanf("%d", &pos);
            head = insert_aft(head, data, pos);
            break;
        }
        case 5:
        {
            printf("Enter data to be inserted at end node : ");
            scanf("%d", &data);
            head = insert_end(head, data);
            break;
        }
        case 10:
        {
            int op = count(head);
            if (op == -1)
                printf("Empty list......count 0");
            else
                printf("count is %d", op);
            break;
        }
        case 0:
        {
            exit(1);
        }
        default:
            printf("wrong choice\n");
        }
    }
}