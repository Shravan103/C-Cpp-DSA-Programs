//overloading unary and binary operators using member function and friend function method
#include<iostream>
using namespace std;
class sample
{
    int a;
    public:
    sample()
    {
        a = 1;
    }
    sample(int b)
    {
        a = b;
    }
    int display()
    {
        cout<<"a = "<<a<<endl;
    }
    // unary by member function
    // void operator ++(int)
    // {
    //     a++;
    // }
    // unary by friend function
    // friend int operator ++ (sample & o1, int x)  //generally one argument but here is an exception.                                             
    // {                                            //for pre increment only one argument(no need of dummy)
    //     o1.a = o1.a + 1;                        
    // }


    //binary by member function
    // sample operator + (sample & z)
    // {
    //     sample temp;
    //     temp.a = a + z.a;
    //     return temp;
    // }
    //binary by friend function
    // friend sample operator + (sample & y , sample & z)
    // {
    //     sample temp;
    //     temp.a = y.a + z.a;
    //     return temp;
    // }
};
int main()
{
    // A] unary overloading
    // sample o1;
    // o1++;
    

    // B] binary overloading
    // sample x,y(5),z(10);
    // x = y + z;
    // x.display();
}