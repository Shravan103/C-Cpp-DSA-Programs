// Merge sort
#include <stdio.h>
#define MAX 100
char temp[MAX];
char A[MAX];
int MergeSort(int lb, int ub);
int Merge(int lb, int mid, int ub);
int main()
{
    int n;
    //char A[MAX];
    printf("Enter the size:");
    scanf("%d", &n);
    // fflush(stdin);
    for (int i = 0; i < n; i++)
    {
        printf("Enter Element %d : ", i + 1);
        scanf("%c", &A[i]);
        getchar();
        // fflush(stdin);
    }
    MergeSort(0, n - 1);
    printf("After merge sort :\n");
    for (int i = 0; i < n; i++)
    {
        printf("%c   ", temp[i]);
    }
}
int MergeSort(int lb, int ub)
{
    //printf("In MergeSort\n");
    int mid;
    if (lb < ub)
    {
        mid = (lb + ub) / 2;
        MergeSort(lb, mid);
        MergeSort(mid + 1, ub);
        Merge(lb, mid, ub);
    }
}
int Merge(int lb, int mid, int ub)
{
    //printf("In merge\n");
    int i = lb;
    int j = mid + 1;
    int k = lb;
    while ((i <= mid) && (j <= ub))
    {
        if (A[i] <= A[j])
        {
            temp[k] = A[i];
            i++;
        }
        else
        {
            temp[k] = A[j];
            j++;
        }
        k++;
    }
    if (i > mid)
    {
        while (j <= ub)
        {
            temp[k] = A[j];
            j++;
            k++;
        }
    }
    else if (j>ub)
    {
        while (i <= mid)
        {
            temp[k] = A[i];
            i++;
            k++;
        }
    }
    //printf("AFTER MERGE LOOPS\n");
    for (int i = lb; i <= ub; i++)
    {
        //printf("%c  ", temp[i]);
        A[i] = temp[i];
    }
}