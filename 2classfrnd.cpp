#include <iostream>
using namespace std;
class two;//forward declaration
class one
{
    int a, b;

public:
    int setvalue(int x,int y)
    {
        a=x;
        b=y;
    }
    int getvalue()
    {
        cout<<"First complesx number is "<<a<<" + "<<b<<"i"<<endl;
    }
    friend int result(one,two);
};
class two
{
    int a, b;

public:
    int setvalue(int x,int y)
    {
        a=x;
        b=y;
    }
    int getvalue()
    {
        cout<<"First complesx number is "<<a<<" + "<<b<<"i"<<endl;
    }
    friend int result(one,two);
};

int result(one o1,two o2)
{

    cout<<"resultant complex number is "<<o1.a+o2.a<<" + "<<o1.b+o2.b<<"i"<<endl;

}
int main()
{
    one p;
    two q;
    p.setvalue(2,3);
    p.getvalue();
    q.setvalue(4,5);
    q.getvalue();
    result(p,q);

}