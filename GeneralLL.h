#ifndef GENERAL_LL
#define GENERAL_LL

typedef struct Node {
	void *data;
	struct Node *next;
} Node;

typedef struct List {
	Node *head;
	int size;
} List;

//Node functions

Node * createNode(void *);

//List functions
List * createList();
int addBegin( List *, void *);
int addEnd( List *, void *);
int addPos( List *, int, void *);

int removeBegin(List *);
int removeEnd(List *);
int removePos(List *, int);

int listIsEmpty(List *);

int destroyList( List * );

#endif
