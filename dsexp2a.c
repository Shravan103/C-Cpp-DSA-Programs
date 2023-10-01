//stacks using arrays
#include<stdio.h>
# define N 5
int stack[N];
int top = -1;
int push(int ele)
{
    if (top == N-1)
    {
        printf("overflow condition!!\n");
    }
    else if (top == -1)
    {
        top = 0;
        stack[top] = ele;
    }
    else
    {
        top++;
        stack[top] = ele;
    }   
}
void pop()
{
    if (top == -1)
    {
        printf("Underflow condition!!\n");
    }
    else
    {
        int a = stack[top];
        top--;
        printf("popped element is %d",a);
    }
}
void peek()
{
    if(top==-1)
    {
        printf("Underflow condition!!\n");
    }
    else
    {
        printf("Top element = %d",stack[top]);
    }
    
}

void display()
{
    if (top == -1)
    {
        printf("Underflow condition !!\n");
    }
    else
    {
        for(int i=0 ; i<=top ; i++)
        {
            printf("%d  ",stack[i]);
        }
        printf("\n");
    }
    
    
}
int main()
{
    int choice,ele;
    while (1)
    {
        printf("\n1 push\n");
        printf("2 pop\n");
        printf("3 peek\n");
        printf("4 display\n");
        printf("5 exit\n");
        printf("Enter your choice : ");
        scanf("%d",&choice);

        switch (choice)
        {
        case 1:
            printf("Enter element for push : ");
            scanf("%d",& ele);
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
        default:
            printf("Wrong choice\n");
            break;
        }
    }
    
}