#ifndef __LIST_H
#define __LIST_H
 
/* a function used to test if two nodes are equals
	must return 1 if elements are equals, 0 if not */
typedef int (*equalsFunction)(void *, void *);
// a function used to free a node
typedef void (*freeFunction)(void *);
 
typedef struct listNode {
	void *data;
	struct listNode *next;
} listNode;
 
typedef struct list {
	int length;
	int elementSize;
	listNode *head;
	listNode *tail;
	equalsFunction equalsFn;
	freeFunction freeFn;
} list;
 
void list_init(list *list, int elementSize, equalsFunction equalsFn, freeFunction freeFn);
void list_destroy(list *list);

// adds at the end of the list
void list_add(list *list, void *element);
void list_remove(list *list, void *element);
 
#endif