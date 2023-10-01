#include <iostream>
using namespace std;
class Base1
{
    int data1;

public:
    Base1() {
        cout << "Base1 class constructor called default" << endl;
    }
    Base1(int i)
    {
        data1 = i;
        cout << "Base1 class constructor called" << endl;
    }
    void printDataBase1(void)
    {
        cout << "The value of data1 is " << data1 << endl;
    }
};

class Base2
{
    int data2;

public:
    Base2() {
        cout << "Base2 class constructor called default" << endl;
    }
    Base2(int i)
    {
        data2 = i;
        cout << "Base2 class constructor called" << endl;
    }
    void printDataBase2(void)
    {
        cout << "The value of data2 is " << data2 << endl;
    }
};

class Derived : public Base2, public Base1
{
    int derived1, derived2;

public:
    Derived () { }
    // Derived(int x, int y)
    // {
    //     derived1 = x;
    //     derived2 = y;
    // }
    // Derived(int a, int b, int c, int d) : Base2(b), Base1(a)
    // {
    //     derived1 = c;
    //     derived2 = d;
    //     cout << "Derived class constructor called" << endl;
    // }
    void printDataDerived(void)
    {
        cout << "The value of derived1 is " << derived1 << endl;
        cout << "The value of derived2 is " << derived2 << endl;
    }
};
int main()
{
    Base1 o1(5);
    o1.printDataBase1();
    Derived d;
    // Derived d(1,2);
    // d.printDataDerived();
    // Derived obj(100, 150, 200, 250);
    // obj.printDataBase1();
    // obj.printDataBase2();
    // obj.printDataDerived();
    return 0;
}