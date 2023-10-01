#include <iostream>
using namespace std;
class employee
{
    static int count;

public:
    static void countData(void)
    {
        count++;

        cout << "The count of the employee is " << count << endl;
    }
};

int employee ::count;

int main()
{
    employee data;
    employee::countData(); // only can be called if countData is static member function.
    data.countData();
    data.countData();
}