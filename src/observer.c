#include "observer.h"

int observer_update(observer_t *o)
{
	return o->update(o);
}


void observer_set_update_function(observer_t *o, int (*f)(observer_t *))
{
	o->update = f;
}


