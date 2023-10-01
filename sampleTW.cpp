#include <iostream>
#include <list>

using namespace std;

int main()
{

    list<int> myList;

    myList.push_front(1);
    myList.push_front(2);
    myList.push_front(3);
    myList.push_back(4);
    myList.push_back(5);

    cout << "Original list: ";
    for (int x : myList)
    {
        cout << x << " ";
    }
    cout << endl;

    list<int> anotherList;
    anotherList.push_back(6);
    anotherList.push_back(7);
    anotherList.push_back(8);

    myList.splice(myList.end(), anotherList);

    cout << "After splice: ";
    for (int x : myList)
    {
        cout << x << " ";
    }
    cout << endl;

    myList.sort();

    cout << "After sort: ";
    for (int x : myList)
    {
        cout << x << " ";
    }
    cout << endl;

    list<int> yetAnotherList;
    yetAnotherList.push_back(9);
    yetAnotherList.push_back(10);
    yetAnotherList.push_back(11);

    myList.merge(yetAnotherList);

    cout << "After merge: ";
    for (int x : myList)
    {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}
