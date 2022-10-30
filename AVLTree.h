#ifndef AVL_TREE
#define AVL_TREE

#define height_t long int

typedef struct AVLTreeNode {
	
	void *data;
	//parent of the node.
	struct AVLTreeNode *parent;
	//the children of the tree
	struct AVLTreeNode *left;
	struct AVLTreeNode *right;
	//height of the node.
	height_t height;

} AVLTreeNode;

typedef struct AVLTree {
	AVLTreeNode *root;
	
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
} AVLTree;

//TreeNode functions;
AVLTreeNode * createAVLTreeNode (void * );

// implementation specific functions.
static void __destroyAVLTree(AVLTreeNode *);
static AVLTreeNode * findInorderSuccessorAndDisconnect(AVLTreeNode *);
static int isLeafNode(AVLTreeNode *);

static void solveRR(AVLTree *, AVLTreeNode *, AVLTreeNode *);
static void solveLL(AVLTree *, AVLTreeNode *, AVLTreeNode *);

static void toLL(AVLTreeNode *, AVLTreeNode *, AVLTreeNode *);
static void toRR(AVLTreeNode *, AVLTreeNode *, AVLTreeNode *);
static void updateHeight(AVLTreeNode *);
static void balanceTree(AVLTree * , AVLTreeNode * );
static height_t getBF(AVLTreeNode * );

// visible functions
AVLTree * createAVLTree(int (*comparator)(void *, void *));
void insert(AVLTree *, void *);
int lookup(AVLTree *, void *);
void * delete(AVLTree *, void *);
height_t getHeight(AVLTree *);
void destroyAVLTree(AVLTree *);

#endif
