 #include<iostream>
 using namespace std;
 class empl
 {
    char name[25];
    long long phone;
    public:
    int putdata();
    int display();
 };
    int empl :: putdata()
    {
        cout<<"Enter the name of employee : ";
        cin>>name;
        cout<<endl<<"Enter the phone number of employee : ";
        cin>>phone;
    }
    int empl :: display()
    {
        cout<<name<<endl<<phone<<endl;
    }
 int main()
 {
    // int * ptr1 = new int(45);
    // cout<<ptr1<<endl;
    // cout<<*(ptr1)<<endl;
    int n;
    cout<<"Enter the number of employee : ";
    cin>>n;
    empl * ptr = new empl[n];

    for(int i=0;i<n;i++)
    {
        cout<<"For employee "<<i+1<<endl;
        ptr[i].putdata();
    }
    for(int i=0;i<n;i++)
    {
        cout<<"DISPLAY"<<endl;
        cout<<"For employee "<<i+1<<endl;
        ptr[i].display();
    }
 }