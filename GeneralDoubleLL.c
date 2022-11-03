#include "GeneralDoubleLL.h"
#include <stdlib.h>

Node * createNode (void * data)
{
	Node *newNode = (Node *) malloc(sizeof(Node));
	if(!newNode) return NULL;

	newNode->data = data;
	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

List * createList()
{
	List *newList = (List *)malloc(sizeof(List));
	newList->head = NULL;
	newList->tail = NULL;
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
	list->tail = NULL;
	list->size = 0;

	free(list);

	return 0;
}

int addFirst(List *list, void * data)
{
	if(!list) return -2;

	Node *newNode = createNode(data);
	if(!newNode) return -1;
	
	if(!listIsEmpty(list)) list->head->prev = newNode;
	newNode->next = list->head;
	list->head = newNode;

	if(!list->tail) list->tail = list->head;
	(list->size)++;

	//this works for empty list also.

	return 0;
}

int addLast(List *list, void * data)
{
	if(!list) return -2;
	//check if list is empty.
	if(listIsEmpty(list))
		return addFirst(list, data);
	else
	{
		Node *newNode = createNode(data);
		if(!newNode) return -1;

		list->tail->next = newNode;
		newNode->prev = list->tail;
		list->tail = newNode;

		(list->size)++;
	}
	return 0;
}

int addAtIndex(List *list, int pos, void * data)
{
	if(!list) return -2;
	else if(pos > list->size || pos < 0) return -1;
	//index = list->size is allowed in this.
	else if(pos == list->size) return addLast(list, data);
	else if(pos == 0) return addFirst(list, data);
	else
	{
		Node *newNode = createNode(data);
		if(!newNode) return -1;

		Node *current = list->head;

		for(int i=0;i<pos-1;i++)
			current = current->next;

		newNode->next = current->next;
		newNode->prev = current;

		if(current->next)
			current->next->prev = newNode;
			
		current->next = newNode;

		(list->size)++;
	}

	return 0;
}

void * removeFirst(List *list, int *status)
{
	if(!list)
	 {
		*status = -2;
	 	return NULL;
	 }
	if(listIsEmpty(list))
	{
		*status = -1;
		return NULL;
	}
	else
	{
		void * tempData  = list->head->data;

		Node *temp = list->head->next;
		free(list->head);
		list->head = temp;
		if(list->head) list->head->prev = NULL;
		(list->size)--;

		if(list->head == NULL) list->tail = NULL;
		
		*status = 0;
		return tempData;
	//works for list with single element too;
	}
}

void * removeLast(List *list, int *status)
{
	if(!list)
	{
		*status = -2;
		return NULL;
	}
	if(listIsEmpty(list))
	{
		*status = -1;
		return NULL;
	}
	else if(list->size == 1)
		return removeFirst(list, status);
	else
	{
		Node *temp = list->tail;
		void * tempData = temp->data;

		if(list->tail->prev)
			list->tail->prev->next = NULL;
		list->tail = list->tail->prev;
		free(temp);

		(list->size)--;
		*status = 0;
		return tempData;
	}
}

void * removeAtIndex(List *list, int pos, int *status)
{
	if(!list)
	{
		*status = -3;
		return NULL;
	}
	else if(listIsEmpty(list))
	{
		*status = -2;
		return NULL;
	}
	else if(pos < 0 || pos >= list->size)
	{
		*status = -1;
		return NULL;
	}
	else if(pos == 0) return removeFirst(list, status);
	else if(pos == list->size-1) return removeLast(list, status);

	Node *current = list->head;

	for(int i=0;i<pos-1;i++)
		current = current->next;

	void * tempData = current->next->data;
	
	Node *temp = current->next;
	current->next = current->next->next;
	if(temp->next)
		temp->next->prev = current;

	(list->size)--;
	
	free(temp);
	
	*status = 0;
	return tempData;
}

int listIsEmpty(List *list)
{
	return list->head == NULL;
}
