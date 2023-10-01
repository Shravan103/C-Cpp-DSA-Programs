#include<bits/stdc++.h>
using namespace std;
class binary
{
    int a;
    public:
    binary ()
    {
        a=0;
    }
    binary (int m)
    {
        a=m;
    }
    friend binary operator + (int p,binary & obj1)
    {
        binary temp;
        temp.a = p + obj1.a;
        return temp;
    }
    binary operator * (binary & obj1)
    {
        binary temp;
        temp.a = a * obj1.a;
        return temp;
    }
    binary operator - (int k)
    {
        binary temp;
        temp.a = a - k;
        return temp;
    }
    void display()
    {
        cout<<" x = "<<a<<endl;
    }

};

int main()
{
    //setting y.a = 5;
    binary x,y(5);
    x = 5 + y;
    cout<<"x = 5 + y --------->" ;
    x.display();
    x = x * y;
    cout<<"x = x * y --------->" ;
    x.display();
    x = y - 5;
    cout<<"x = y - 5 --------->" ;
    x.display();
    
}