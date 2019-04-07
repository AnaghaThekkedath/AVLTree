#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define new_max(x,y) ((x) >= (y)) ? (x) : (y)
#define left(node) node->left
#define right(node) node->right
#define parent(node) node->parent



// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; //key of this item
	int  value;  //value (int) of this item 
	int height; //height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left; //pointer to left child
	struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
	int  size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0; // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
	AVLTree *T;
	T = malloc(sizeof (AVLTree));
	assert (T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}

int height(AVLTreeNode *root){
    if (root==NULL){
        return -1;
    }
    int h = new_max(height(root->left),height(root->right));
    return h + 1;
}

bool IsTreeBalanced(AVLTreeNode *root){
    
    if (root==NULL){
        return true;
    }
    if (IsTreeBalanced(left(root)) && IsTreeBalanced(right(root))){
        return abs(height(left(root))-height(right(root)))<=1; 
    }
    return false;
}

AVLTreeNode *RotateLeft(AVLTreeNode *root){
    if(root==NULL||root->right==NULL){
        return;
    }
    AVLTreeNode *new = root->right;
    root->right = new->left;
    new->left = root;
    new->parent = root->parent;
    root->parent = new;
    if(root->right) root->right->parent = root;
    return new;
}

AVLTreeNode *RotateRight(AVLTreeNode *root){
    if(root==NULL||root->left==NULL){
        return;
    }
    AVLTreeNode *new = left(root);
    root->left = new->right;
    new->right = root;
    new ->parent = root->parent;
    root->parent = new;
    if(root->left) root->left->parent = root; 
    return new;
}


AVLTreeNode *InsertRec(AVLTreeNode *root, int k, int v){
    if (root->key==k && root->value==v){
        return NULL;
    }
    if((root->key>k)||((root->key==k)&&(root->value>v))){
        if (root->left==NULL){
            AVLTreeNode *newNode = newAVLTreeNode(k,v);
            root->left = newNode;
            newNode->parent=root;
            return newNode; 
        } 
        return InsertRec(root->left,k,v);
    }
    if((root->key<k)||((root->key==k)&&(root->value<v))){
        if (root->right==NULL){
            AVLTreeNode *newNode = newAVLTreeNode(k,v);
            root->right = newNode;
            newNode->parent=root;
            return newNode; 
        } 
        return InsertRec(root->right,k,v);
    }
}

AVLTree *read_from_file(const char *filename, AVLTree *T){
	FILE *txf;
	int key, value;
	txf = fopen(filename, "r");
    assert(txf!=NULL);
    while (fscanf(txf,"%*c%d%*c%d%*c",&key,&value)!=EOF){
        InsertNode(T,key, value);
    }
	fclose(txf);
	return T;
}

AVLTree *read_from_stdin(AVLTree *T){
    char *input=NULL;
	unsigned int inputSize = 50;
    FILE *temp;
    temp = fopen("new","w+");
	while(getline(&input,&inputSize,stdin)!=1)
    {	
        fputs(input,temp);
    }
    fclose(temp);
    return read_from_file("new",T);
}


// For Each node Insertion, O(log n)
// Complexity: O(n log n)
AVLTree *CreateAVLTree(const char *filename)
{
	AVLTree *myTree = newAVLTree();
	if (strcmp(filename,"stdin")!=0)
		return read_from_file(filename, myTree);
	else
    return read_from_stdin(myTree);
}

AVLTreeNode *CloneAVLTreeNode(AVLTreeNode *root){
	if (root == NULL){
		return root;
	}
	AVLTreeNode *newRoot = newAVLTreeNode(root->key, root->value);
	newRoot -> left = CloneAVLTreeNode(root->left);
	newRoot -> right = CloneAVLTreeNode(root->right);
	return newRoot; 
}

// Time Complexity: Number of elements in the tree
// Complexity: O(n)
AVLTree *CloneAVLTree(AVLTree *T)
{
  AVLTree *newTree = newAVLTree();
	newTree->root = CloneAVLTreeNode(T->root);
	newTree->size=T->size;
	return newTree;
}



void connectToParent(AVLTreeNode *parent, AVLTreeNode *child){
	if((child->key<parent->key)||((child->key==parent->key)&&(child->value<parent->value))){
		parent->left = child;
        child->parent = parent;
	}
	if((child->key>parent->key)||((child->key==parent->key)&&(child->value>parent->value))){
		parent->right = child;
        child->parent = parent;
	}
}


void disconnectFromParent(AVLTreeNode *parent, AVLTreeNode *child){
    if((child->key<parent->key)||((child->key==parent->key)&&(child->value<parent->value))){
		parent->left = NULL;
	}
	if((child->key>parent->key)||((child->key==parent->key)&&(child->value>parent->value))){
		parent->right = NULL;
	}
}

// Time complexity for balancing: O(log n)
void BalanceTree(AVLTree *T, AVLTreeNode *current){
    while(current){
        if(!IsTreeBalanced(current)){
            if(height(left(current))>height(right(current))){
                if(height(left(left(current)))>height(right(left(current)))){ //LL
                    current = RotateRight(current);
                } else { 
                    current->left = RotateLeft(current->left);
                    current = RotateRight(current);
                }
            } else {
                if(height(left(right(current)))>height(right(right(current)))){ //RL
                    current->right = RotateRight(current->right);
                    current = RotateLeft(current);
                } else { //RR
                    current = RotateLeft(current);
                } 
            }
            if(parent(current)==NULL) T->root = current;
            else connectToParent(parent(current),current);
        } 
        current = parent(current);
    } 
}

// Time complexity analysis for InsertNode():
// Time complexity for inserting as leaf, O(log n) +
// Time complexity for balancing, O(log n)
// So Complexity: O(log n)
int InsertNode(AVLTree *T, int k, int v)
{
    if(T->root == NULL){
        T->root = newAVLTreeNode(k,v);
        return 1;
    }
    AVLTreeNode *newNode = InsertRec(T->root,k,v);
    if (newNode == NULL){
        return 0;
    }
    T->size++;
    AVLTreeNode *current = parent(newNode);
    BalanceTree(T,current);
    return 1;
}


// Will have to traverse the height of the tree, 
// Complexity: O(height) = O(log n)
AVLTreeNode *Search_Node(AVLTreeNode *root, int k, int v){
	if (root!=NULL){
		if(root->key == k && root->value == v){	
			return root;
		}
		if((root->key > k)||(root->key == k && root->value > v)){
			return Search_Node(root->left, k, v);
		}
		if((root->key < k)||(root->key == k && root->value < v)){
			return Search_Node(root->right,k,v);
		}
	}
    return root;
}

// Will have to traverse the height of the tree, 
// Complexity: O(height) = O(log n)
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
	if(T==NULL || T->root==NULL){
		return NULL;
	}
  return Search_Node(T->root,k,v);
} 

AVLTreeNode *joinTree(AVLTreeNode *t1, AVLTreeNode *t2){
	AVLTreeNode *parent=NULL,*cur;
	cur = t2;
	if (t1==NULL){
		return t2;
	}
	if (t2==NULL){
		return t1;
	}
	while(cur->left){
		parent = cur;
		cur = cur->left;
	}
	if (parent!=NULL){
		parent->left = cur ->right; 
    if(parent->left) parent(parent->left) = parent;
		cur -> right = t2;
	} 
	cur -> left = t1;
	return cur;
}

// Time complexity analysis for DeleteNode():
// Time complexity for Searching, O(log n) +
// Time complexity for balancing, O(log n)
// So Complexity: O(log n)
int DeleteNode(AVLTree *T, int k, int v)
{
	AVLTreeNode *node = Search(T,k,v);
	//not found
	if (node == NULL){
		return 0;
	}
    AVLTreeNode *parent = node->parent;
	//leaf node
	if (node->left==NULL&&node->right==NULL){
        if (parent!=NULL) disconnectFromParent(node->parent,node);
		free(node);
        BalanceTree(T,parent);
		return 1;
	}
	if (node->left==NULL){
		if (parent!=NULL) connectToParent(node->parent,node->right);
        else { parent(node->right)=NULL; T->root = node->right; }
		free(node);
        BalanceTree(T,parent);
		return 1;
	}
	if (node->right==NULL){
		if (parent!=NULL) connectToParent(node->parent,node->left);
        else { parent(node->left)=NULL; T->root = node->left; }
		free(node);
        BalanceTree(T,parent);
		return 1;
	}
	if (parent != NULL) connectToParent(node->parent, joinTree(node->left,node->right));
    else T->root = joinTree(node->left,node->right);
	free(node);
    BalanceTree(T,parent);
	return 1;
}

void RecTreeUnion(AVLTree *newTree, AVLTree *T1, AVLTreeNode *t2){
    if(t2==NULL){
        return;
    }
    AVLTreeNode *found = Search(T1, t2->key, t2->value);
    if (!found){
        InsertNode(newTree,t2->key,t2->value);  
    } 
    RecTreeUnion(newTree, T1,t2->left);
    RecTreeUnion(newTree, T1, t2->right);
}
 
// Complexity of CloneTree,O(n) + Search of elements t2,O(m log n) +
// Insertion of m element, O(m log n+m)
// Complexity: O(m log n+m)
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
    if (T1==NULL || T1->root == NULL){
        return T2;
    }
    if (T2==NULL || T2->root == NULL){
        return T1;
    }
	AVLTree *newTree = CloneAVLTree(T1);
    RecTreeUnion(newTree,T1,T2->root);
    return newTree;
}

void RecTreesIntersection(AVLTree *new, AVLTree *T1, AVLTreeNode *t2){
    if (t2==NULL){
        return;
    }
    AVLTreeNode *found = Search(T1, t2->key, t2->value);
    if (found){
        InsertNode(new,t2->key,t2->value);  
    }
    RecTreesIntersection(new,T1,t2->left);
    RecTreesIntersection(new,T1,t2->right);
}
 
// Complexity of Search of elements t2,O(m log n) +
// Insertion of m element, O(m log m)
// Complexity: O(m log m)
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
	if (T1==NULL || T1->root == NULL){
        return NULL;
    }
    if (T2==NULL || T2->root == NULL){
        return NULL;
    }
	AVLTree *newTree = newAVLTree();
    RecTreesIntersection(newTree, T1, T2->root);
    return newTree;
}


void freeNode(AVLTreeNode *TN){
	if (TN!=NULL){
		freeNode(TN->left);
		freeNode(TN->right);
		free(TN);
	}
}

// Each node is freed,
// Complexity: O(n)
void FreeAVLTree(AVLTree *T)
{
	freeNode(T->root);
	free(T);
}

void PrintNode(AVLTreeNode *root){
	if(root!=NULL){
		PrintNode(root->left);
		printf("(%d,%d)%d\n", root->key, root->value, height(root));
		PrintNode(root->right);
	}
}

// Each node is Printed,
// Complexity: O(n)
void PrintAVLTree(AVLTree *T)
{
 PrintNode(T->root);
}




int main() //sample main for testing 
{ int i,j;
 AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
 AVLTreeNode *node1;
 
 tree1=CreateAVLTree("stdin");
 PrintAVLTree(tree1);
 FreeAVLTree(tree1);
 //you need to create the text file file1.txt
 // to store a set of items without duplicate items
 tree2=CreateAVLTree("file1.txt"); 
 PrintAVLTree(tree2);
 tree3=CloneAVLTree(tree2);
 PrintAVLTree(tree3);
 FreeAVLTree(tree2);
 FreeAVLTree(tree3);
 //Create tree4 
 tree4=newAVLTree();
 j=InsertNode(tree4, 10, 10);
 for (i=0; i<15; i++)
  {
   j=InsertNode(tree4, i, i);
   if (j==0) printf("(%d, %d) already exists\n", i, i);
  }
  PrintAVLTree(tree4);
  node1=Search(tree4,20,20);
  if (node1!=NULL)
    printf("key= %d value= %d\n",node1->key,node1->value);
  else 
    printf("Key 20 does not exist\n");
  
  for (i=17; i>0; i--)
  {
    j=DeleteNode(tree4, i, i);
	if (j==0) 
	  printf("Key %d does not exist\n",i);  
    PrintAVLTree(tree4);
  }
 FreeAVLTree(tree4);
 //Create tree5
 tree5=newAVLTree();
 j=InsertNode(tree5, 6, 25);
 j=InsertNode(tree5, 6, 10);
 j=InsertNode(tree5, 6, 12);
 j=InsertNode(tree5, 6, 20);
 j=InsertNode(tree5, 9, 25);
 j=InsertNode(tree5, 10, 25);
 PrintAVLTree(tree5);
 //Create tree6
 tree6=newAVLTree();
 j=InsertNode(tree6, 6, 25);
 j=InsertNode(tree6, 5, 10);
 j=InsertNode(tree6, 6, 12);
 j=InsertNode(tree6, 6, 20);
 j=InsertNode(tree6, 8, 35);
 j=InsertNode(tree6, 10, 25);
 PrintAVLTree(tree6);
 tree7=AVLTreesIntersection(tree5, tree6);
 tree8=AVLTreesUnion(tree5,tree6);
 PrintAVLTree(tree7);
 PrintAVLTree(tree8);
 return 0; 
}
