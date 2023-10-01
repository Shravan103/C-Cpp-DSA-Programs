/*Menue driven code on  1]base conversion  2]tower of hanoi  3]GCD  4]reverse a string  5]search in LL*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node
{
    int data;
    struct node *next;
};
int base_conv(int num, int base);
void tower(int n, char beg, char aux, char end);
int gcd(int x,int y);
void reverse(char str[20]);
int main()
{
    int choice, num, base, n;
    char A, B, C;
    int x,y;
    char str[20];
    while (1)
    {
        printf("\n1 Base conversion\n");
        printf("2 Tower of hanoi\n");
        printf("3 Greatest common divisor\n");
        printf("4 Reverse a string\n");
        printf("5 search Linked List\n");
        printf("0 exit\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter the number for conversion : ");
            scanf("%d", &num);
            printf("Enter the base for conversion : ");
            scanf("%d", &base);
            printf("Decimal number %d converted to base %d is (%d)", num, base, base_conv(num, base));
            break;
        case 2:
            printf("Enter the number of disks : ");
            scanf("%d", &n);
            tower(n, 'A', 'C', 'B');
            break;
        case 3:
            printf("Enter two numbers : ");
            scanf("%d   %d", &x, &y);
            printf("GCD = %d",gcd(x,y));
            break;
        case 4:
            printf("Enter the string : ");
            scanf("%s",&str);
            reverse(str);
        case 0:
            exit(1);
        default:
            printf("wrong choice !!\n");
            break;
        }
    }
}
// uses return statement to return value to the calling function
int base_conv(int num, int base)
{
    if (num == 0 || base == 0)
        return num;
    else
    {
        return ((num % base) + 10 * base_conv(num / base, base));
    }
}
// void function as printf is used to print results in the function itself(no returning value to calling function)
void tower(int n, char beg, char end, char aux)
{
    if (n <= 0)
    {
        printf("illegal entry !!\n");
    }
    else if (n == 1)
    {
        printf("Move disc 1 from %c to %c\n", beg, end);
        return;
    }
    else
    {
        tower(n - 1, beg, aux, end);
        printf("Move disc %d from %c to %c\n", n, beg, end);
        tower(n - 1, aux, end, beg);
    }
}
int gcd(int x,int y)
{
    if(x == 0)
        return y;
    if(y == 0)
        return x;
    if(x == y)
        return x;
    if(x>y)
        return gcd(x-y,y);
    else
        return gcd(x,y-x);
}
void reverse(char * a)
{
    if (*a)
    {
        reverse(a+1);
        printf("%c",*a);
    }
    
}