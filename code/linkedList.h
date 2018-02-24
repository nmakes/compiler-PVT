// ====================
// LINKED LIST DATATYPE
// ====================

typedef struct __LINKED_LIST_NODE dt_linkedListNode;
struct struct __LINKED_LIST_NODE
{
	void * data;
	int size;

	dt_linkedListNode * next;
	dt_linkedListNode * prev;
};

dt_linkedListNode * llMakeNode(void * data, int size)
{
	dt_linkedListNode * node = (dt_linkedListNode*) malloc(sizeof(dt_linkedListNode));
	
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

typedef __LINKED_LIST dt_linkedList;
struct __LINKED_LIST
{
	dt_linkedListNode * head;
	dt_linkedListNode * tail;
	int count;
};

void llPushBack(dt_linkedList * l, void * data, int size, int pos=-1)
{
	if(pos==-1)
	{
		pos = l->count;
	}


	if(ll->head==NULL && ll->tail==NULL)
	{
		ll->head = ll->tail = llMakeNode(data, size);
		ll->count = 1;
	}
	else
	{
		dt_linkedListNode * mov = l->

		// move pointer and insert in location

		dt_linkedListNode * node = llMakeNode(data, size);
		node->prev = ll->tail;
		ll->tail->next = node;
		ll->tail = node;
		ll->count++;
	}
}

void llPopFront(dt_linkedList * l)
{
	if(l->count<=0)
	{
		printf("ERROR::helpers.h::llPopFront: list is empty");
	}
	else if
	{
		
	}
}