//Graphs - insert edge & insert vertex & display & find vertex.
#include <stdio.h>
#include <stdlib.h>
struct vertex
{
    struct vertex *nextvertex;
    int info;
    struct edge *firstedge;
};
struct edge
{
    struct vertex *destvertex;
    struct edge *nextedge;
};
struct vertex *start = NULL;
void create_vtx(int u);
void add_edge(int u,int v);
struct vertex * find(int x);
void display();
int main()
{
    int u,origin,destin;
    int n = 3;
    for (int i = 0; i < n; i++)
    {
        printf("Enter the vertex %d : ",i+1);
        scanf("%d", &u);
        create_vtx(u);
    }
    for (int j = 0; j < n*(n-1); j++)
    {
        printf("Enter edge (-1 -1 for no edge) %d  : ",j+1);
        scanf("%d %d", &origin,&destin);
        if(origin==-1 && destin==-1)
            break;
        add_edge(origin,destin);
    }
    display();
    return 0;
}
void display()
{
    struct vertex * temp = start;
    struct edge * ptr;
    while (temp !=NULL)
    {
        printf("[ %d ] ---->    ",temp->info);
        ptr = temp->firstedge;
        while (ptr!=NULL)
        {
            printf("{ %d }  ",ptr->destvertex->info);
            ptr = ptr->nextedge;
        }
        printf("\n");
        temp = temp->nextvertex;
    }
    
}
struct vertex * find(int x)
{
    struct vertex * temp;
    temp = start;
    while (temp!=NULL)
    {
        if (temp->info == x)
        {
            return temp;
        }
        temp = temp->nextvertex;
    }
    return NULL;
}
void add_edge(int u,int v)
{
    struct vertex * locu, *locv;
    struct edge * newnode, * temp;
    locu = find(u);
    locv = find(v);
    if(locu == NULL)
    {
        printf("Invalid start vertex\n");
        return;
    }
    if(locv == NULL)
    {
        printf("Invalid end vertex\n");
        return;
    }
    newnode = (struct edge*)malloc(sizeof(struct edge));
    newnode->nextedge = NULL;
    newnode->destvertex = locv;
    if(locu->firstedge == NULL)
    {
        locu->firstedge = newnode;
        return; 
    }
    else
    {
        temp = locu->firstedge;
        while (temp->nextedge != NULL)
        {
            temp = temp->nextedge;
        }
        temp->nextedge = newnode;
    }
}
void create_vtx(int u)
{
    struct vertex *newnode = (struct vertex *)malloc(sizeof(struct vertex));
    newnode->nextvertex = NULL;
    newnode->info = u;
    newnode->firstedge = NULL;
    if (start == NULL)
    {
        start = newnode;
        return;
    }
    struct vertex *temp = start;
    while (temp->nextvertex != NULL)
    {
        temp = temp->nextvertex;
    }
    temp->nextvertex = newnode;
}