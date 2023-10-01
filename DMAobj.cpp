#include <iostream>
#include <stdio.h>
#include <conio.h>
#include<stdlib.h>
using namespace std;

class book
{
    char bname[20];
    char aname[20];

public:
    void getbook()
    {
        cout << "Enter the name of the book" << endl;
        fflush(stdin);
        gets(bname);
        cout << "Enter the name of the author of the book" << endl;
        fflush(stdin);
        gets(aname);
    }

    void showbook()
    {
        cout << "The name of the book is: " << bname << endl;
        cout << "The name of the author of the book is: " << aname << endl;
    }
};
int main()
{
    int n, i;
    book *b;
    cout << "Enter the number of books to be stored: ";
    cin >> n;
    b = new book[n];
    cout << "Enter the Books Data you want to store" << endl;
    for (i = 0; i < n; i++)
    {
        b[i].getbook();
    }
    cout << "Entered book names are: " << endl;
    for (i = 0; i < n; i++)
    {
        b[i].showbook();
    }
    return 0;
}