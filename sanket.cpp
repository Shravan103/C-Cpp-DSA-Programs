#include <iostream>
#include <string>
#include <string.h>
#include <conio.h>
using namespace std;
int x, X;
int O = 0;
int Ot = 0;
int i;
int n;
char T;
int Salary;
int Fee;
static int trainer_no = 0;
int customer_no = 0;

class people
{
public:
    char name[100];
    int age;
    int num;

    void getdata_person()
    {
        cout << "\t\t\twhat is your name: ";
        cin >> name;
        cout << "\t\t\twhat is your age: ";
        cin >> age;
        cout << "\t\t\twhat is your phone number : ";
        cin >> num;
    }
    void showdata_person()
    {

        cout << "\n\t\t\tname : " << name << endl;
        cout << "\t\t\tAge : " << age << endl;
        cout << "\t\t\tPhone number: " << num << endl;
    }
};

class customer : public people
{
public:
    char pos[100] = "\t\t\tCustomer";
    int fees;
    int memebership_time;
    customer()
    {
        cout << "\t\t\tEntering Customer......";
        system("CLS");
    }
    void getdata_customer()
    {
        cout << "\t\t\tWhat is your monthly fee: ";
        cin >> fees;
        cout << "\t\t\tsubcribtion time : ";
        cin >> memebership_time;
        customer_no++;
    }

    void dispaly_customer()
    {
        cout << "\t\t\tYour yearly fees are : Rs " << fees << endl;
        cout << "\t\t\tSubcribtion time : " << memebership_time << "years";
    }
    void num_customer()
    {
        cout << "\t\t\t Total number of customer are: " << customer_no << endl;
    }
};

class trainer : public people
{
public:
    char pos[100] = "\t\t\tTrainer";

    int yearlys;
    int salary;
    int emp_time;
    trainer()
    {
        cout << "\t\t\tEntering Trainer......";
        system("CLS");
    }
    void getdata_trainer()
    {
        cout << "\t\t\tsalary: ";
        cin >> salary;
        cout << endl;
        cout << "\t\t\temp time : ";
        cin >> emp_time;
        cout << endl;
        trainer_no++;
    }

    void calc()
    {
        yearlys = salary * emp_time;
    }
    void dispaly_trainer()
    {
        cout << "\t\t\tyour yearly salary are : " << yearlys << endl;
        cout << "\t\t\temployment time : " << emp_time << " year" << endl;
    }
    void num_trainers()
    {
        cout << "\t\t\t Total number of trainers are: " << trainer_no << endl;
    }
};

int take(trainer t[], int n)
{
    for (int i = 0; i < n; i++)
    {
        t[i].getdata_person();
        t[i].getdata_trainer();
    }
}
int display(trainer t[],int n)
{
    for (int i = 0; i < n; i++)
    {
        t[i].showdata_person();
        t[i].calc();
        t[i].dispaly_trainer();
        t[i].num_trainers();
    }
}
int main()
{
    customer c[100];
    trainer t[100];
    int k = 0, k1, k2;
    cout << "\t\t\t=============== GYM DATA SHEET =============== ";
    do
    {

        cout << "\n\t\t\tSelect \n\t\t\t1 For Trainer \n\t\t\t2 For Customer \n\t\t\t3 Exit \n\t\t\tchoice: ";
        cin >> k;
        switch (k)
        {
        case 1:
        {

            cout << "\n\t\t\t select \n\t\t\t1 for Put Data \n\t\t\t2 for display \n\t\t\tchoice:";
            cin >> k1;
            if (k1 == 1)
            {

                cout << "\n\t\t\tEnter number of trainers : ";
                cin >> n;
                take(t, n);
            }
            else if (k1 == 2)
            {
                display(t,n);
            }
            else
            {
                cout << "\t\t\tInvalid entry";
            }
            break;
        }
        case 2:
        {

            cout << "\n\t\t\tselect \n\t\t\t1 for Put Data \n\t\t\t2 for display \n\t\t\tchoice:";
            cin >> k1;
            if (k1 == 1)
            {
                cout << "Enter number of customers : ";
                cin >> n;

                for (int i = 0; i < n; i++)
                {
                    c[i].getdata_person();
                    c[i].getdata_customer();
                }
            }
            else if (k1 == 2)
            {
                for (int i = 0; i < n; i++)
                {
                    c[i].showdata_person();
                    c[i].dispaly_customer();
                    c[i].num_customer();
                }
            }
            else
            {
                cout << "\t\t\tInvalid entry";
            }
            break;
        }
        case 3:
        {
            cout << "\t\t\tExit";
            exit(0);
            break;
        }
        default:
            cout << "\t\t\tInvalid Choice...";
            break;
        }
    } while (k != 3);
}
