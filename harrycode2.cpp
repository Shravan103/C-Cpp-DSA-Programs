#include<iostream>
using namespace std;
class operation
{
        int a;
        int b;
    public:
        void add(int v1,int v2)
        {
            a=v1;
            b=v2;
        }
        void addBoth(operation o1,operation o2)
        {
            //int p=0;
            //int q=0;
            /*p*/
                a=(o1.a) + (o2.a);
            /*q*/
                b=(o1.b) + (o2.b);
            //cout<<"p= "<<p<<endl;    
            //cout<<"q= "<<q<<endl;                
        }
        void getop()
        {

            cout<<"a="<<a<<"    b="<<b<<endl;
        }
        
};

int main()
{
    operation a1,a2,a3;
    //sum for first operation
    a1.add(1,2);
    a1.getop();
    //sum for second operation
    a2.add(3,4);
    a2.getop();
    //sum both (a1 and a2)
    a3.addBoth(a1,a2);
    a3.getop();

}