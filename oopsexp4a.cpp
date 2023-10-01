//overloading pre and post increment and decrement operators
#include<iostream>
//#include<bits/stdc++.h>
using namespace std;
class samp
{
    int a;
    public:
    samp();
    void display();
    void operator ++();
    void operator ++(int);
    void operator --();
    void operator --(int);
};
samp :: samp()
{
    a = 0;
}
void samp :: display()
{
    cout<<"a = "<<a<<endl;
}
void samp :: operator ++()
{
    ++a;
}
void samp :: operator ++(int)
{
    a++;

}
void samp :: operator --()
{
    --a;
}
void samp :: operator --(int)
{
    a--;
}
int main()
{
    samp o1;
    cout<<"initial value of a"<<endl;
    o1.display();
    ++o1;
    o1.display();
    o1++;
    o1.display();
    --o1;
    o1.display();
    o1--;
    o1.display();
}