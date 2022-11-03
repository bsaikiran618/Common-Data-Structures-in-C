#include "GeneralLL.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int countDigits(int D)
{
	int count = 1;
	D = D / 10;
	while(D > 0)
	{
		count++;
		D = D / 10;
	}
	return count;
}

int main(int argc, char *argv[])
{
	
	int N;
	scanf("%d", &N);

	int arr[N];
	int largest;

	for(int i = 0; i < N; i++)
	{
		scanf("%d", &arr[i]);
		if(i==0) largest = arr[i];
		else largest = largest < arr[i] ? arr[i] : largest;
	}

	int iterationCount = countDigits(largest);

	List * listArr[10];

	//initialize the lists.

	for(int i=0;i<10;i++)
		listArr[i] = createList();

	for(int i = 0; i < iterationCount; i++)
	{
		for(int j = 0; j < N; j++)
		{
			int digit = (int)(arr[j] / pow(10, i)) % 10;
			int * num = (int *) malloc(sizeof(int));
			*num = arr[j];
			addEnd(listArr[digit],(void *) num);
		}
		//copy back elements from the list to the array.
		
		int k = 0;

		for(int j = 0; j < 10; j++)
		{
			while(listArr[j]->head)
			{
				int *num = (int *)(listArr[j]->head->data);
				arr[k++] = *num;
				removeBegin(listArr[j]);
				free(num);
			}
		}
	}

	for(int i=0;i<10;i++)
	{
		destroyList(listArr[i]);
		listArr[i] = NULL;
	}

	for(int i=0; i < N; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}
