//files
#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<string.h>
using namespace std;
int main()
{
    //using constructors
    // string st;
    char s[20];
    // ofstream out("oops.txt");  //.txt format is necessary
    // printf("Enter the data : ");
    // scanf("%s",&s);
    // out<<s;

    // out<<"hello there"<<endl;
    // out<<"i am shravan";
    // out.close();

    ifstream in ("oops.txt");
    // // in>>st1>>st2;
    // // cout<<st1<<st2;
    while (!in.eof())
    {
        in.getline(s,20);       //use when using char type 'variable'
        cout<<s<<endl;
    }
    // while (!in.eof())               
    // {
    //     getline(in,st);         //use when using string type 'object'
    //     cout<<st<<endl;
    // }
    


    //using open()
    // char p[20];
    // printf("Enter the data : ");
    // //scanf("%s",&p);
    // gets(p);
    // ofstream out;
    // out.open("oops.txt");
    // out<<p;

    // char sp[20];
    // cout<<"Enter sting : ";
    // gets(sp);
    // int len = strlen(sp);
    // ofstream out;
    // out.open("oops6.txt");
    // out<<sp;
    // out.write((char*) &sp , len);
    // out.write(sp,len);
    
}