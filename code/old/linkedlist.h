#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "linkedlistDef.h"

dt_linkedListNode llMakeNode(void * data, int size);
void llFreeNode(dt_linkedListNode node);
dt_linkedList llInit();
void llPushBack(dt_linkedList ll, void * data, int size);
void llPushFront(dt_linkedList ll, void * data, int size);
void llPopFront(dt_linkedList ll);
void llPopBack(dt_linkedList ll);

#endif