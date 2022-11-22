#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct node_s {
    void* data;
    struct node_s* next;
} node_t;

typedef struct {
    node_t* head;
    node_t* tail;
    int size;
} linked_list_t;

linked_list_t* linked_list_init();
void linked_list_add(linked_list_t* linked_list, void* data);
void linked_list_remove(linked_list_t* linked_list, int index);
void linked_list_clear(linked_list_t* linked_list);
void* linked_list_get(linked_list_t* linked_list, int index);
void linked_list_destroy(linked_list_t* linked_list);

#endif