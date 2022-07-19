#include <stdio.h>
#include <stdlib.h>

// Tree traversal in C
/* Below is tree structure
         1
    12       9
5       6   
     10       
 
inorder -> left - root -right
           5 12 6 1 9
preorder ->  root - left -right
            1 12 5 6 9
postorder -> left -  right -root
            5 6 12 9 1
*/

typedef struct Node {
  int item;
  struct Node* left;
  struct Node* right;
}node;

/*create new Node */
node* createNode(int value){
    node* newNode=(node*) malloc(sizeof(node));
    newNode->item=value;
    newNode->left=NULL;
    newNode->right=NULL;
    
    return newNode;
}

void inorderTraversal(node* root){
    if(root==NULL)
       return;
    
    //printf("\nDEBUG right now I am here: %d\n", root->item);
    inorderTraversal(root->left);
    printf("%d -> ", root->item);
    inorderTraversal(root->right);
}

void preorderTraversal(node* root){
    if(root==NULL)
       return;
    
    printf("%d -> ", root->item);
    preorderTraversal(root->left);
    preorderTraversal(root->right);   
}

void postorderTraversal(node* root){
    if(root==NULL)
       return;
    
    postorderTraversal(root->left);
    postorderTraversal(root->right);  
    printf("%d -> ", root->item);
}

/* post order used, as to delete root node at last */
void deleteTree(node* root){
	if(root==NULL)
		return;

	deleteTree(root->left);
	deleteTree(root->right);
	free(root);
}

/* calculate height of tree,
   first iterate over left side, then on right side
   check the max number */
int calcHeight(node* root){
     if(!root)
        return 0;

    int left=calcHeight(root->left);

    int right=calcHeight(root->right);
    // printf("\nnode=%d, left=%d, right=%d", root->item, left, right);
     if(left>right) 
        return left+1;
     else 
        return right+1;
}

int main()
{
    printf("--Tree, creating nodes\n");
    node* root=createNode(1);
    root->left=createNode(12);
    root->right=createNode(9);
    root->left->left=createNode(5);
    root->left->right=createNode(6);
    root->left->right->left=createNode(10);

    int h=calcHeight(root);
    printf("\nHeight of Tree = %d", h);
    
  printf("\nInorder traversal \n");
  inorderTraversal(root);

  printf("\nPreorder traversal \n");
  preorderTraversal(root);

  printf("\nPostorder traversal \n");
  postorderTraversal(root);

   /* delete tree nodes */
   printf("\nDeleting Tree");
   deleteTree(root);
         
    return 0;
}
