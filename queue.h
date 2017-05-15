#pragma once
typedef struct queue queue;
struct queue
{
	queue* next;
	char value;
};
void* pop(queue** head);
void* popBack(queue** head);
void push(queue** head, void* val);
int isEmpty(queue* head);
int isOnly(queue* head, void* p);
void printQueueInt(queue* head);