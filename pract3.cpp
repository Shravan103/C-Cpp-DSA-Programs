// #include<iostream>
// using namespace std;
//parameterized constructor
// class para
// {
//     int data1;
//     public:
//     para(int a)
//     {
//         data1 = a;
//         cout<<"data1 = "<<data1<<endl;
//     }
// };
// int main()
// {
//     int j;
//     cout<<"Enter the data : ";
//     cin>>j;
//     para obj1(j);
// }

//copy constructor
// class copy1
// {
//     int data2;
//     public:
//     copy1()
//     {
//         data2 = 0;
//     }
//     copy1(int a)
//     {
//         data2 = a;
//     }
//     copy1(copy1 & obj_ref)
//     {
//         data2 = obj_ref.data2;
//     }
//     int display()
//     {
//         cout<<data2<<endl;
//     }
// };
// int main()
// {
//     copy1 o1(50);
//     copy1 o2(o1);
//     cout<<"object o1"<<endl;
//     o1.display();
//     cout<<"object o2"<<endl;
//     o2.display();
// }

//destructors