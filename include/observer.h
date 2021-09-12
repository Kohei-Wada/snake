#ifndef OBSERVER_H
#define OBSERVER_H


typedef struct observer observer_t;

struct observer {
	void (*plot)(observer_t *o);
	int(*update)(observer_t *o);
};

int observer_update(observer_t *o);
void observer_plot(observer_t *o);

void observer_set_update_function(observer_t *o, int (*f)(observer_t *));
void observer_set_plot_function(observer_t *o, void (*f)(observer_t *));


#endif
