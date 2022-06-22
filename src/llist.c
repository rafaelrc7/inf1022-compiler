#include <assert.h>
#include <stdlib.h>

#include "llist.h"

typedef struct _node Node;
struct _node {
	void *val;
	Node *next;
};

struct _llist {
	Node *head, *last;
};

LinkedList *llist_create(void) {
	return (LinkedList *)calloc(1, sizeof(LinkedList));
}

void llist_free(LinkedList *ll) {
	assert(ll != NULL);
	while (!llist_is_empty(ll))
		llist_pop(ll);
	free(ll);
}

int llist_is_empty(LinkedList *ll) {
	assert(ll != NULL);
	return ll->head == NULL;
}

void llist_push(LinkedList *ll, void *v) {
	Node *n;

	assert(ll != NULL);

	n = (Node *)malloc(sizeof(Node));
	if (n == NULL)
		return;

	n->next = ll->head;
	n->val = v;

	if (llist_is_empty(ll))
		ll->last = n;

	ll->head = n;
}

void llist_append(LinkedList *ll, void *v) {
	Node *n;

	assert(ll != NULL);

	n = (Node *)malloc(sizeof(Node));
	if (n == NULL)
		return;

	n->next = NULL;
	n->val = v;

	if (llist_is_empty(ll)) {
		ll->head = n;
	} else {
		ll->last->next = n;
	}

	ll->last = n;
}

void *llist_pop(LinkedList *ll) {
	Node *next;
	void *val;

	assert(ll != NULL);

	if (ll->head == NULL)
		return NULL;

	next = ll->head->next;
	val = ll->head->val;
	free(ll->head);

	if (next == NULL)
		ll->last = NULL;
	ll->head = next;

	return val;
}

