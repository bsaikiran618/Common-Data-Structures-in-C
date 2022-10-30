#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

typedef struct TreeNode {
	
	void *data;

	//the children of the tree
	struct TreeNode *left;
	struct TreeNode *right;	

} TreeNode;

typedef struct BST {
	TreeNode *root;
	
	int (*comparator)(void *, void *);
	/*
		Comparison is a key operation in BST.
		A comparing function needs to be provided to the tree
		to help compare non-primitive types.

		the comparator has to return:
			0 for equal
			1 for data1 > data2;
		   -1 for data1 < data2;
	*/

} BST;

//TreeNode functions;
TreeNode * createTreeNode (void * );
// hiddden functions
static int __getHeight(TreeNode *);
static void __destroyBST(TreeNode *);
static TreeNode * findInorderSuccessorAndDisconnect(TreeNode *);
static int isLeafNode(TreeNode *);

// visible functions
BST * createBST();
void destroyBST(BST *);
void insert(BST *, void *);
void * delete(BST * tree, void * data);
int lookup(BST *, void *);
int getHeight(BST *);
int printSorted(BST *);

#endif
