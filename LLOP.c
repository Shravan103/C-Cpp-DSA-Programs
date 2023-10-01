#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 30
int a[MAX];
int temp[MAX];
void merge_sort(int low, int high);
void merge(int low, int mid, int high);
int binarySearch(int low, int high, int x);
int main()
{
	int i, n, key;

	printf("Enter size of the array:");
	scanf("%d", &n);

	printf("Elements below:\n");
	for (i = 0; i < n; i++)
	{
		scanf("%d", &a[i]);
	}

	int low = 0, high = n - 1;
	printf("In merge:");
	merge_sort(low, high);
	printf("Applying merge sort on the array:\n");

	for (i = 0; i < n; i++)
	{
		printf("%d\t", a[i]);
	}

	return 0;
}

void merge_sort(int low, int high)
{

	int mid;

	if (low < high)
	{
		mid = (low + high) / 2;
		merge_sort(low, mid);
		merge_sort(mid + 1, high);
		merge(low, mid, high);
	}
}

void merge(int low, int mid, int high)
{
	int i = low;
	int j = mid + 1;
	int k = low;

	while ((i <= mid) && (j <= high))
	{
		if (a[i] <= a[j])
		{
			temp[k] = a[i];
			i++;
			k++;
		}
		else
		{
			temp[k] = a[j];
			j++;
			k++;
		}
	}
	if (i > mid)
	{
		while (j <= high)
		{
			temp[k] = a[j];
			j++;
			k++;
		}
	}
	else if (j > high)
	{
		while (i <= mid)
		{
			temp[k] = a[i];
			i++;
			k++;
		}
	}
	for (int i = low; i <= high; i++)
	{
		a[i] = temp[i];
	}
}