#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 100
char stack[MAX];
int top=-1;
void push(char x)
{
    stack[++top] = x;
}

char pop()
{
    if(top == -1)
        return -1;
    else{
        char item = stack[top];
        top=top-1;
        return item;
    }
}

int main()
{
    int n;
    printf("Enter the length of the string : ");
    scanf("%d",&n);
    printf("Enter the string : ");
    for(int i=0;i<=n;i++)
    {
        char c;
        scanf("%c",&c);
       push(c);
    }
   
    printf("The reversed string is : ");
    for(int i=0;i<n;i++)
    {
        int item=pop();
        printf("%c",item);
    }
    return 0;
}