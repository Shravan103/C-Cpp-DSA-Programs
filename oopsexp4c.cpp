// program on overloading insertion and extraction operator
#include <iostream>
using namespace std;
class sample
{
    char name[20];

public:
    friend istream & operator>>(istream & in, sample & obj)
    {
        in >> obj.name;
        return in;
    }
    friend ostream & operator<<(ostream & out, sample & obj)
    {
        out << obj.name;
        return out;
    }
};

int main()
{
    sample s;
    cin>>s;
    cout<<s;
}