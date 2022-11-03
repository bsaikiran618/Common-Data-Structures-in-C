#ifndef _SK_HASH_TABLE
#define _SK_HASH_TABLE

//A custom type for the table size.
#define table_size_t long long int

#define INITIAL_TABLE_SIZE 4

/*
	In this hash table, we use linear probing to resolve collisions.
	Because of this, we have the possibility of primary clustering.
	i.e elements mapped to the same index, will be clumped together
	near that index. Now lets say that initially we inserted X at index
	1. then again, we try to insert Y but it maps to the same index as
	X. Now both of them map to the same index, but since we use linear
	probing, we put Y at the next slot beside X. Now lets say that we
	delete X and replace its position's value with NULL. Now we try to
	lookup Y. When we hash Y, we get the same index as X previously
	but since that slot is empty, we may stop the search, but in reality
	we must continue to search nearby slots. So when we delete X, 
	instead of replacing its value with NULL, we place SEARCH_CONTINUE
*/
#define SEARCH_CONTINUE ((void * ) -1)

//The threshold at which we increase the size of the table.
#define LOAD_FACTOR_THRESHOLD 0.7

//Factor by which we extend the size of the table.
#define EXTENSION_FACTOR 2

//Some arbitrary named constants
#define HT_LOOKUP_MODE 0
#define HT_INSERT_MODE 1
#define HT_DELETE_MODE 2

typedef struct HashTable
{
	table_size_t size;
	//The number of keys value helps us determine the load factor
	table_size_t numberOfKeys;
	// This implementation uses open addressing so, we use an array.
	void **table;
	//for comparisons during the lookup operation we need a comparator function
	int (*comparator)(void *, void *);
	//the hash function is configurable.
	table_size_t (*hashFunction)(struct HashTable *, void *);

} HashTable;

// hidden functions
static void _rehash(HashTable *);
static table_size_t _iterateTable(HashTable *, void *, int);

//constructor and destructor
HashTable * createHashTable(table_size_t (*)(HashTable *, void *), int (*)(void *, void *));
void destroyHashTable(HashTable *);

//hashtable operations
int insert(HashTable *, void *);
void * delete(HashTable *, void *);
int lookup(HashTable *, void *);

#endif
