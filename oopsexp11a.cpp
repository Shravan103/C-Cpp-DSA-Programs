//vectors
#include<iostream>
#include<vector>
using namespace std;
void display(vector <int> & v);
int main()
{
    vector<int> v;
    cout<<"Enter three elements : ";
    for (int i = 0; i < 3; i++)
    {
        int ele;
        cin>>ele;
        v.push_back(ele);
    }
    //cout<<endl;
    cout<<"size = "<<v.size()<<endl;
    v.push_back(40);
    display(v);
    cout<<"Front element = "<<v.front()<<endl;
    cout<<"Back element = "<<v.back()<<endl;

    vector<int> :: iterator itr = v.begin();
    v.insert(itr,69);
    display(v);
}
void display(vector <int> & v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout<<v.at(i)<<"   ";
    }
    
}