#include <iostream>
#include <string.h>
#include <stdlib.h>
#include<iomanip>
using namespace std;

class customer
{
    char name[20];
    long double phone;
    char ad[100];
    int acc;
    int balance;

public:
    char create(customer list[100], int n);
    int display(customer list[100], int n);
    int search(customer list[100], int n, int number);
    void deposite(customer list[100], int n, int number, int amt);
    void withdraw(customer list[100], int n, int number, int amt);
    void problem(void);
};

char customer ::create(customer list[100], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        cout<<"FOR A/C "<<i + 1<<endl;
        cout<<"enter the account holders name: "<<endl;
        fflush(stdin);
        cin>>list[i].name;
        cout<<"enter the account holders phone number: "<<endl;
        scanf("%Lf", &list[i].phone);
        cout<<"enter the account holders address: "<<endl;
        fflush(stdin);
        cin>>list[i].ad;
        list[i].acc = 1000 + i;
        cout<<"your account number is:"<<list[i].acc<<endl;
        list[i].balance = 0;
    }
}

int customer ::display(customer list[100], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        cout<<"FOR A/C "<<i + 1<<endl;
        cout<<"The account number is "<<list[i].acc<<endl;
        cout<<"The name of the account holder is "<<list[i].name<<endl;
        printf("The account holders phone number is %.0Lf\n", list[i].phone);
        cout<<"The address of account holder is "<<list[i].ad<<endl;
    }
}

int customer ::search(customer list[100], int n, int number)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (list[i].acc == number)
        {
            cout<<"The name of the account holder is "<<list[i].name<<endl;
            printf("The phone number of account holder is %.0Lf\n",list[i].phone);
            cout<<"The address of the account holder is "<<list[i].ad<<endl;
            cout<<"The balance of the account holder is "<<list[i].balance<<endl;
            return i;
        }
        else
        {
            cout<<"Record not found"<<endl;
            return -1;
        }
    }
    
}

void customer ::deposite(customer list[100], int n, int number, int amt)
{
    int i = search(list, n, number);
    if (i == -1)
    {
        cout<<"Record not found"<<endl;
    }
    else
    {
        list[i].balance += amt;
        cout<<"Updated total ammount is "<<list[i].balance<<endl;
    }
}

void customer ::withdraw(customer list[100], int n, int number, int amt)
{
    int i = search(list, n, number);
    if (i == -1)
    {
        cout<<"Record not found "<<endl;
    }
    else if (list[i].balance < amt)
    {
        cout<<"Insufficient balance "<<endl;
    }
    else
    {
        list[i].balance -= amt;
        cout<<"Updated Total ammount is "<<list[i].balance<<endl;
    }
}

void customer::problem()
{
}

int main()
{
    customer data[100];
    int p, n, account_no, index, ammount;
    do
    {
        cout << "Press 1 to create an account" << endl;
        cout << "Press 2 to display the details" << endl;
        cout << "Press 3 to search the account details" << endl;
        cout << "Press 4 to deposite the amount" << endl;
        cout << "Press 5 to withdraw the amount" << endl;
        cout << "Press 0 to exit" << endl;
        cin >> p;

        switch (p)
        {
        case 1:
            cout << "Enter the number of accounts to be created:" << endl;
            cin >> n;
            data[100].create(data, n);
            break;

        case 2:
            data[100].display(data, n);
            break;

        case 3:
            cout << "Enter the account number to be searched: " << endl;
            cin >> account_no;
            index = data[100].search(data, n, account_no);
            if (index == -1)
            {
                cout << "Record not found : " << endl;
            }
            

        case 4:
            cout << "enter the account number: " << endl;
            cin >> account_no;
            cout << "Enter the ammount to be deposited: " << endl;
            cin >> ammount;
            data[100].deposite(data, n, account_no, ammount);
            break;

        case 5:
            cout << "Enter account number : " << endl;
            cin >> account_no;
            cout << "Enter amount to withdraw : " << endl;
            cin >> ammount;
            data[100].withdraw(data, n, account_no, ammount);
        }
    } while (p != 0);
}