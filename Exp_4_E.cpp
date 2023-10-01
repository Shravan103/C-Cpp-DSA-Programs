#include<iostream>
using namespace std;
int num;
class vector
{
    public:
        int *ptr;
        void setvalue()
        {
            cout<<"Enter the number of vector elements to be created:";
            cin>>num;
            ptr = new int[num];
            for(int i=0;i<num;i++)
            {
                cout<<"Enter vector element number "<<i+1<<" : ";
                cin>>ptr[i];
            }    
        }
        void getvalue()
        {
            for(int i=0;i<num;i++)
            {
                cout<<"Vector element number "<<i+1<<" is : "<<ptr[i]<<endl;
            }
        }
        friend vector operator * (vector & obj1,int & k)
        {
            for(int i=0;i<num;i++)
                obj1.ptr[i] = obj1.ptr[i] * k;
        }
};

int main()
{
    int k;
    vector o1,o2;
    o1.setvalue();
    cout<<"--------------------------------"<<endl;
    o1.getvalue();
    cout<<"Enter the scalar value to be multiplied to each vector element:";
    cin>>k;
    o1 * k;
    cout<<"--------------------------------"<<endl;
    o1.getvalue();
}