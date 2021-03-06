#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

typedef struct _llist_node LLNode;
struct _llist_node {
	void *val;
	LLNode *next;
};

typedef struct _llist LinkedList;
struct _llist {
	LLNode *head, *last;
};

LinkedList *llist_create(void);
void llist_free(LinkedList *ll);

int llist_is_empty(LinkedList *ll);

void llist_push(LinkedList *ll, void *v);
void llist_append(LinkedList *ll, void *v);
void *llist_pop(LinkedList *ll);
void *llist_search(const LinkedList *ll, const void *key, int (*cmp)(const void *node, const void *key));

#endif

