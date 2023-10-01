#include<iostream>
using namespace std;
class op
{
    char name[35];
    public:
    friend void operator >> (istream & in, op & s)
    {
        in>>s.name;
    }
    friend void operator << (ostream & out, op & s)
    {
        out<<s.name; 
    }
};

int main()
{
    op s;
    cin>>s;
    cout<<s;
}