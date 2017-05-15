#include "stdio.h"
#include "stdlib.h"

typedef struct queue queue;
struct queue
{
	queue* next;
	void* point;
};

void* pop(queue** head)
{
	if (*head == NULL)
	{
		return 0;
	}
	void* tmp = (*head)->point;
	if ((*head)->next == NULL)
	{
		free(*head);
		*head = NULL;
		return tmp;
	}


	queue* tq = *head;
	(*head) = (*head)->next;
	free(tq);
	return tmp;
}

void* popBack(queue** head)
{
	if (*head == NULL)
		return NULL;
	if ((*head)->next == NULL)
	{
		void* tmp = (*head)->point;
		free(*head);
		*head = NULL;
		return tmp;
	}
	queue* cur = *head;
	while ((cur->next)->next != NULL)
	{
		cur = cur->next;
	}
	void* tmp = (cur->next)->point;
	free(cur->next);
	cur->next = NULL;
	return tmp;
}

void push(queue** head,void* val)
{
	queue* nqueue = malloc(sizeof(queue));
	nqueue->point = val;
	nqueue->next = NULL;
	queue* cur = *head;
	if (*head == NULL)
	{
		*head = nqueue;
	}
	else
	{
		while (cur->next)
		{
			cur = cur->next;
		}
		cur->next = nqueue;
	}
}

int isEmpty(queue* head)
{
	if (head == NULL)
		return 1;
	else
		return 0;
}

int isOnly(queue* head, void* p)
{
	while (head!=NULL)
	{
		if (head->point != p)
			return 0;
		head = head->next;
	}
	return 1;
}

void printQueueInt(queue* head)
{
	while (head != NULL)
	{
		printf("%i ", *(int*)(head->point));
		head = head->next;
	}
	printf("\n");
}