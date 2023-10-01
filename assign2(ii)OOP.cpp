#include<stdio.h>
#include<stdbool.h>
#define true 1
#define false 0
struct Pw
{
	int p, w;
};
typedef struct Pw PW;
int Search_01(int l, int h, int pp, int ww, PW pair[])
{
	int low = l, high = h;

	while(low<=high)
	{
		 
		int mid = (high+low)/2;

		 
		if(pair[mid].p==pp && pair[mid].w==ww)
		{
			  
			return 1;
		}
		else if(pair[mid].w<ww)
		{
			low = mid+1;
		}
		else
		{
			high = mid-1;
		}
	} 
	return 0;
}

void Traceback(int p[], int w[], PW pair[], int x[], int b[], int m, int n)
{
	int last = b[n+1] - 1;
	int t = n;
	int pp = pair[last].p;
	int ww = pair[last].w; 

	while(pp>0 && ww>0)
	{
		bool f = 1;

		for(int j=t; j>=0; j--)
		{
			f = Search_01(b[j], b[j+1]-1, pp, ww, pair);
			if(!f)
			{
				if(j!=n)
				{
					x[j+1] = 1;
					pp-=p[j+1];
					ww-=w[j+1];
				}
				else
				{
					x[j] = 1;
					pp-=p[j];
					ww-=w[j];
					
				}
				t = j;
			 
			}
		} 
	}
}

int Largest(PW pair[], int w[], int t, int h, int i, int m)
{
	int r, low = t, high = h;

	while(low<=high)
	{
		int mid = (low+high)/2;

		if((pair[mid].w + w[i])<=m)
		{
			r = mid;
			low = mid+1;
		 
		}
		else
		{
			high = mid-1;
		}
	}
	return r;
}

void DKnap(int p[], int w[], int x[], int n, int m)
{
	int b[100];
	PW pair[100];
	
	for(int i=1; i<=n; i++)
	{
		printf("\n[pi,wi] = [%d,%d]\n",p[i],w[i]);
	}
	for(int x=0; x<=n; x++)
	{
		pair[x].p = pair[x].w = 0.0;
	}

	pair[0].p = pair[0].w = 0.0;
	b[0] = 1;
	int t, h, next;
	t = h = 1;
	b[1] = next = 2;

	for(int i=1; i<=n; i++)
	{
		int k = t;
		int u = Largest(pair, w, t, h, i, m);

		for(int j=t; j<=u; j++)
		{
			int pp = pair[j].p + p[i];
			int ww = pair[j].w + w[i];

			while((k<=h) && (pair[k].w<=ww))
			{
			
				pair[next].p = pair[k].p;
				pair[next].w = pair[k].w;
				next++; 
                k++;
			}

			if((k<=h) && (pair[k].w==ww))
			{
				if(pp<pair[k].p)
				{
					pp = pair[k].p;
				}
				k++;
			}

			if(pp>pair[next-1].p)
			{
				pair[next].p = pp;
				pair[next].w = ww;
				next++;
			}

			while((k<=h) && (pair[k].p<=pair[next-1].p))
			{
				k++;
			}
		}

		while(k<=h)
		{
			pair[next].p = pair[k].p;
			pair[next].w = pair[k].w;
			next++; 
            k++;
		}

		t = h+1;
		h = next-1;
		b[i+1] = next;
	}
    
	printf("State Sets = \n");
	for(int i=0; i<=n; i++)
	{
		printf("\nState %d {",i);
		for(int j=b[i]; j<=b[i+1]-1; j++)
		{
			printf("( %d , %d )",pair[j].p,pair[j].w);

			if(j==b[i+1]-1)
			{  
				printf(")");
			}
			else
			{ 
				printf("), ");
			}
		}
		printf("}");
	} 
	Traceback(p, w, pair, x, b, m, n);
}
int main()
{
	int profit=0, weight=0;
	int m, n;
	printf("Enter Object Numbers[n]: ");
	scanf("%d",&n);
	printf("Enter sack size[m]: ");
	scanf("%d",&m);
	int p[100], w[100];
	int x[100];
	printf("\nEnter profits :\n");
	for(int i=1; i<=n; i++)
	{
		printf("P[%d]: ",i);
		scanf("%d",&p[i]);
	}
	printf("\nEnter weights : \n");
	for(int i=1; i<=n; i++)
	{
		printf("W[%d]: ",i);
		scanf("%d",&w[i]);
		x[i] = 0;
	}
	DKnap(p, w, x, n, m);
    printf("\nSolution Vector: (");
	for(int i=1; i<=n; i++)
	{ 
		profit+=p[i]*x[i];
		weight+=w[i]*x[i];
		printf("%d",x[i]);
		if(i<n)
		{
			printf(",");
		}
	}
	printf(")\n");
	printf("\nMAX Profit = %d \n",profit);
	printf("Weight Occupied = %d ",weight);
    
	return 0;
}
