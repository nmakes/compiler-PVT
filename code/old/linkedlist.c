#include <stdio.h>
#include "linkedlistDef.h"

/*
	===========================
	== LINKED LIST FUNCTIONS ==
	===========================
*/

dt_linkedListNode llMakeNode(void * data, int size) // assuming data is newly malloc'd
{
	dt_linkedListNode node = (dt_linkedListNode) malloc(sizeof(struct __LINKED_LIST_NODE));
	
	if(node==NULL)
	{
		printf("ERROR::helpers.h::llMakeNode: could not malloc node");
	}
	else
	{
		node->data = data;
		node->size = size;
		node->prev = NULL;
		node->next = NULL;
	}

	return node;
}

void llFreeNode(dt_linkedListNode node)
{
	free(node->data);
	free(node);
}

dt_linkedList llInit()
{
	dt_linkedList l = (dt_linkedList) malloc(sizeof(struct __LINKED_LIST));
	l->front = NULL;
	l->back = NULL;
	l->count = 0;
	return l;
}

void llPushBack(dt_linkedList ll, void * data, int size)
{
	if(ll->front==NULL && ll->back==NULL)
	{
		ll->front = ll->back = llMakeNode(data, size);
		ll->count = 1;
	}
	else
	{
		dt_linkedListNode  node = llMakeNode(data, size);
		node->prev = ll->back;
		ll->back->next = node;
		ll->back = node;
		ll->count++;
	}
}

void llPushFront(dt_linkedList ll, void * data, int size)
{
	if(ll->front==NULL && ll->back==NULL)
	{
		ll->front = ll->back = llMakeNode(data, size);
		ll->count = 1;
	}
	else
	{
		dt_linkedListNode  node = llMakeNode(data, size);
		node->next = ll->front;
		ll->front->prev = node;
		ll->front = node;
		ll->count++;
	}
}

void llPopFront(dt_linkedList ll)
{
	if(ll->count<=0)
	{
		printf("ERROR::helpers.h::llPopFront: list is empty");
	}
	else if(ll->front==ll->back)
	{
		dt_linkedListNode node = llMakeNode(ll->front->data, ll->front->size);
		dt_linkedListNode topNode = ll->front;
		ll->front = ll->back = NULL;
		ll->count = 0;
		llFreeNode(topNode);
	}
	else
	{
		dt_linkedListNode node = llMakeNode(ll->front->data, ll->front->size);
		dt_linkedListNode topNode = ll->front;
		ll->front = topNode->next;
		ll->front->prev = NULL;
		ll->count--;
		llFreeNode(topNode);
	}
}

void llPopBack(dt_linkedList ll)
{
	if(ll->count<=0)
	{
		printf("ERROR::helpers.h::llPopBack: list is empty");
	}
	else if(ll->front==ll->back)
	{
		dt_linkedListNode node = llMakeNode(ll->back->data, ll->back->size);
		dt_linkedListNode endNode = ll->back;
		ll->back = ll->front = NULL;
		ll->count = 0;
		llFreeNode(endNode);
	}
	else
	{
		dt_linkedListNode node = llMakeNode(ll->back->data, ll->back->size);
		dt_linkedListNode endNode = ll->back;
		ll->back = endNode->prev;
		ll->back->next = NULL;
		ll->count--;
		llFreeNode(endNode);
	}
}