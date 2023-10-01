#include<iostream>
#include<stdlib.h>
using namespace std;
class demo
{
        int arrsize;
        int * arr;
    public:
        void get_in();
        void output();
};
void demo::get_in()
{
    arrsize=5;
    arr=(int*)malloc(arrsize*sizeof(int));
    for(int i=0;i<arrsize;i++)
    {
        arr[i]=rand()%50;
    }
}
void demo::output()
{
    cout<<"Array content is"<<endl;
    for(int i=0;i<arrsize;i++)
    cout<<arr[i]<<endl;
}
int main()
{
    demo * ptr;
    int n;
    srand(0);
    cout<<"Enter a number: ";
    cin>>n;
    ptr=new demo[n];
    for(int i=0;i<n;i++)
        ptr[i].get_in();
    for(int i=0;i<n;i++)
    {    cout<<"object"<<i<<" ";
        ptr[i].output();
        cout<<endl;
    }
}