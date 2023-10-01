#include<iostream>
using namespace std;
//swap values of two variables

void swapvar(int &a,int &b)
{
    int temp;
    temp=a;
    a=b;
    b=temp;
}

int main()
{
    int x=5;
    int y=10;
    cout<<"The value of x = "<<x<<" and y = "<<y<<endl;
    swapvar(x,y);
    cout<<"The value of x = "<<x<<" and y = "<<y<<endl;

}