// ====================
// LINKED LIST DATATYPE
// ====================
struct __LINKED_LIST_NODE;
struct __LINKED_LIST;
typedef struct __LINKED_LIST_NODE * dt_linkedListNode;
typedef struct __LINKED_LIST * dt_linkedList;

struct __LINKED_LIST_NODE
{
	void * data;
	int size;

	dt_linkedListNode next;
	dt_linkedListNode prev;
};

struct __LINKED_LIST
{
	dt_linkedListNode front;
	dt_linkedListNode back;
	int count;
};
// front [0]-[1]-[2]-...-[n] back

// dt_linkedListNode llMakeNode(void * data, int size);

// void llFreeNode(dt_linkedListNode node);

// dt_linkedList llInit();

// void llPushBack(dt_linkedList ll, void * data, int size);

// void llPushFront(dt_linkedList ll, void * data, int size);

// void llPopFront(dt_linkedList ll);

// void llPopBack(dt_linkedList ll);