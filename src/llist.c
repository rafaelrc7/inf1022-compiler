#include <assert.h>
#include <stdlib.h>

#include "llist.h"

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
	LLNode *n;

	assert(ll != NULL);

	n = (LLNode *)malloc(sizeof(LLNode));
	if (n == NULL)
		return;

	n->next = ll->head;
	n->val = v;

	if (llist_is_empty(ll))
		ll->last = n;

	ll->head = n;
}

void llist_append(LinkedList *ll, void *v) {
	LLNode *n;

	assert(ll != NULL);

	n = (LLNode *)malloc(sizeof(LLNode));
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
	LLNode *next;
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

void *llist_search(const LinkedList *ll, const void *key, int (*cmp)(const void *node, const void *key)) {
	LLNode *n;

	assert(ll != NULL);
	assert(key != NULL);
	assert(cmp != NULL);

	for (n = ll->head; n != NULL; n = n->next) {
		if (cmp(n->val, key) == 0)
			return n->val;
	}

	return NULL;
}

