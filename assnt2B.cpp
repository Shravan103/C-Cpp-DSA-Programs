#include<iostream>
#include<string.h>
#include<stdlib.h>
using namespace std;
class electricity_board
{
        char name[40];
        float bill_amnt;
        float units;
    public:
        void getdata()
        {
            cout<<"Enter the name of the user:";
            fflush(stdin);
            gets(name);
            cout<<"Enter the units consumed: ";
            cin>>units;
        }
        void amnt_initialize()
        {
            bill_amnt=0;
            if(units<=100)
            {
                bill_amnt=units*60.0;
            }
            else if (units>100 && units <=300)
            {
                float s=units - 100.00;
                bill_amnt=(60.0*100)+(80.0*s);
            }
            else if(units>300)
            {
                float x= units-300;
                bill_amnt=(60.0 * 100.00) + (80.0 * 200.00) + (90.0 * x);
            }

            if(bill_amnt<50)
                bill_amnt=50;
            else if(bill_amnt>300)
            {
                bill_amnt=bill_amnt+(bill_amnt*0.15);
            }
        }
        void putdata()
        {
            cout<<"Name of the user: "<<name<<endl;
            cout<<"units consumed by user: "<< units<<endl;
            cout<<"Bill amount to be paid by user: "<<bill_amnt<<endl;
        }
};

int main()
{
    int n;
    electricity_board * ptr;
    cout<<"Enter the number of public users: ";
    cin>>n;
    ptr = new electricity_board[n];
    for(int i=0;i<n;i++)
        ptr[i].getdata();
    for(int i=0;i<n;i++)
        ptr[i].amnt_initialize();
    for(int i=0;i<n;i++)
        ptr[i].putdata();
    
}