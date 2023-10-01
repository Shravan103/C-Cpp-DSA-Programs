#include <iostream>
#include <string.h>
using namespace std;
int num = 0;
class string1
{
    char st[50];

public:
    void read()
    {
        cout << "Enter the string " << ++num << ":";
        cin >> st;
    }
    void show()
    {
        cout << "string: " << st;
    }
    string1 operator+(string1 &t2)
    {
        string1 temp;
        strcpy(temp.st, st);
        strcat(temp.st, " ");
        strcat(temp.st, t2.st);
        return temp;
    }
    friend int operator<(string1 &s1, string1 &s2)
    {
        int m = strlen(s1.st);
        int n = strlen(s2.st);
        if (m < n)
            return (1);
        else
            return (0);
    }
    friend int operator==(string1 &s1, string1 &s2)
    {
        int m = strlen(s1.st);
        int n = strlen(s2.st);
        if (m == n)
            return (1);
        else
            return (0);
    }
};

int main()
{
    string1 s1, s2, s3;
    s1.read();
    // s1.show();
    s2.read();
    // s2.show();
    s3 = s1 + s2;
    cout << "After addition ";
    s3.show();
    cout << endl;
    if (s1 == s2)
    {
        s1.show();
        cout << "   is of same length as    ";
        s2.show();
        exit(1);
    }
    if (s1 < s2)
    {
        s1.show();
        cout << "   is less than    ";
        s2.show();
    }
    else
    {
        s1.show();
        cout << "   is more than    ";
        s2.show();
    }
}