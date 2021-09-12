#include "observer.h"

int observer_update(observer_t *o)
{
	return o->update(o);
}


void observer_plot(observer_t *o)
{
	o->plot(o);
}

void observer_set_plot_function(observer_t *o, void (*f)(observer_t *))
{
	o->plot = f;
}

void observer_set_update_function(observer_t *o, int (*f)(observer_t *))
{
	o->update = f;
}


