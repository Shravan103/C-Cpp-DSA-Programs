// #include<iostream>
// using namespace std;
//reference variables
// class ref
// {
//     int refdata;
//     public:
//     void setdata(int & k)
//     {
//         refdata = k;
//         cout<<"refdata = "<<refdata<<endl;
//         k = 100;
//     }
// };
// int main()
// {
//     ref obj;
//     cout<<"Enter a number : ";
//     int num;
//     cin>>num;
//     cout<<"number = "<<num<<endl;
//     obj.setdata(num);
//     cout<<"number = "<<num<<endl;
// }

//friend function
// class sample
// {
//     int sample1;
//     public:
//     sample(){ } //default constructor
//     sample(int a);
//     friend int display(sample );
// };
// sample :: sample(int a)
// {
//     sample1 = a;
// }
// int display(sample obj)
// {
//     cout<<"sample1 = "<<obj.sample1<<endl;
//     obj.sample1 = 100;
//     cout<<"sample1 = "<<obj.sample1<<endl;
// }

// int main()
// {
//     sample o1(50);
//     display(o1);
// }

//inline function
// class sampline
// {
//     int var;
//     public:
//     sampline () { }
//     sampline(int j)
//     {
//         var = j;
//     }
//     inline int sum (sampline & o1, sampline & o2);
// };
//     int sampline :: sum (sampline & o1, sampline & o2)
//     {
//         return ((o1.var) + (o2.var));
//     }
// int main()
// {
//     sampline o1(70);
//     sampline o2(30);
//     sampline o3;
//     cout<<"sum = "<<o3.sum(o1,o2);
// }

//function overloading
// class func
// {
//     public:
//     inline float area(float side)
//     {
//         return (side*side);
//     }
//     inline float area(float l,float b)
//     {
//         return (l*b);
//     }
// };
// int main()
// {
//     func ob;
//     cout<<"area = "<<ob.area(3.00,5.00);
// }