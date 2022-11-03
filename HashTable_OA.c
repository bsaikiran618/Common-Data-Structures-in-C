#include <string.h>
#include <stdlib.h>
#include "HashTable_OA.h"

HashTable * createHashTable(table_size_t (*hashFunction)(HashTable *, void *), int (*comparator)(void *, void *))
{
	HashTable * newHT = (HashTable *) malloc(sizeof(HashTable));
	
	newHT->comparator = comparator;
	newHT->hashFunction = hashFunction;
	newHT->numberOfKeys = 0;
	newHT->size = INITIAL_TABLE_SIZE;
	
	//creating an array of void pointers.
	newHT->table = (void **) malloc(sizeof(void *) * INITIAL_TABLE_SIZE);
	//initialize the table to all NULLs
	memset(newHT->table, 0, sizeof(void *) * INITIAL_TABLE_SIZE);

	return newHT;
}

void destroyHashTable(HashTable * ht)
{
	//first deallocate the table.
	free(ht->table);
	//deallocate the ht
	free(ht);
}

void _rehash(HashTable *ht)
{
	//create a new array that is double the size of the current table
	table_size_t oldSize = ht->size;
	table_size_t newSize = oldSize * EXTENSION_FACTOR;
	void **oldTable = ht->table;

	void **newTable = (void **)malloc(sizeof(void *) * newSize);
	memset(newTable, 0, sizeof(void *) * newSize);

	ht->table = newTable;
	ht->numberOfKeys = 0;
	ht->size = newSize;

	for(table_size_t index = 0; index < oldSize; index++)
	{
		if(oldTable[index] != NULL && oldTable[index] != SEARCH_CONTINUE)
		{
			insert(ht, oldTable[index]);
		}
	}

	//free the old table.
	free(oldTable);
}

int insert(HashTable * ht, void * data)
{
	/*
		first check the load factor of the hash table.
		if the lf is more than or equal to the threshold,
		increase the hash table size.
	*/

	double loadFactor = (ht->numberOfKeys + 1) / ((double)(ht->size));

	if(loadFactor > LOAD_FACTOR_THRESHOLD)
	{
		_rehash(ht);
	}

	table_size_t index = _iterateTable(ht, data, HT_INSERT_MODE);
	if(index >= 0)
	{
		ht->table[index] = data;
		ht->numberOfKeys++;
		return 0;
	}
	return -1;
}

int lookup(HashTable * ht, void * data)
{
	table_size_t index = _iterateTable(ht, data, HT_LOOKUP_MODE);
	if(index < 0)
		return 0;
	return 1;
}

static table_size_t _iterateTable(HashTable * ht, void * data, int mode)
{
	/*
		This function is a general function that traverses
		the table and does some operation based on the mode value.
	*/
	table_size_t initialIndex = ht->hashFunction(ht, data);
	table_size_t counter = 0;
	int compVal;

	for(table_size_t counter = 0; counter < ht->size; counter++)
	{
		table_size_t currentIndex = (initialIndex + counter) % ht->size;

		//There are 3 types of values that can be found: NULL, SEARCH_CONTINUE or some valid address.
		if(ht->table[currentIndex] == NULL)
		{
			switch(mode)
			{
				case HT_LOOKUP_MODE:
					return -1; // element to lookup was not found
				case HT_INSERT_MODE: 
					//found a free slot for the element to be inserted
					return currentIndex;
				case HT_DELETE_MODE:
					return -1; //element to be deleted was not found
			}
		}
		else if(ht->table[currentIndex] == SEARCH_CONTINUE)
		{
			switch(mode)
			{
				case HT_LOOKUP_MODE:
					//we have to continue searching for the lookup element
					continue;
				case HT_INSERT_MODE:
					//we have found a free slot for the element to be inserted.
					return currentIndex;
				case HT_DELETE_MODE:
					//we have to continue searching for the element to be deleted.
					continue;
			}
		}
		else
		{
			// there is a valid address at this index.
			compVal = ht->comparator(ht->table[currentIndex], data);
			switch(mode)
			{
				case HT_LOOKUP_MODE:
					//we have to compare the element at this index to the lookup element
					if(compVal == 0)
					{
						//we found the element
						return currentIndex;
					}
				case HT_INSERT_MODE:
					// check if the data is the same as the one we are trying to insert.
					//else, we have to further search for the next free slot.

					if(compVal == 0)
					{
						//we found the element
						return -1;
					}
				case HT_DELETE_MODE:
					// we have to compare this element to the data to be deleted
					if(compVal == 0)
					{
						//we found the element to be deleted.
						return currentIndex;
					}
			}
		}
	}
	
	//we exited the loop, now again the return value depends on the mode.
	switch(mode)
	{
		case HT_LOOKUP_MODE:
			//did not find the lookup element.
			return -1;
		case HT_INSERT_MODE:
			//insert failed. Actually insert cannot fail.
			return -1;
		case HT_DELETE_MODE:
			//did not find the element to be deleted.
			return -1;
	}
}

void  * delete(HashTable * ht, void * data)
{
	table_size_t index = _iterateTable(ht, data, HT_DELETE_MODE);
	if(index < 0)
	{
		return NULL;
	}
	else
	{
		void * temp = ht->table[index];
		ht->table[index] = SEARCH_CONTINUE;
		ht->numberOfKeys--;
		return temp;
	}
}
