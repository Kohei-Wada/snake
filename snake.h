typedef struct _node {
    int x;
    int y;
    struct _node *prev;
    struct _node *next;
}Node;


typedef struct {
    Node *head;
    Node *tail;
    int vx;
    int vy;
}Snake;



enum {
    FIELD,
    WALL_H,
    WALL_V,
    SNAKE,
    FOOD,
};
