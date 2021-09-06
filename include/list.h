#ifndef LIST_H
#define LIST_H


typedef struct list list_t;

int list_init(list_t **l);
void list_free(list_t *l);


int list_add_head(list_t *l, void *v);
int list_add_tail(list_t *l, void *v);
int list_delete(list_t *l , void *v);
void list_display(list_t *l);
void *list_next(list_t *l, void *v);
void *list_get(list_t *l, int index);





#endif
