#include<iostream>
using namespace std;
class cube
{
    float l;
    public:
    float setlen(float x)
    {
        l=x;
    }
    friend inline float volume(cube o1);
};

inline float volume(cube o1)
{
    return (o1.l*o1.l*o1.l);
}

int main()
{
    cube p;
    float result,len;
    cout<<"Enter the length of the side of the cube :"<<endl;
    cin>>len;
    p.setlen(len);
    result= volume( p );
    cout<<"volume of the cube is "<<result<<endl;
}