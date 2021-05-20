#include <stdio.h> 
#include <stdlib.h>

typedef struct node {
	struct node *prev;
	struct node *next;
	void *v;
} node_t;

typedef struct list {
	node_t *head;
	node_t *tail;
} list_t;

/*********************************************************************/

int list_init(list_t **l)
{
	*l = malloc(sizeof(list_t));
	if (!(*l)) return 1;

	(*l)->head = (*l)->tail = NULL;
	return 0;
}

void list_free(list_t *l)
{
	node_t *n, *tmp;
	for (n = l->head; n; n = tmp) {
		tmp = n->next; free(n);
	}
	free(l);
}

static int list_add_node_tail(list_t *l, node_t *n)
{
	if (!l || !n) return 1;
	n->prev = l->tail;
	if (l->tail) 
		l->tail->next = n;
	else 
		l->head = n;
	l->tail = n;

	return 0;
}

static int list_add_node_head(list_t *l, node_t *n)
{
	if (!l || !n) return 1;

	n->next = l->head;
	if (l->head) 
		l->head->prev = n;
	else 
		l->tail = n;
	l->head = n;

	return 0;
}

int list_add_head(list_t *l, void *v)
{
	node_t *n = malloc(sizeof(node_t));
	if (!n) return 1;
	n->v = v;
	return list_add_node_head(l, n);
}

int list_add_tail(list_t *l, void *v)
{
	node_t *n = malloc(sizeof(node_t));
	if (!n) return 1;
	n->v = v;
	return list_add_node_tail(l, n);
}

static int list_delete_node(list_t *l, node_t *n)
{
	if (!l || !n) return 1;

	if (n == l->head && n == l->tail) 
		l->head = l->tail = NULL;
	else if (n == l->head) {
		n->next->prev = NULL; l->head = n->next;
	} else if (n== l->tail) {
		n->prev->next = NULL; l->tail = n->prev;
	} else {
		n->prev->next = n->next;
		n->next->prev = n->prev;
	}
	free(n);
	return 0;
}

static node_t* list_get_node(list_t *l, void *v)
{
	node_t *n;
	for (n = l->head; n && n->v != v; n = n->next);
	return n;
}

int list_delete(list_t *l, void *v)
{
	node_t *n = list_get_node(l, v);
	return list_delete_node(l, n);
}

void list_display(list_t *l)
{
	for (node_t *n = l->head; n; n = n->next)
		printf("n = %p\n", n);
	//	printf("n = %p, *(n->v) = %d\n", n, *(int*)(n->v));
}


void* list_next(list_t *l, void *v)
{
	node_t *n = list_get_node(l, v);
	if (!n) return NULL;
	return n->next? n->next->v : NULL;
}


void *list_get(list_t *l, int index)
{
	int i; node_t *n = l->head;
	for (i = 0; n && i < index; ++i){
		n = n->next;
	}

	return n? n->v : NULL;
}
