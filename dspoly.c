// polynomial creation , addition , multiplication
#include <stdio.h>
#include <stdlib.h>
struct node
{
    int coef;
    int expo;
    struct node *next;
};

struct node *insert_s(struct node *head, int co, int ex)
{
    struct node *temp;
    struct node *newnode = (struct node *)malloc(sizeof(struct node));
    newnode->coef = co;
    newnode->expo = ex;
    newnode->next = NULL;

                       //imagine below condn(ex > head->expo) to write if block code
    if (head == NULL || ex > head->expo)
    {
        newnode->next = head;
        head = newnode;
    }
    else
    {
        temp = head;
        while (temp->next != NULL && temp->next->expo >= ex)
        {
            temp = temp->next;
        }
        newnode->next = temp->next;
        temp->next = newnode;
    }
    return head;
}
struct node *create(struct node *head)
{
    int num, co, ex;
    printf("Enter the number of elements to be inserted :");
    scanf("%d", &num);
    for (int i = 0; i < num; i++)
    {
        printf("Enter coefficient for  term %d : ", i + 1);
        scanf("%d", &co);
        printf("Enter the exponenet for term %d : ", i + 1);
        scanf("%d", &ex);
        head = insert_s(head, co, ex);
    }
    return head;
}

int display(struct node *head)
{
    if (head == NULL)
    {
        printf("No polynomal");
    }
    else
    {
        struct node *temp = head;
        while (temp != NULL)
        {
            printf("(%dx^%d)", temp->coef, temp->expo);
            temp = temp->next;
            if (temp != NULL)
                printf(" + ");
            else
                printf("\n");
        }
    }
}
int polyadd(struct node *head1, struct node *head2)
{
    struct node *p1 = head1;
    struct node *p2 = head2;
    struct node *p3 = NULL;
    while (p1 != NULL && p2 != NULL)
    {
        if (p1->expo == p2->expo)
        {
            p3 = insert_s(p3, p1->coef + p2->coef, p1->expo);
            p1 = p1->next;
            p2 = p2->next;
        }
        else if (p1->expo > p2->expo)
        {
            p3 = insert_s(p3, p1->coef, p1->expo);
            p1 = p1->next;
        }
        else if (p1->expo < p2->expo)
        {
            p3 = insert_s(p3, p2->coef, p2->expo);
            p2 = p2->next;
        }
    }
    while (p1 != NULL)
    {
        p3 = insert_s(p3, p1->coef, p1->expo);
        p1 = p1->next;
    }
    while (p2 != NULL)
    {
        p3 = insert_s(p3, p2->coef, p2->expo);
        p2 = p2->next;
    }
    printf("Polynomial after addition is: \n");
    display(p3);
}

int polymult(struct node *head1, struct node *head2)
{
    struct node *p1 = head1;
    struct node *p2 = head2;
    struct node *p3 = NULL;
    struct node *p2_beg = head2;
    if (p1 == NULL || p2 == NULL)
    {
        printf("Zero polynomial!!\n");
    }
    else
    {
        while (p1 != NULL)
        {
            head2 = p2_beg;
            while (p2 != NULL)
            {
                p3 = insert_s(p3, p1->coef * p2->coef, p1->expo + p2->expo);
                p2 = p2->next;
            }
            p1 = p1->next;
        }
        printf("Polynomial after multiplication is: \n");
        display(p3);
    }
}

int main()
{
    struct node *head1 = NULL;
    struct node *head2 = NULL;
    printf("Enter the polynomial 1\n");
    head1 = create(head1);
    display(head1);
    printf("Enter the polynomial 2\n");
    head2 = create(head2);
    display(head2);
    polyadd(head1, head2);
    polymult(head1, head2);
}