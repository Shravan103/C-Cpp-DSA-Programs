// Pure virtual functions
#include<iostream>
using namespace std;
class base                             //class base is used only to pass some of its traits(if any) to derived class
{                                      //and to achieve run time polymorphism                 
    protected:
    int base1;
    public:
    base(){ }
    void setbase()
    {
        base1 = 25;
    }
    virtual void display() = 0;
    // {                                                 NOT POSSIBLE TO GIVE DEFINITION TO PURE VIRTUAL FUNCTION
    //     cout<<"(BASE) base1 = "<<base1<<endl;
    // }
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
    // base b;                                      NOT POSSIBLE TO CREATE OBJECT OF ABSTRACT CLASS
    derived d;
    base * base_ptr = & d;
    base_ptr ->setbase();
    // base_ptr ->setderived(); not possible
    base_ptr -> display();
}