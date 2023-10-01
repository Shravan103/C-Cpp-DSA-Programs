#include <iostream>
using namespace std;
class base
{
public:
    virtual void print()
    {
        cout << "printing base class\n";
    }
    void show()
    {
        cout << "showing base class\n";
    }
};
class derived : public base
{
public:
    void print()
    {
        cout << "printing derived class\n";
    }
    void show()
    {
        cout << "showing derived class\n";
    }
};
int main()
{
    base *bptr;
    derived d;
    bptr = &d;
    bptr->print();//run time
    bptr->show();//compile time
    return 0;
}
