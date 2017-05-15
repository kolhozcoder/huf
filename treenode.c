#include "stdlib.h"
#include "stdio.h"
#include "queue.h"


typedef struct treenode treeNode;
struct treenode
{
	treeNode *link[2];
	char value;
};



void swap(treeNode *n1, treeNode *n2)
{
	treeNode* tmp0 = n1->link[0];
	treeNode* tmp1 = n1->link[1];
	char tmpval = n1->value;
	n1->value = n2->value;
	n1->link[0] = n2->link[0];
	n1->link[1] = n2->link[1];
	n2->link[0] = tmp0;
	n2->link[1] = tmp1;
	n2->value = tmpval;
}

treeNode* createNode(char val)
{
	treeNode *node = malloc(sizeof(treeNode));
	node->value = val;
	node->link[0] = node->link[1] = NULL;
	return node;
}

treeNode* joinNodes(treeNode *left, treeNode *right)
{
	treeNode *node = malloc(sizeof(treeNode));
	node->link[0] = left;
	node->link[1] = right;
	node->value = left->value;
	return node;
}

void bedSort(treeNode **bed, unsigned int *freqtable, size_t bedsize)
{
	size_t i, j;
	char swapped = 0;

	for (i = 0; i < bedsize - 1; i++)
	{
		swapped = 0;
		for (j = 0; j < bedsize - i - 1; j++)
		{
			if (freqtable[(bed[j])->value] < freqtable[(bed[j + 1])->value])
			{
				printf("before b1 %c b2 %c\n", bed[j]->value, bed[j + 1]->value);
				swap(bed[j], bed[j + 1]);
				printf("after b1 %c b2 %c\n", bed[j]->value, bed[j + 1]->value);
				swapped = 1;
			}
		}
		if (swapped == 0)
			return;
	}

}
void printTree_(treeNode* root)
{
	if (root != NULL)
	{
		printf("%c ", root->value);
		printf("( ");
		printTree_(root->link[0]);
		printTree_(root->link[1]);
		printf(") ");
	}
}

void printTree(treeNode* root)
{
	if (root == NULL)
		return;
	queue* q = NULL;
	treeNode* cur = NULL;
	treeNode* terminal = malloc(sizeof(treeNode));
	treeNode* space = malloc(sizeof(treeNode));
	int height = treeHeight(root);
	int strwidth = 3;


	for (int i = 0; i<height - 1; i++)
	{
		strwidth *= 2;
	}
	strwidth++;

	int indent = strwidth / 2;
	int spacebtw = strwidth - indent;

	printf("HOW TALL IS THE TREE? %i nodes\n", height);

	push(&q, root);
	push(&q, terminal);
	if (root != NULL)
	{
		for (int i = 0; i < indent / 2; i++)
			printf(" ");
	}
	while (!isEmpty(q))
	{

		cur = (treeNode*)pop(&q);
		if (cur == terminal && !isEmpty(q) && !isOnly(q, space))
		{
			push(&q, terminal);

			printf("\n");
			indent /= 2;
			for (int i = 0; i < (indent / 2 == 1 ? 2 : indent / 2); i++)
				printf(" ");
			if (indent == 1)
				printf(" ");

			spacebtw /= 2;
			continue;
		}
		if (cur == terminal && isOnly(q, space))
		{
			break;
		}
		if (cur == terminal && isEmpty(q))
		{
			continue;
		}
		
		if (cur != space)
		{
			if (indent == 1)
			{
				printf(" %c", cur->value);
				
			}
			else
			{
				printf("%c", 218);
				for (int i = 0; i < indent / 2 - 1; i++)
					printf("-");
				printf("%c", cur->value);
				if (cur->link[0])
				{
					push(&q, cur->link[0]);
				}
				else
				{
					push(&q, space);
				}
				if (cur->link[1])
				{
					push(&q, cur->link[1]);
				}
				else
				{
					push(&q, space);
				}
				for (int i = 0; i < indent / 2 - 1; i++)
				{
					printf("-");
				}
				printf("%c", 191);
				//for (int i = 0; i < (indent / 2) * 2 - 1; i++)
				for (int i = 0; i < (indent - 1 == 2 ? 3 : indent - 1); i++)
				{
					printf(" ");
				}
			}
		}
		else
		{
			if (indent == 1)
			{
				printf("   ");
			}
			else
			{
				printf(" ");
				for (int i = 0; i < indent / 2; i++)
					printf(" ");

				for (int i = 0; i < indent / 2 - 1; i++)
				{
					printf(" ");
				}
				printf(" ");

				for (int i = 0; i < (indent - 1 == 2 ? 3 : indent - 1); i++)
				{
					printf(" ");
				}
			}
			push(&q, space);
			push(&q, space);
		}

	}
	printf("\n");
	free(terminal);
	free(space);
}

int treeHeight(treeNode *root)
{
	if (!root)
		return 0;
	else
		return treeHeight(root->link[0]) > treeHeight(root->link[1]) ? treeHeight(root->link[0]) + 1 : treeHeight(root->link[1]) + 1;
}

int bfs(treeNode* root, char searchvalue, unsigned int *length)
{
	if (root->value == searchvalue && root->link[0] == NULL)
	{
		*length++;
		return 2;
	}
	if (bfs(root->link[0], searchvalue, length) == 2)
	{
		*length++;
		return 0;
	}
	if (bfs(root->link[0], searchvalue, length) == 2)
	{
		*length++;
		return 0;
	}
}
