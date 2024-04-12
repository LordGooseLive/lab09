#include <stdio.h>
#include <stdlib.h>

#define MAXHASH 17

// RecordType
typedef struct RecordType
{
	int		id;
	char	name;
	int		order; 
} RecordType;

typedef struct node //linked list node
{
	RecordType* data;
	struct node* next;
} node;

// Fill out this structure
typedef struct HashType //hash table
{
	int size;
	node* head; //linked list
} HashType;

// Compute the hash function
int hash(int x)
{
	return x % MAXHASH;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i, j;
	//int  k = 0
	node* curr = NULL;
	//if (pHashArray == NULL)
	//	printf("passby array failed\n");

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		curr = pHashArray[i].head;
		if (curr!= NULL)
		{
			for (j = 0; j < pHashArray[i].size; j++)
			{	
				printf("table[%d] item %d: \n", i, j);
				printf("\t%d %c %d\n\n", curr->data->id , curr->data->name, curr->data->order);
				curr = curr->next;
			}
		}
	}
}

//frees the hash
void freeHash (HashType * pHashArray)
{
	int i, j;
	node* curr = NULL, *temp = NULL;

	//free nodes
	for (i = 0; i < MAXHASH; i++)
	{
		curr = pHashArray[i].head;
		if (curr!= NULL)
		{
			for (j = 0; j < pHashArray[i].size; j++)
			{
				temp = curr;
				curr = curr->next;
				free (temp);
			}
		}
	}
	//free hash
	free(pHashArray);
}

int main(void)
{
	RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

	//temp variables
	int i, j;
	node* curr = NULL, *new_node = NULL;

	//make and initialise table
	HashType table[MAXHASH];
	for ( i = 0; i < MAXHASH; i++) 
	{	
		table [i].head = NULL;
		table [i].size = 0;
	}
	
	//hash data
	//check pRecords and hash the ID
	//insert into that index of table
	for (j=0; j < recordSz; j++)
	{
		i = hash(pRecords[j].id);
		curr = table[i].head;

		//set-up new node
		new_node = (node*) malloc(sizeof(node)); //make node
		new_node->data = &(pRecords[j]);
		new_node->next = NULL;

		//locate tail
		if (table[i].size == 0) //list empty
			table[i].head = new_node;
		
		else //traverse to tail of list
		{
			while (curr->next!= NULL)
				curr = curr->next;

			curr->next = new_node;
		}
		table[i].size++;
	}

	displayRecordsInHash(table, MAXHASH);

	//free data 
	freeHash(table);
}