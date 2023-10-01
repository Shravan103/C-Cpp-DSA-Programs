#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
char stack[MAX];
int top = -1;
void push(char x)
{
    stack[++top] = x;
}

char pop()
{
    if (top == -1)
        return -1;
    else
    {
        char item = stack[top];
        top = top - 1;
        return item;
    }
}
int check_palidrome(char *str)
{
    int len;
    len = strlen(str);
    int mid = len / 2;
    for (int i = 0; i < mid; i++)
    {
        push(str[i]);
    }
    char tmp;
    for (int i = mid + 1; i < len; i++)
    {
        tmp = pop();
        if (tmp != str[i])
        {
            return 0;
        }
    }
    return 1;
}
int main()
{
    char str[MAX];
    printf("Enter the string : ");
    gets(str);
    int check = check_palidrome(str);
    if (check == 1)
    {
        printf("Palidrome string");
    }
    else
    {
        printf("Not a palidrome string");
    }
    return 0;
}