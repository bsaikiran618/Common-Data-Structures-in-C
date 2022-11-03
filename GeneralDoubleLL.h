#ifndef GENERAL_DLL
#define GENERAL_DLL

typedef struct Node {
	void *data;
	struct Node *next, *prev;
} Node;

typedef struct List {
	Node *head, *tail;
	int size;
} List;

//Node functions

Node * createNode(void *);

//List functions
List * createList();
int addFirst( List *, void *);
int addLast( List *, void *);
int addAtIndex( List *, int, void *);


void * removeFirst(List *, int *);
void * removeLast(List *, int *);
void * removeAtIndex(List *, int, int * );

int listIsEmpty(List *);

int destroyList( List * );

#endif
