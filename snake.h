typedef struct _node {
    int x;
    int y;
    struct _node *prev;
    struct _node *next;
}Node;


typedef struct {
    Node *head;
    Node *tail;
}Snake;



enum FIELD_ELEMS{
    FIELD,
    SNAKE,
    FOOD,
    WALL_1,
    WALL_2
};
