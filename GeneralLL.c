#include "GeneralLL.h"
#include <stdlib.h>

Node * createNode (void * data)
{
	Node *newNode = (Node *) malloc(sizeof(Node));
	if(!newNode) return NULL;

	newNode->data = data;
	newNode->next = NULL;

	return newNode;
}

List * createList()
{
	List *newList = (List *)malloc(sizeof(List));
	newList->head = NULL;
	newList->size = 0;

	return newList;
}

int destroyList( List * list )
{
	if(!list) return -2;
	if(!list->head)
		return -1;
	
	Node *currentNode = list->head;
	Node *temp = NULL;

	while(currentNode)
	{
		temp = currentNode->next;
		free(currentNode);
		currentNode = temp;
	}
	list->head = NULL;
	list->size = 0;
	free(list);

	return 0;
}

int addBegin(List *list, void * data)
{
	if(!list) return -2;
	Node *newNode = createNode(data);
	if(!newNode) return -1;
	newNode->next = list->head;
	list->head = newNode;
	(list->size)++;

	//this works for empty list also.

	return 0;
}

int addEnd(List *list, void * data)
{
	if(!list) return -2;
	//check if list is empty.
	if(!list->head)
	{
		return addBegin(list, data);
	}
	else
	{
		Node *newNode = createNode(data);
		if(!newNode) return -1;

		Node *current = list->head;
		while(current->next != NULL) current = current->next;
		current->next = newNode;

		(list->size)++;
	}
	return 0;
}

int addPos(List *list, int pos, void * data)
{
	if(!list) return -2;
	else if(pos > list->size-1 || pos < 0) return -1;
	//check if list is empty
	else if(!list->size)
	{
		if(pos != 0) return -1; //index out of bounds.
		else
		{
			return addBegin(list, data);
		}
	}
	else
	{
		Node *newNode = createNode(data);
		if(!newNode) return -1;

		Node *current = list->head;

		for(int i=0;i<pos-1 && current;i++)
			current = current->next;

		newNode->next = current->next;
		current->next = newNode;

		(list->size)++;
	}

	return 0;
}

int removeBegin(List *list)
{
	if(!list) return -2;
	if(list->size == 0) return -1;
	else
	{
		Node *temp = list->head->next;
		free(list->head);
		list->head = temp;
		(list->size)--;

		return 0;
	//works for list with single element too;
	}
}

int removeEnd(List *list)
{
	if(!list) return -2;
	if(!list->size) return -1;
	else if(list->size == 1)
		return removeBegin(list);
	else
	{
		Node *current = list->head;
		while(current->next->next) current = current->next;

		free(current->next->next);

		current->next = NULL;
		(list->size)--;

		return 0;
	}
}

int removePos(List *list, int pos)
{
	if(!list) return -2;
	else if(!list->size) return -1;
	else if(list->size==1) return removeBegin(list);

	Node *current = list->head;

	for(int i=0;i<pos-1;i++)
		current = current->next;
	
	Node *temp = current->next;
	current->next = temp->next;

	(list->size)--;

	free(temp);

	return 0;
}

int listIsEmpty(List *list)
{
	return list->head == NULL;
}
