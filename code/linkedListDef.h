// ====================
// LINKED LIST DATATYPE
// ====================

#ifndef __LINKEDLISTDEF_H
#define __LINKEDLISTDEF_H

struct __LINKED_LIST_NODE
{
	void * data;
	int size;

	struct __LINKED_LIST_NODE * next;
	struct __LINKED_LIST_NODE * prev;
};
typedef struct __LINKED_LIST_NODE * dt_linkedListNode;

struct __LINKED_LIST
{
	struct __LINKED_LIST_NODE * front;
	struct __LINKED_LIST_NODE * back;
	int count;
};
typedef struct __LINKED_LIST * dt_linkedList;
// front [0]-[1]-[2]-...-[n] back

// dt_linkedListNode llMakeNode(void * data, int size);

// void llFreeNode(dt_linkedListNode node);

// dt_linkedList llInit();

// void llPushBack(dt_linkedList ll, void * data, int size);

// void llPushFront(dt_linkedList ll, void * data, int size);

// void llPopFront(dt_linkedList ll);

// void llPopBack(dt_linkedList ll);

#endif