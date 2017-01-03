#include <stdlib.h>
#include <string.h>

#include "list.h"

void list_init(list *l, int elementSize, equalsFunction equalsFn, freeFunction freeFn) {
	
	l->length = 0;
	l->elementSize = elementSize;
	l->head = list->tail = NULL;
	l->equalsFn = equalsFn;
	l->freeFn = freeFn;
}

void list_destroy(list *l) {

	listNode *current;
	while(l->head != NULL) {
		current = l->head;
		l->head = current->next;

		if(l->freeFn) {
			l->freeFn(current->data);
		}

		free(current->data);
		free(current);
	}	
}

void list_add(list *l, void *element) {
	listNode *node = malloc(sizeof(listNode));
  	node->data = malloc(l->elementSize);
	node->next = NULL;

	memcpy(node->data, element, l->elementSize);

	if(l->length == 0) {
		l->head = l->tail = node;
	} else {
		l->tail->next = node;
		l->tail = node;
  	}

	l->length++;
}

void list_remove(list *l, void *element) {
	listNode *currNode;
	listNode *prevNode;
	listNode *nextNode;

	prevNode = NULL;
	currNode = l->head;

	while(currNode) {

		nextNode = currNode->next;

		if(l->equalsFn(currNode->data, element)) {
			
			if(l->freeFn) {
      			l->freeFn(currNode->data);
    		}
    		free(currNode->data);
    		free(currNode);

    		if(prevNode) {
    			prevNode->next = nextNode;
    		} else {
    			l->head = nextNode;
    		}

    		l->length--;
    		return;
		}
		
		prevNode = currNode;
		currNode = nextNode;
	}
}

int list_contains(list *l, void* elem) {
	listNode *currNode;

	currNode = l->head;

	while(currNode) {

		if(l->equalsFn(currNode->data, element)) {
			return 1;
		}

		currNode = currNode->next;
	}

	return 0;
}