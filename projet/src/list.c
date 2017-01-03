#include <stdlib.h>
#include <string.h>

#include "list.h"

void list_init(list *list, int elementSize, equalsFunction equalsFn, freeFunction freeFn) {
	
	list->length = 0;
	list->elementSize = elementSize;
	list->head = list->tail = NULL;
	list->equalsFn = equalsFn;
	list->freeFn = freeFn;
}

void list_destroy(list *list) {

	listNode *current;
	while(list->head != NULL) {
    	current = list->head;
    	list->head = current->next;

    	if(list->freeFn) {
      		list->freeFn(current->data);
    	}

    	free(current->data);
    	free(current);
  	}	
}

void list_add(list *list, void *element) {
	listNode *node = malloc(sizeof(listNode));
  	node->data = malloc(list->elementSize);
	node->next = NULL;

	memcpy(node->data, element, list->elementSize);

	if(list->length == 0) {
    	list->head = list->tail = node;
	} else {
    	list->tail->next = node;
    	list->tail = node;
  	}

	list->length++;
}

void list_remove(list *list, void *element) {
	listNode *currNode;
	listNode *prevNode;
	listNode *nextNode;

	prevNode = NULL;
	currNode = list->head;

	while(currNode) {

		nextNode = currNode->next;

		if(list->equalsFn(currNode->data, element)) {
			
			if(list->freeFn) {
      			list->freeFn(currNode->data);
    		}
    		free(currNode->data);
    		free(currNode);

    		if(prevNode) {
    			prevNode->next = nextNode;
    		} else {
    			list->head = nextNode;
    		}
    		
    		list->length--;
    		return;
		}
		
		prevNode = currNode;
		currNode = nextNode;
	}
}