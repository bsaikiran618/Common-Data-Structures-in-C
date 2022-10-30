#include "AVLTree.h"
#include <stdlib.h>
#define max(a, b) ((a) > (b) ? (a) : (b))

AVLTreeNode * createAVLTreeNode(void * data)
{
	AVLTreeNode * newNode = (AVLTreeNode *) malloc(sizeof(AVLTreeNode));

	newNode->data = data;
	newNode->height = 0;

	newNode->parent = NULL;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

AVLTree * createAVLTree(int (*comparator)(void *, void *))
{
	AVLTree * newTree = (AVLTree *)malloc(sizeof(AVLTree));

	newTree->root = NULL;
	newTree->comparator = comparator;

	return newTree;
}

void destroyAVLTree(AVLTree * tree)
{
	__destroyAVLTree(tree->root);
}

static void __destroyAVLTree(AVLTreeNode *root)
{
	//recursively deletes the tree.
	if(!root) return;
	__destroyAVLTree(root->left);
	__destroyAVLTree(root->right);
	root->left = NULL;
	root->right = NULL;
	free(root);
}

int lookup(AVLTree * tree, void * data)
{
	AVLTreeNode * current = tree->root;

	while(current)
	{
		int compVal = tree->comparator(current->data, data);
		switch(compVal)
		{
			case 0: return 1;
			case 1:
				current = current->left;
				break;
			case -1:
				current = current->right;
				break;
		}
	}
	
	//element has not been found.
	return 0;
}

height_t getHeight(AVLTree *tree)
{
	if(!tree) return -1;
	else if(!(tree->root)) return -1;

	return tree->root->height;
}

void insert(AVLTree * tree, void * data)
{
	AVLTreeNode * newNode = createAVLTreeNode(data);

	//if the tree is empty.
	if(!(tree->root))
	{
		tree->root = newNode;
		return;
	}

	// Step 1) Insert the node at the correct position
	AVLTreeNode * current = tree->root;

	while(1)
	{
		int res = tree->comparator(current->data, data);

		if(res <= 0)
		{
			//go right
			if(current->right)
				current = current->right;
			else
			{
				newNode->parent = current;
				current->right = newNode;
				break;
			}
		}
		else
		{
			//go left
			if(current->left)
				current = current->left;
			else
			{
				newNode->parent = current;
				current->left = newNode;
				break;
			}
		}
	}

	/*
		Step 2) Update the parent heights by backtracking.
				Check for balancing problems.
	*/

	balanceTree(tree, newNode);
}
static void solveRR(AVLTree * tree, AVLTreeNode * A, AVLTreeNode * B)
{

	/*
		Either one of the following cases may occur.

          G               G     |  G               G
		 /               /      |   \               \
		A               B		|	 A               B		
	   / \     ==>     / \		|   / \    ==>      / \		.
	  X1  B           A   C	    |  X1  B           A   C	
		 / \         / \		|     / \         / \		.
		X2  C       X1 X2		| 	 X2  C       X1 X2		

	*/

	// B's old left child will be A's right child.
	A->right = B->left; 
	if(B->left)
		B->left->parent = A;

	//make A and left child of B
	B->left = A;

	//Update Child information in A's old parent.
	if(A->parent)
	{
		if(A->parent->left == A) A->parent->left = B;
		else if(A->parent->right == A) A->parent->right = B;
	}

	// A will become the left child of B.
	//update parent information in B and A.
	B->parent = A->parent;
	A->parent = B;

	//If A was the root, make B the new root.
	if(tree->root == A) tree->root = B;

	//update the heights after rotation, children first then parent.
	updateHeight(A);
	updateHeight(B);
}
static void solveLL(AVLTree * tree, AVLTreeNode * A, AVLTreeNode * B)
{
	/*
		Either one of the following cases may occur.

          G              G 
		 /              /  
        A              B      
       / \     ==>    / \
      B  X1          C   A
     / \                / \
    C  X2             X2  X1

	*/

	
	//B's old right child will be A's new right child.
	A->left = B->right; 
	if(B->right)
	B->right->parent = A;

	//Make A as the right child of B
	B->right = A;

	//Update child information in A's Old parent.
	if(A->parent)
	{
		if(A->parent->left == A) A->parent->left = B;
		else if(A->parent->right == A) A->parent->right = B;
	}

	// A will become the right child of B.
	//update parent information in A and C
	B->parent = A->parent;
	A->parent = B;

	//If A was the root, make B the new root.
	if(tree->root == A) tree->root = B;

	//update heights after rotation, children first, then parent.
	updateHeight(A);
	updateHeight(B);
}
static void toRR(AVLTreeNode * A, AVLTreeNode * B, AVLTreeNode * C)
{
	/*
        A          A
		 \          \
		  B          C
		 /    ==>     \
		C              B
		 \             /
		 X1           X1

	*/

	A->right = C;
	C->parent = A;

	B->left = C->right;
	if(C->right)
		C->right->parent = B;

	C->right = B;
	B->parent = C;
	
	//update the heights of B and C
	updateHeight(B);
	updateHeight(C);

	//There will be no need to update root because here there isnt a possibility of root changing.
}
static void toLL(AVLTreeNode * A, AVLTreeNode * B, AVLTreeNode * C)
{
	/*
        A      	     A
	   /       	    /
	  B        	   C
	   \    ==>	  /
		C      	 B
	   /      	  \ .  
	  X1      	  X1

	*/
	A->left = C;
	C->parent = A;
	
	B->right = C->left;
	if(C->left)
		C->left->parent = B;

	C->left = B;
	B->parent = C;
	
	//update the heights of B and C
	updateHeight(B);
	updateHeight(C);

	//There will be no need to update root because here there isnt a possibility of root changing.
}
static void updateHeight(AVLTreeNode * node)
{
	height_t leftHeight = node->left ? (node->left->height + 1) : 0;
	height_t rightHeight = node->right ? (node->right->height + 1) : 0;

	node->height = max(leftHeight, rightHeight);
}
static int isLeafNode(AVLTreeNode * node)
{
	return (!(node->left) && !(node->right));
}
void * delete(AVLTree * tree, void * data)
{
	AVLTreeNode * current = tree->root;

	while(current)
	{
		int compVal = tree->comparator(current->data, data);

		if(compVal == 0)
		{
			break;
		}
		else if(compVal < 0)
			current = current->right;
		else if (compVal > 0)
			current = current->left;
	}

	if(!current)
	{
		//node not found.
		return NULL;
	}

	void * dataToBeDeleted = current->data;

	//replace with inorder successor data;
	
	AVLTreeNode * succ = current->right;

	if(!succ)
	{
		//There is no RST. No succ.
		if(current == tree->root)
		{
			//if node was root.
			tree->root = current->left;
			if(current->left)
				current->left->parent = NULL;
			free(current);
			// NO NEED TO BALANCE.
		}
		else
		{
			//if this node was normal node, without an RST
			if(current->parent->left == current)
			{
				//set LC's parent as current's parent
				if(current->left)
				current->left->parent = current->parent;
				//set LC of parent as current's LC
				current->parent->left = current->left;
			}
			else
			{
				//set LC's parent as current's parent
				if(current->left)
				current->left->parent = current->parent;
				//set RC of parent as current's LC
				current->parent->right = current->left;
			}
			//balance starting from current's parent
			balanceTree(tree, current->parent);
			//delete the node.
			free(current);
		}
	}
	else
	{
		//succ exists.

		//find succ
		while(succ->left)
		{
			succ = succ->left;
		}

		if(succ == current->right)
		{
			//Right child is only the succ.
			current->data = succ->data;
			//make the RST of succ as the RST of current only.
			current->right = succ->right;
			//tell the succ's RST that current is the new parent.
			if(succ->right)
				succ->right->parent  = current;

			//BALANCE FROM Current;
			balanceTree(tree, current);
			//delete succ.
			free(succ);
		}
		else
		{
			current->data = succ->data;
			if(succ->parent)
				succ->parent->left = succ->right;
			if(succ->right)
				succ->right->parent = succ->parent;

			AVLTreeNode * temp = succ;

			// BALANCE FROM SUCC PARENT
			balanceTree(tree, succ->parent);
			free(temp);
		}
	}

	return dataToBeDeleted;
}

static height_t getBF(AVLTreeNode * node)
{
	height_t LH = (node->left ? node->left->height + 1 : 0);
	height_t RH = (node->right ? node->right->height + 1 : 0);

	return LH - RH;
}

static void balanceTree(AVLTree * tree, AVLTreeNode * start)
{
	AVLTreeNode * current = start;

	while(current)
	{
		//first update the height
		updateHeight(current);
		
		//check the BF
		height_t BF = getBF(current);

		if(BF < -1 || BF > 1)
		{
			//current is the critical node.
			//now we need to find the other 2 problematic nodes.
			AVLTreeNode * A, * B, * C;
			A = current;
			if(BF < 0)
			{
				//this is right heavy.
				//B is decided.
				B = A->right;
				if(getBF(B) < 0)
				{
					//B is right heavy
					C = B->right;
				}
				else
				{
					//B is left heavy.
					C = B->left;
				}
			}
			else
			{
				//this is left heavy.
				//B is decided
				B = A->left;
				if(getBF(B) < 0)
				{
					//B is right heavy
					C = B->right;
				}
				else
				{
					//B is left heavy.
					C = B->left;
				}
			}

			// TIME TO SOLVE PROBLEMS
			if(A->right == B && B->right == C)
			{
				//RR problem
				solveRR(tree, A, B);
				current = B;
			}
			else if(A->left == B && B->left == C)
			{
				//LL Problem
				solveLL(tree, A, B);
				current = B;
			}
			else if(A->right == B && B->left == C)
			{
				//RL Problem

				//first convert to RR
				toRR(A, B, C);
			
				//The order of B and C changes, so set them right.
				AVLTreeNode * temp = B;
				B = C;
				C = temp;

				solveRR(tree, A, B);
				current = B;
			}
			else if(A->left == B && B->right == C)
			{
				//LR Problem

				//first convert to LL
				toLL(A, B, C);

				//The order of B and C changes, set them right.
				AVLTreeNode * temp = B;
				B = C;
				C = temp;

				solveLL(tree, A, B);
				current = B;
			}
		}
		current = current->parent;
	}
}
