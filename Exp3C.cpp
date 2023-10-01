#include <iostream>
using namespace std;
class Demo
{
    int num1, num2;

public:
    Demo(int n1, int n2)
    {
        cout << "CONSTRUCTOR CALLED!!" << endl;
        num1 = n1;
        num2 = n2;
    }
    ~Demo()
    {
        cout << "DESTRUCTOR CALLED!!" << endl;
    }
    void display()
    {
        cout << "num1 = " << num1 << endl
             << "num2 = " << num2 << endl;
    }
};
int main()
{
    Demo obj1(10, 20);
    obj1.display();
    {
        Demo obj2(30, 40);
        obj1.display();
    }
    return 0;
}