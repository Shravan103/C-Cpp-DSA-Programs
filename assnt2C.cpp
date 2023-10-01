#include <iostream>
using namespace std;
class election
{
private:
static int votes[5];
static int sp;
public:
void count(int num);
void display();
};
int election::votes[5]={0,0,0,0,0};
int election :: sp=0;
void election :: display()
{
for(int i=0;i<5;i++)
{
cout<<"\nVotes of candidate "<<i+1<<" is : "<<votes[i];
}
cout<<"\nspoiled ballot votes ="<<sp;
}
void election :: count(int num)
{
if(num>=1 && num<=5)
votes[num-1]++;
else{
sp++;
}
}
int main()
{
election c[5];
int n,num;
cout << "\nEnter the count of voters : ";
cin >> n;
for (int i = 0; i < n; i++)
{
cout << "\nEnter the candidate's number : ";
cin>>num;
c[i].count(num);
}
c[0].display();
return 0;
}