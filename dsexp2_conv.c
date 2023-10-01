// Infix to Postfix conversion
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 100
#define SPACE ' '
#define TAB '\t'
long int stack[MAX];
char infix[MAX]; char postfix[MAX];
int top = -1;
int push(long int symbol);
long int pop();
int whitespace(char symbol);
int Empty();
void in_to_po();
int instack_priority (char symbol);
int incoming_priority(char symbol);
int main()
{

    printf("Enter the infix expression : ");
    gets(infix);
    in_to_po();
    printf("Postfix expression :    %s",postfix);
}
void in_to_po()
{
    int i,j=0;
    char symbol, next;
    for(i=0;i<strlen(infix);i++)
    {
        symbol = infix[i];
        if(!whitespace(symbol))
        {
            switch (symbol)
            {
            case '(':
                push(symbol);
                break;
            
            case ')':
                while((next = pop()) != '(')
                    postfix[j++] = next;
                break;

            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
                while(!Empty() && instack_priority(stack[top]) >= incoming_priority(symbol))
                    postfix[j++] = pop();
                push(symbol);
                break;

            default:
                postfix[j++] = symbol;
                break;
            }
        }
    }
    while (!Empty())
        postfix[j++] = pop();
    postfix[j] = '\0';
}
int instack_priority (char symbol)
{
    switch(symbol)
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}
int incoming_priority(char symbol)
{
    switch(symbol)
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '^':
            return 4;
        default:
            return 0;
    }
}
int whitespace(char symbol)
{
    if(symbol == SPACE || symbol == TAB)
        return 1;
    else
        return 0;
}
int Empty()
{
    if (top == -1)
        return 1;
    else
        return 0;
    
}
int push(long int symbol)
{
    if (top == MAX-1)
    {
        printf("overflow condition!!\n");
    }
    else if (top == -1)
    {
        top = 0;
        stack[top] = symbol;
    }
    else
    {
        top++;
        stack[top] = symbol;
    }   
}
long int pop()
{
    if (top == -1)
    {
        printf("Underflow condition!!\n");
    }
    else
    {
        return (stack[top--]);
    }
}