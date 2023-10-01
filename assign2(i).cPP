#include<iostream>
using namespace std;

class complex_no
{
        int a;
        int b;
    public:
        int setdata(int &m,int &n)
        {
            a=m;
            b=n;
            cout<<a<<" + "<<b<<" i"<<endl;
        }
    friend int sum(complex_no ,complex_no );
};

int sum(complex_no o1,complex_no o2)
{
    int p,q;
    p=o1.a+o2.a;
    q=o1.b+o2.b;
    complex_no o3;
    cout<<"The sum of complex num 1 and complex num 2 :"<<endl;
    o3.setdata(p,q);
}

int main()
{
    complex_no x,y;
    int real1,complex1,real2,complex2;
    cout<<"Enter the real part(complex num 1):";
    cin>>real1;
    cout<<"Enter the complex part(complex num 1):";
    cin>>complex1;
    x.setdata(real1,complex1);
    cout<<"Enter the real part(complex num 2):";
    cin>>real2;
    cout<<"Enter the complex part(complex num 2):";
    cin>>complex2;
    y.setdata(real2,complex2);
    sum(x,y);
}