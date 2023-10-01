#include<stdio.h>
struct customer
{
    char name[20];
    long int phone;
    char ad[100];
    int acc;
    int balance;
};

char create(struct customer list[100],int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        printf("enter the account holders name:\n");
        scanf("%s",&list[i].name);
        printf("enter the account holders phone number:\n");
        scanf("%ld",&list[i].phone);
        printf("enter the account holders address:\n");
        scanf("%s",&list[i].ad);
        list[i].acc=1000+i;
        printf("your account number is:\n%d\n",list[i].acc);
        list[i].balance=0;
    }
}

int display(struct customer list[100],int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("The account number is %d\n",list[i].acc);
        printf("The name of the account holder is %s\n",list[i].name);
        printf("The account holders phone number is %ld\n",list[i].phone);
        printf("The address of account holder is %s\n",list[i].ad);
    }
}

int search(struct customer list[100], int n, int number)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (list[i].acc == number)
        {
            return i;
        } 
    }
    return  - 1;
}

void deposite(struct customer list[100],int n,int number,int amt)
{
    int i=search(list,n,number);
    if (i ==  - 1)
    {
        printf("Record not found");
    } 
    else
    {
        list[i].balance += amt;
        printf("total ammount is %d",list[i].balance);
    }
}

void withdraw(struct customer list[100],int n,int number,int amt)
{
    int i = search(list,n,number);
    if (i ==  - 1)
    {
        printf("Record not found\n");
    } 
    else if (list[i].balance < amt)
    {
        printf("Insufficient balance\n");
    }
    else
    {
        list[i].balance -= amt;
        printf("Total ammount is %d",list[i].balance);
    }
}

int main()
{
    struct customer data[100];
    int p,n,account_no,index,ammount;

    do
    {
        printf("Press 1 to create an account\n");
        printf("Press 2 to display the details\n");
        printf("Press 3 to search the account details\n");
        printf("Press 4 to deposite the amount\n");
        printf("Press 5 to withdraw the amount\n");
        printf("Press 0 to exit\n");
        scanf("%d",&p);

        switch (p)
        {
            case 1:
            printf("Enter the number of accounts to be created:");
            scanf("%d",&n);
            create(data,n);
            break;

            case 2:
            display(data,n);
            break;

            case 3:
            printf("Enter the account number to be searched: ");
            scanf("%d",&account_no);
            index=search(data,n,account_no);
            if (index ==  - 1)
            {
                printf("Record not found : ");
            }
            else
            {
                printf("The name of the account holder is %s\n",data[index].name);
                printf("The phone number of account holder is %ld\n",data[index].phone);
                printf("The address of the account holder is %s\n",data[index].ad);
                printf("The balance of the account holder is %d\n",data[index].balance);
            }
            break;

            case 4:
            printf("enter the account number: ");
            scanf("%d",&account_no);
            printf("Enter the ammount to be deposited: ");
            scanf("%d",&ammount);
            deposite(data,n,account_no,ammount);
            break;

            case 5:
            printf("Enter account number : ");
            scanf("%d", &account_no);
            printf("Enter amount to withdraw : ");
            scanf("%d", &ammount);
            withdraw(data, n, account_no, ammount);
        }
    
    } while(p != 0);

}