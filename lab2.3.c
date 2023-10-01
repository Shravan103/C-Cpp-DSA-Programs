#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
struct node
{
int ele;
struct node*next;
};
void get_s(char *s)
{
a:gets(s);
if(strlen(s)==0)
goto a;
}
struct node* push(struct node*top,int data)
{
struct node *temp=malloc(sizeof(struct node));
temp->ele=data;
temp->next=top;
top=temp;
return top;
}
struct node* pop(struct node*top,int *n)
{
if(top==NULL)
return NULL;
*n=top->ele;
struct node *temp=top;
top=top->next;
free(temp);
return top;
}
int operation(int m,int n,char c)
{
switch(c)
{
case '+':return m+n;
case '-':return m-n;
case '*':return m*n;
case '/':return n!=0?(int)m/n:__INT32_MAX__;
case '%':return m%n;
case '^':return pow(m,n);
}
}
int postfix_evaluation(char *postfix)
{
int i,n,n1,n2;
struct node *top=NULL;
for(i=n=0;i<strlen(postfix);i++)
{
if(isdigit(postfix[i]))
{
n=n*10+postfix[i]-'0';
if(postfix[i+1]==' ')
{
top=push(top,n);
n=0;
}
}
else if(postfix[i]=='+'||postfix[i]=='-'||postfix[i]=='*'||postfix)
{
if(n!=0)
{
printf("incorrect expression");
return 0;
}
top=pop(top,&n1);
top=pop(top,&n2);
top=push(top,operation(n2,n1,postfix[i]));
}
}
return top->ele;
}
int main()
{
char s[100];
printf("Enter postfix expression = ");
get_s(s);
printf("Ans=%d",postfix_evaluation(s));
}