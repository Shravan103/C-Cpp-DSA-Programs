#include<iostream>
using namespace std;
class sample
{
        int a;
    public:
        sample()
        {
            a=0;
            cout<<"Intial value of a :"<<endl;
        }
        void operator ++()
        {
            ++a;
        }
        void operator ++(int)
        {
            a++;
        }
        void operator --()
        {
            --a;
        }
        void operator --(int)
        {
            a--;
        }
        void display()
        {
            cout<<"a = "<<a<<endl;
        }
};

int main()
{
    sample s1;
    s1.display();
    cout<<"prefix increment(++ op) :"<<endl;
    ++s1;
    s1.display();
    cout<<"postfix increment(op++) :"<<endl;
    s1++;
    s1.display();
    cout<<"prefix decrement(-- op) :"<<endl;
    --s1;
    s1.display();
    cout<<"postfix decrement(op--) :"<<endl;
    s1--;
    s1.display();
    return 0;
}