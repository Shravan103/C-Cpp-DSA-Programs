#include<iostream>
using namespace std;
class Number
{
    int a;
    public:
        Number(int x)
        {
            a=x;
        }
        Number(Number & obj)
        {
            a=obj.a;
        }
        int display();
};
int Number::display()
{
    cout<<"The value of a = "<<a<<endl;
}
int main()
{
    //parameterised constructor
    Number obj1(45);
    cout<<"The value of 'a' for obj1 is "<<endl;
    obj1.display();
    //copy constructor
    Number obj2(obj1);
    cout<<"The value of 'a' for obj2 is "<<endl;
    obj2.display();
}