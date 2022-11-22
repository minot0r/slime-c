#include "../../include/objects/other/linkedlist.h"

#include <stdlib.h>

node_t* create_node(void* data) {
    node_t* node = malloc(sizeof(node_t));
    node->data = data;
    node->next = NULL;
    return node;
}

linked_list_t* linked_list_init() {
    linked_list_t* list = malloc(sizeof(linked_list_t));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void linked_list_add(linked_list_t* linked_list, void* data) {
    node_t* node = create_node(data);
    if(linked_list->head == NULL) {
        linked_list->head = node;
        linked_list->tail = node;
    } else {
        linked_list->tail->next = node;
        linked_list->tail = node;
    }
    linked_list->size++;
}

void linked_list_remove(linked_list_t* linked_list, int index) {
    if(index < 0 || index >= linked_list->size) return;
    node_t* node = linked_list->head;
    node_t* prev = NULL;
    for(int i = 0; i < index; i++) {
        prev = node;
        node = node->next;
    }
    if(prev == NULL) {
        linked_list->head = node->next;
    } else {
        prev->next = node->next;
    }
    if(node == linked_list->tail) {
        linked_list->tail = prev;
    }
    free(node->data);
    free(node);
    linked_list->size--;
}

void linked_list_clear(linked_list_t* linked_list) {
    node_t* node = linked_list->head;
    while(node != NULL) {
        node_t* next = node->next;
        free(node);
        node = next;
    }
    linked_list->head = NULL;
    linked_list->tail = NULL;
    linked_list->size = 0;
}

void* linked_list_get(linked_list_t* linked_list, int index) {
    if(index < 0 || index >= linked_list->size) return NULL;
    node_t* node = linked_list->head;
    for(int i = 0; i < index; i++) {
        node = node->next;
    }
    return node->data;
}

void linked_list_destroy(linked_list_t* linked_list) {
    linked_list_clear(linked_list);
    free(linked_list);
}