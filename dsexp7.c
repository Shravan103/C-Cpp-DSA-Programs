#include<stdio.h>
#include<stdlib.h>
struct node
{
    struct node * lchild;
    int data;
    struct node * rchild;
};
struct node * create();
void preorder(struct node * root);
void inorder(struct node * root);
void postorder(struct node * root);
void levelorder(struct node * root);
int height(struct node * root);
void displaygivenlevel(struct node * root,int i);
int depth(struct node * root, int x);
int main()
{
    struct node * root = NULL;
    printf("Creating a tree....\n");
    root = create();
    printf("Preorder is : \n");
    preorder(root);
    printf("\nInorder is : \n");
    inorder(root);
    printf("\nPostorder is : \n");
    postorder(root);
    printf("\nLevel order is : \n");
    levelorder(root);
    printf("\nFinding the DEPTH....\n");
    printf("Enter the data who's depth is to be found : ");
    int x;
    scanf("%d",&x);
    int d = depth(root,x);
    printf("Depth of %d = %d\n",x,d);
}
int depth(struct node * root, int x)
{
    if(root == NULL)
        return -1;
    int dist = -1;
    if(root->data==x || (dist = depth(root->lchild,x))>=0 || (dist = depth(root->rchild,x))>=0)
        return dist+1;
    return dist;
}
void displaygivenlevel(struct node * root,int i)
{
    if(root == NULL)
        return;
    else if (i == 1)
        printf("%d  ",root->data);
    else if(i>1)
    {
        displaygivenlevel(root-> lchild,i-1);
        displaygivenlevel(root-> rchild,i-1);
    }
}
void levelorder(struct node * root)
{
    int h = height(root);
    printf("height = %d\n",h);
    for (int i = 1; i <= h; i++)
    {
        displaygivenlevel(root,i);
    }    
}
int height(struct node * root)
{
    int l_ht,r_ht;
    if (root == NULL)
    {
        return 0;
    }
    l_ht = height(root->lchild);
    r_ht = height(root->rchild);
    if(l_ht>r_ht)
        return 1+l_ht;
    else
        return 1+r_ht;
    
}
void postorder(struct node * root)
{
    if(root == NULL)
        return;
    postorder(root->lchild);
    postorder(root->rchild);
    printf("%d  ",root->data);
}
void inorder(struct node * root)
{
    if(root == NULL)
        return;
    inorder(root->lchild);
    printf("%d  ",root->data);
    inorder(root->rchild);
}
void preorder(struct node * root)
{
    if (root == NULL)
        return;
    printf("%d  ",root->data);
    preorder(root->lchild);
    preorder(root->rchild);
}
struct node * create()
{
    struct node * newnode;
    int x;
    newnode = (struct node*)malloc(sizeof(struct node));
    newnode->lchild = NULL;
    newnode->rchild = NULL;
    printf("\nEnter the data(-1 for no node) : ");
    scanf("%d",&x);
    if(x == -1)
    {
        return NULL;
    }
    newnode->data = x;
    printf("Enter left child of %d : ",x);
    newnode->lchild = create();
    printf("Enter right child of %d : ",x);
    newnode->rchild = create();
    return newnode;
}