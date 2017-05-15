#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "treenode.h"
#include "queue.h"

typedef struct stCode stCode;
typedef unsigned long long int ulli;
struct stCode
{
	unsigned int length;
	unsigned long long int code[4];
};
void printCodes(stCode **codes);

void encode(char *encoding_filename, char *archive_filename)
{
	unsigned char buf[1024], outbuf[1024];

	size_t bufsize, outbufsize, ospace, i, j, k, partofcode, clqsize = 0,
		filesize=0, bedsize=0;
	unsigned long long int mask = 0;
	unsigned int freqtable[256] = { 0 };
	unsigned int sum;
	treeNode* bed[256] = { 0 };
	treeNode *root;
	unsigned int codelength;
	unsigned int clq[256];
	unsigned long long int code[4] = { 0 };

	stCode* codetable[256] = { 0 };
	queue* head=NULL;

	FILE* enc = fopen(encoding_filename, "rb");
	if (enc == NULL)
	{
		printf("Cannot open file: %s\n", encoding_filename);
		exit(1);
	}

	do
	{
		bufsize = fread(buf, 1, 1024, enc);
		filesize += bufsize;

		for (i = 0; i < bufsize; i++)
		{
			freqtable[buf[i]]++;
		}

	} while (bufsize == 1024);

	for (i = 0; i < 256; i++)
	{
		if (freqtable[i])
		{
			bed[bedsize++] = createNode(i);
			mask = mask | (1 << (255 - i));
			
		}
	}
	
	k = bedsize;
	while (k > 1)
	{
		for (i = 0; i < k-1; i++)
		{
			if (freqtable[(bed[i])->value] < freqtable[(bed[i + 1])->value])
				swap(bed[i], bed[i + 1]);
		}
		for (i = 0; i < k - 2; i++)
		{
			if (freqtable[(bed[i])->value] < freqtable[(bed[i + 1])->value])
				swap(bed[i], bed[i + 1]);
		}

		sum = freqtable[(bed[k - 2])->value] + freqtable[(bed[k - 1])->value];
		bed[k - 2] = joinNodes(bed[k - 1], bed[k - 2]);

		freqtable[(bed[k - 2])->value] = sum;

		k--;
	}

	printTree(bed[0]);

	root = bed[0];
	codelength = 0;
	partofcode = 0;

	while (!isEmpty(head) || root != NULL)
	{
		if (root != NULL)
		{
			push(&head, root);
			if (root != bed[0])
			{		
				code[partofcode] = code[partofcode] | ((ulli)0 << (codelength - 1 - 64 * partofcode));
			}
			
			clq[clqsize] = codelength;
			codelength++;
			clqsize++;
			
			if (codelength > (partofcode + 1) * 64)
				partofcode++;
			root = root->link[0];
			
		}
		else
		{
			root = (treeNode*)popBack(&head);

			codelength = clq[--clqsize];
			
			while (codelength < partofcode * 64)
			{
				code[partofcode] = 0;
				partofcode--;
			}

			code[partofcode] = code[partofcode] & (((ulli)1 << (codelength + 1 - 64 * partofcode)) - 1);

			if (codetable[root->value] == NULL)
			{
				codetable[root->value] = malloc(sizeof(stCode));

				codetable[root->value]->length = codelength;
				for (i = 0; i < 4; i++)
				{
					codetable[root->value]->code[i] = code[i];
				}
			}

			root = root->link[1];
			codelength++;

			if (codelength >(partofcode + 1) * 64)
				partofcode++;
			code[partofcode] = code[partofcode] | ((ulli)1 << (codelength - 1 - 64 * partofcode));
	
		}
	}

	printCodes(codetable);

	fseek(enc, 0, SEEK_SET);

	outbufsize = 0;
	ospace = 8;

	do
	{
		bufsize = fread(buf, 1, 1024, enc);
		
		for (i = 0; i < bufsize; i++)
		{
			codelength = codetable[buf[i]]->length;
			while(codelength>0)
			{
				if (codelength % 64 > 8)
				{
					outbuf[outbufsize] = codetable[buf[i]]->code[codelength / 64] & ((((ulli)1 << 8) - 1) << codelength % 64);
					outbufsize++;
					codelength -= 8;
				}
				else
				{
					outbuf[outbufsize] = (codetable[buf[i]]->code[codelength / 64] & ((((ulli)1 << codelength%64) - 1))<<8-codelength%64);
				}
			}
		}

	} while (bufsize == 1024);

	fclose(enc);
	return;
}

void printCodes(stCode **codes)
{
	int k = 0;
	for (int i = 0; i < 256; i++)
	{
		k = 0;
		if (codes[i] == NULL)
			continue;
		else
		{
			printf("Symbol: %c\nLength: %i\nCode: ", i,codes[i]->length);
			for (unsigned int j = 0; j < codes[i]->length; j++)
			{
				if (j >= (k + 1) * 64)
					k++;
				printf("%i", ((codes[i]->code[k])&(1 << j))>>j);
			}
			printf("\n");
		}
	}
}

