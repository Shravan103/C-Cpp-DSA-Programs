#include<stdio.h>
struct student
{
    char name[20];
    int roll;
    char address[100];

    struct marks
    {
        float phy;
        float chem;
        float bio;
    }m;

};

int main()
{
    int i,n;
    float perc;
    struct student s[50];
    printf("how many students are to be added?\n");
    scanf("%d",&n);
    printf("Enter the details of the students\n");
    for(i=0;i<n;i++)
    {
        printf("The name of the student %d is\n",i+1);
        scanf("%s",&s[i].name);
        printf("The roll number of student %d is\n",i+1);
        scanf("%d",&s[i].roll);
        printf("The address of the student %d is\n",i+1);
        scanf("%s",&s[i].address);
        printf("the marks of student %d for PHYSICS is \n",i+1);
        scanf("%f",&s[i].m.phy);
        printf("the marks of student %d for CHEMISTRY is \n",i+1);
        scanf("%f",&s[i].m.chem);
        printf("the marks of student %d for BIOLOGY is \n",i+1);
        scanf("%f",&s[i].m.bio);
        printf("the percentage of student %d is\n",i+1);
        perc=(s[i].m.phy + s[i].m.chem + s[i].m.bio)*0.666;
        printf("%f\n",perc);

        printf("The grade of the student is:\n");
        if(perc>95)
        printf("AO\n");
        else if(perc>=86 && perc<=95)
        printf("AA\n");
        else if(perc>=76 && perc<=85)
        printf("AB\n");
        else if(perc>=66 && perc<=75)
        printf("BB\n");
        else if(perc>=56 && perc<=65)
        printf("BC\n");   
    }

    
}
