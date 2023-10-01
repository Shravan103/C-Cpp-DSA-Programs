//virtual functions
#include<iostream>
using namespace std;
class base
{
    protected:
    int base1;
    public:
    base(){ }
    void setbase()
    {
        base1 = 25;
    }
    virtual void display()
    {
        cout<<"(BASE) base1 = "<<base1<<endl;
    }
};
class derived : public base
{
    protected:
    int derived1 = 50;
    public:
    derived(){ }
    void setderived()
    {
        derived1 = 50;
    }
    void display()
    {
        cout<<"(DERIVED) base1 = "<<base1<<endl;
        cout<<"(DERIVED) derived1 = "<<derived1<<endl;
    }
};
int main()
{
    base b;
    derived d;
    base * base_ptr = & d;
    base_ptr ->setbase();
    //base_ptr ->setderived(); //not possible
    base_ptr -> display();
}