#include<stdio.h>

int convert(int number,int base)
{
    if(number==0||base==0)
    return number;

    return (number % base) + 10*convert(number/base,base);
}

void tower(int num,char frompeg,char topeg,char auxpeg)
{
    if (num==1)
    {
        printf("move disk 1 form %c to %c\n",frompeg,topeg);
        return;
    }
    tower(num-1,frompeg,auxpeg,topeg);
        printf("move disk %d from %c to %c\n",num,frompeg,topeg);
    tower(num-1,auxpeg,topeg,frompeg);
}
int greatest_common_divisor(int a, int b)
{
    if (a == 0)
        return b;
    if (b == 0)
        return a;
    if (a == b)
        return a;
    if (a > b)
        return greatest_common_divisor(a - b, b);
    else
        return greatest_common_divisor(a, b - a);
}
void reverse_string(char *a)
{
    if (*a)
    {
        reverse_string(a + 1);
        printf("%c", *a);
    }
}
struct NODE
{
    /* data */
    int data;
    struct NODE *next;
};
typedef struct NODE node;
node *create_list(node *start)
{
    int n;
    printf("Enter Number of elements\n");
    scanf("%d", &n);
    node *p = start;
    for (int i = 0; i < n; i++)
    {
        node *temp = (node *)malloc(sizeof(node));
        temp->next = NULL;
        printf("Enter data\n");
        scanf("%d", &temp->data);
        if (start == NULL)
        {
            start = temp;
            p = temp;
        }
        else
        {
            p->next = temp;
            p = p->next;
        }
    }
    return start;
}
void display_list(node *start)
{
    node *p = start;
    while (p)
    {
        printf("%d\t", p->data);
        p = p->next;
    }
    printf("\n");
}
node *search(node *start, int data)
{
    if (start == NULL)
    {
        printf("Element Not Found\n");
        return NULL;
    }
    if (start->data == data)
    {
        printf("Element found\n");
        return NULL;
    }
    else
    {
        search(start->next, data);
    }
}


int main()
{
    while(1)
    {
        int choice,num2,numb;
        printf("Enter the choice:\n");
        printf("1. Base conversions\n");
        printf("2. Tower of Hanoi\n");
        printf("3. Greatest common divisor\n");
        printf("4. Reverse a string\n");
        printf("5. Search a string\n");
        printf("0. Exit\n");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
            	printf("enter the number for the conversion : ");
            	scanf("%d",&numb);
                for(int i=2;i<=16;i++)
                    printf("%d is %d base(%d)\n",numb,convert(numb,i),i);
                break;
            case 2:
                printf("Enter the number of disks: ");
                scanf("%d",&num2);
                tower(num2,'A','C','B');
                break;
            case 3:
            	printf("Enter 2 numbers\n");
	            {
	                int a, b;
	                scanf("%d %d", &a, &b);
	                printf("%d\n", greatest_common_divisor(a, b));
	            }
	            break;
	        case 4:
	        	printf("Enter a String\n");
	            {
	                char a[100];
	                scanf("%s", a);
	                reverse_string(a);
	                printf("\n");
	            }
	            break;
	        case 5:
	        {
	            node *start = NULL;
	            start = create_list(start);
	            printf("Enter data to be searched\n");
	            int data;
	            scanf("%d", &data);
	            search(start, data);
		    }
		    break;
        }


    }
}
