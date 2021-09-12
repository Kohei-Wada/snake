#ifndef OBSERVER_H
#define OBSERVER_H

typedef struct observer observer_t;

int observer_update(observer_t *o);
void observer_set_update_function(observer_t *o, int (*f)(observer_t *));


#endif
