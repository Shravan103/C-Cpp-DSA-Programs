//Operator Overloading joining two strings(objects) & finding greatest string(object)
#include<iostream>
#include<string.h>
#include<stdlib.h>
using namespace std;
class string_c
{
    char a[25];
    public:
    void get()
    {
        cout<<"Enter the string : ";
        gets(a);
    }
    void show()
    {
        cout<<"string = "<<a<<endl;
    }
    string_c operator + (string_c s2)
    {
        string_c temp;
        strcpy(temp.a,a);
        strcat(temp.a," ");
        strcat(temp.a,s2.a);
        return temp;
    }
    friend int operator > (string_c s1,string_c s2)
    {
        int a = strlen(s1.a);
        int b = strlen(s2.a);
        if(a>b)
            return 1;
        else
            return 0;
    }
    friend int operator < (string_c s1,string_c s2)
    {
        int a = strlen(s1.a);
        int b = strlen(s2.a);
        if(a<b)
            return 1;
        else
            return 0;
    }
};

int main()
{
    string_c s1,s2,s3;
    s1.get();
    s1.show();
    s2.get();
    s2.show();
    s3 = s1 + s2;
    s3.show();
    if(s1>s2)
    {
        s1.show();
        cout<<" is greater than "<<endl;
        s2.show();
    }
    else if(s1<s2)
    {
        s1.show();
        cout<<" is smaller than "<<endl;
        s2.show();
    }
    else
    {
        s1.show();
        cout<<" is equal to "<<endl;
        s2.show();
    }
    return 0;
}