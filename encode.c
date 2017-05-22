#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "treenode.h"
#include "queue.h"

typedef struct stCode stCode;
typedef unsigned long long int ulli;
struct stCode
{
	unsigned char length;
	unsigned char code[256];
};
void printCodes_(stCode **codes);

void encode(char *encoding_filename, char *archive_filename)
{
	unsigned char buf[1024], outbuf[1024] = { 0 };

	size_t bufsize, outbufsize, ospace, i, j, k, clqsize,
		 bedsize = 0;
	unsigned int freqtable[256] = { 0 };
	unsigned int sum;
	unsigned int filesize = 0;
	unsigned char codelength;
	unsigned int clq[256];
	unsigned char code[256] = { 0 };

	treeNode* bed[256] = { 0 };
	treeNode *root;
	

	stCode* codetable[256] = { 0 };
	queue* head = NULL;

	FILE* enc = fopen(encoding_filename, "rb");
	if (enc == NULL)
	{
		printf("Cannot open file: %s\n", encoding_filename);
		exit(1);
	}

	FILE* arch = fopen(archive_filename, "wb");

	do
	{
		bufsize = fread(buf, 1, 1024, enc);
		filesize += bufsize;

		for (i = 0; i < bufsize; i++)
		{
			freqtable[buf[i]]++;
		}

	} while (bufsize == 1024);

	fwrite(&filesize, sizeof(unsigned int), 1, arch);

	ospace = 8;
	outbufsize = 0;
	for (i = 0; i < 256; i++)
	{
		if (freqtable[i])
		{
			bed[bedsize++] = createNode(i);
			outbuf[outbufsize] = outbuf[outbufsize] | (1 << ospace - 1);
		}
		ospace--;
		if (ospace == 0)
		{
			outbufsize++;
			ospace = 8;
		}
	}

	fwrite(outbuf, 1, 32, arch);
	memset(outbuf, 0, 1024);

	k = bedsize;
	while (k > 1)
	{
		for (i = 0; i < k - 1; i++)
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

	root = bed[0];
	codelength = 0;
	clqsize = 0;
	
	while (!isEmpty(head) || root != NULL)
	{
		if (root != NULL)
		{
			push(&head, root);
			code[codelength] = 0;

			clq[clqsize] = codelength;
			codelength++;
			clqsize++;

			root = root->link[0];

		}
		else
		{
			root = (treeNode*)popBack(&head);

			codelength = clq[--clqsize];

			if (root->value == -1)
			{
				if (codetable[255] == NULL)
				{
					codetable[255] = malloc(sizeof(stCode));
					codetable[255]->length = codelength;
					for (i = 0; i < codelength; i++)
					{
						codetable[255]->code[i] = code[i];
					}
				}
			}

			if (codetable[(size_t)(root->value)] == NULL)
			{
				codetable[(size_t)(root->value)] = malloc(sizeof(stCode));
				codetable[(size_t)(root->value)]->length = codelength;
				for (i = 0; i < codelength; i++)
				{
					codetable[root->value]->code[i] = code[i];
				}
			}

			root = root->link[1];
			code[codelength] = 1;
			codelength++;

			

		}
	}
	
	for (i = 0; i < 256; i++)
	{
		if (codetable[i] != NULL)
		{
			fwrite(&(codetable[i]->length), 1, 1, arch);
			k = 0;
			outbufsize = 0;
			ospace = 8;

			while (k < codetable[i]->length)
			{
				outbuf[outbufsize] = outbuf[outbufsize] | (codetable[i]->code[k] << (ospace - 1));
				k++;
				ospace--;
				if (ospace == 0)
				{
					outbufsize++;
					ospace = 8;
				}
			}
			if (ospace != 8)
			{
				outbufsize++;
			}
			fwrite(outbuf, 1, outbufsize, arch);
			memset(outbuf, 0,1024);
		}
	}

	fseek(enc, 0, SEEK_SET);

	outbufsize = 0;
	ospace = 8;
	k = 0;

	do
	{
		bufsize = fread(buf, 1, 1024, enc);

		for (i = 0; i < bufsize; i++)
		{
			codelength = codetable[(size_t)buf[i]]->length;
			
			k = 0;
			while (k < codelength )
			{
				outbuf[outbufsize] = outbuf[outbufsize]|(codetable[buf[i]]->code[k] << (ospace - 1));
				
				k++;
				ospace--;
				if (ospace == 0)
				{
					outbufsize++;
					ospace = 8;
					if (outbufsize == 1024)
					{
						fwrite(outbuf, 1, 1024, arch);
						outbufsize = 0;
						memset(outbuf, 0, 1024);
					}
					
				}
			}
		}

	} while (bufsize == 1024);
	if (ospace != 8)
		outbufsize++;
	fwrite(outbuf, 1, outbufsize, arch);
	

	fclose(enc);
	fclose(arch);
	return;
}

void printCodes_(stCode **codes)
{
	int k = 0;
	for (int i = 0; i < 256; i++)
	{
		k = 0;
		if (codes[i] == NULL)
			continue;
		else
		{
			printf("Symbol: %c\nLength: %i\nCode: ", i, codes[i]->length);
			for (unsigned int j = 0; j < codes[i]->length; j++)
			{
				printf("%c", codes[i]->code[j]+48);
			}
			printf("\n");
		}
		printf("\n");
	}
}

