#include <iostream>
using namespace std;
class overload
{
    public:
    // function with 2 parameters
    int display(int var1, double var2)
    {
        cout << "Integer number: " << var1;
        cout << " and double number: " << var2 << endl;
    }

    // function with double type single parameter
    double display(double var)
    {
        cout << "Double number: " << var << endl;
    }

    // function with int type single parameter
    int display(int var)
    {
        cout << "Integer number: " << var << endl;
    }
};

int
main()
{
    overload o1,o2,o3;
    int a = 5;
    double b = 5.5;

    // call function with int type parameter
    o1.display(a);

    // call function with double type parameter
    o2.display(b);

    // call function with 2 parameters
    o3.display(a, b);

    return 0;
}