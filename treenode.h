#pragma once
typedef struct treenode treeNode;
struct treenode
{
	treeNode *link[2];
	unsigned char value;
};

treeNode* createNode(unsigned char val);
treeNode* joinNodes(treeNode *left, treeNode *right);
void bedSort(treeNode **bed, unsigned int *freqtable, size_t bedsize);
void swap(treeNode *n1, treeNode *n2);
void printTree(treeNode* root);
void printTree_(treeNode* root);
int treeHeight(treeNode *root);
