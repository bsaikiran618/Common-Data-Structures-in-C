#include "BST.h"
#include <stdlib.h>

static int isLeafNode(TreeNode * node)
{
	return !node->left && !node->right;
}

TreeNode * createTreeNode(void * data)
{
	//this function creates a new node with the given data and returns the pointer to the new node.
	TreeNode * newNode = (TreeNode *) malloc(sizeof(TreeNode));

	//malloc() failed.
	if(!newNode) return NULL;

	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;
}

BST * createBST(int (*comparator)(void *, void *))
{
	BST * newBST = (BST *)malloc(sizeof(BST));
	newBST->root = NULL;
	newBST->comparator = comparator;
	return newBST;
}
void destroyBST(BST * tree)
{
	//calls the helper destroy function.
	__destroyBST(tree->root);
}
void insert(BST * tree, void * data)
{
	TreeNode *newNode = createTreeNode(data);

	/*
		case 0: Tree is empty, create the root.
		case 1: Tree is not empty, find the right place.
	*/

	//case 0
	if(!(tree->root))
	{
		tree->root = newNode;
		return;
	}
	//case 1
	else
	{
		TreeNode *current = tree->root;

		while(1)
		{
			if(tree->comparator(current->data, data) <= 0)
			{
				//if right exists, go right.
				if(current->right)
					current = current->right;
				//else add the child at the right.
				else
				{
					current->right = newNode;
					break;
				}
			}
			else
			{
				//if left exists, go left.
				if(current->left)
					current = current->left;
				//else add the child at the left
				else
				{
					current->left = newNode;
					break;
				}
			}
		}
	}
}

int lookup(BST * tree, void * data)
{
	TreeNode * current = tree->root;

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

static int __getHeight(TreeNode *root)
{
	//this is a helper function for the actual getHeight() function.
	if(!root) return 0;

	int leftHeight = __getHeight(root->left);
	int rightHeight = __getHeight(root->right);

	return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int getHeight(BST *tree)
{
	if(!tree) return 0;

	return __getHeight(tree->root);
}

void * delete(BST *tree, void * data)
{

	/*
		Step 1) Find the node.
				if found:
					if the node is a leaf node, just delete.
					else, go to next step.
				if the node is not found return 0; 
		Step 2) Find the inorder successor of the node
		Step 3) Replace the node to be delete with that node.
	*/

	/*

		case 1: node to be deleted is a leaf node.
				a) it is also the root.
				b) not the root.

		case 2: node to be deleted is not a leaf node.
				a) the node is root.
					i) inorder successor exists.
					ii)inorder successor doesnt exist.
				b) the node is not root.
					i) inorder successor exists.
					ii)inorder successor doesnt exist.
	*/

	//Step 1) First find the node to be deleted.

	TreeNode *current	= tree->root;
	TreeNode *parent	= NULL;

	while(current)
	{
		int compVal = tree->comparator(current->data, data);
		
		int flag = 0;
		switch(compVal)
		{
			case 0:
				flag = 1;
				break;
			case 1:
				parent = current;
				current = current->left;
				break;
			case -1:
				parent = current;
				current = current->right;
				break;
		}
		if(flag) break;
	}

	if(!current) return 0; //did not find the node.


	//case 1
	if(isLeafNode(current))
	{
		//case 1a
		if(tree->root == current) 
			tree->root = NULL;
		//case 1b
		else
		{
			// not a root, just delete and update parent.
			// if current was parent's left child,
			// update parent's left pointer.

			if(parent->left == current) 
				parent->left = NULL;
			else
				parent->right = NULL;
		}
	}
	//case 2
	else
	{
		// Step 2) find the inorder successor.

		TreeNode * inorderSuccessor = findInorderSuccessorAndDisconnect(current);
		//case 2 a ==> not leaf + root.
		if(current == tree->root)
		{
			//case 2 a i ==> not leaf + root + no successor.
			if(!inorderSuccessor)
			{
				tree->root = tree->root->left;
			}
			//case 2 a ii ==> not leaf + root + successor
			else
			{
				// inorder successor inherits children
				// of the node to be deleted.

				inorderSuccessor->left = current->left;

				//if the inordersuccessor is the right child
				//only, then we must not change its right children.
				if(inorderSuccessor != current->right)
					inorderSuccessor->right = current->right;

				tree->root = inorderSuccessor;
			}
		}
		//case 2 b ==> not leaf + not root.
		else
		{
			//case 2 b i ==> not leaf + not root + no successor
			if(!inorderSuccessor)
			{
				//if the node is not the root, and there is no successor,
				//just replace the node with its left child.
				if(parent->left == current)
					parent->left = current->left;
				else if(parent->right == current)
					parent->right = current->left;
			}
			//case 2 b ii ==> not leaf + not root + successor
			else
			{

				//inorder successor inherits children of the node to be deleted.
				inorderSuccessor->left = current->left;

				//if the inordersuccessor is the right child
				//only, then we must not change its right children.
				if(inorderSuccessor != current->right)
					inorderSuccessor->right = current->right;

				if(parent->left == current)
					parent->left = inorderSuccessor;
				else if(parent->right == current)
					parent->right = inorderSuccessor;
			}
		}
		
	}
	void * temp = current->data;
	free(current); //delete the node.
	return temp; //return the data.
}

static TreeNode * findInorderSuccessorAndDisconnect(TreeNode *node)
{
	/*
	  This function finds the inorder successor of the node
	  in its right subtree, i.e the smallest element in the
	  right subtree of the node. If the node does not have a right
	  subtree then it simply returns NULL.
	*/

	/*
		case 0: there is no right subtree.
		case 1: root of right subtree is the successor.
		case 2: successor is a leaf node.
		case 3: successor is not a leaf node.
	*/

	//case 0
	if(!node->right)
		return NULL;
	
	//go to the left most node of the right subtree of node.
	TreeNode * current = node->right;
	TreeNode * parent = node;

	while(current->left)
	{
		parent = current;
		current = current->left;
	}

	//case 1
	if(current == node->right)
		return current;
	//case 2
	if(isLeafNode(current))
	{
		//disconnect with the parent.
		if(parent->left == current)
			parent->left = NULL;
		else
			parent->right = NULL;
	}
	//case 3
	else
	{
		//connect the right subtree of the leftmost node to the parent of it.
		parent->left = current->right;
	}
	return current;
}

static void __destroyBST(TreeNode *root)
{
	//recursively deletes the tree.
	if(!root) return;
	__destroyBST(root->left);
	__destroyBST(root->right);
	root->left = NULL;
	root->right = NULL;
	free(root);
}
