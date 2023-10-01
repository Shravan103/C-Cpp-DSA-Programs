#include <iostream>
using namespace std;
class color
{
protected:
    int a;

public:
    void get()
    {
        cout << "Enter a value: ";
        cin >> a;
    }
};

class red : virtual public color
{
protected:
    int b;

public:
    void get1()
    {
        cout << "Enter b value: ";
        cin >> b;
    }
};

class green : virtual public color
{
protected:
    int c;

public:
    void get2()
    {
        cout << "Enter c value: ";
        cin >> c;
    }
};

class yellow : public red, public green
{
public:
    void display()
    {
        get();
        get1();
        get2();
        cout << "Multiplication value is " << a * b * c << endl;
    }
};

int main()
{
    yellow a;
    a.display();
    return 0;
}