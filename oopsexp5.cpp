//program on constructor in derived class
#include<iostream>
#include<string.h>
#include<stdlib.h>
using namespace std;
class base1
{
    char b1[20];
    public:
    base1(char b1_cpy[20])
    {
        strcpy(b1,b1_cpy);
        // for (int i = 0; i < strlen(b1_cpy); i++)
        // {
        //     b1[i] = b1_cpy[i];
        // }   
    }
    int displayb1()
    {
        cout<<"string b1 = "<<b1<<endl;
    }
};
class base2
{
    char b2[20];
    public:
    base2(char b2_cpy[20])
    {
        strcpy(b2,b2_cpy);
        // for (int i = 0; i < strlen(b2_cpy); i++)
        // {
        //     b2[i] = b2_cpy[i];
        // }   
    }
    int displayb2()
    {
        cout<<"string b2 = "<<b2<<endl;
    }
};

class derived : public base1 , public base2
{
    int data;
    public:  //same as constructor declaration               //same as main() calling declaration
    derived (char b1_cpy[20],char b2_cpy[20],int data_cpy) : base1(b1_cpy),base2(b2_cpy)
    {
        data = data_cpy;
    }
    int displaydata()
    {
        displayb1();
        displayb2();
        cout<<"data= "<<data<<endl;
    }
};
int main()
{
    char b1_cpy[20];
    printf("Enter a string for Base1: ");
    gets(b1_cpy);
    // base1 o1(b1_cpy);
    
    char b2_cpy[20];
    printf("Enter a string for Base2: ");
    gets(b2_cpy);
    // base2 o1(b2_cpy);

    int data_cpy;
    cout<<"Enter the value for data : ";
    cin>>data_cpy;
    derived d(b1_cpy,b2_cpy,data_cpy);
    d.displaydata();
}