#include <stdlib.h> /* malloc(), exit()*/
#include "tk_container.h"

tk_node_t* tk_list_create(void* data){
    tk_node_t *node; 
    
    node = malloc(sizeof(tk_node_t));
    if(!node){
        return NULL;
    }
    
    node->data = data;
    node->next = NULL;
    
    return node;
}

void tk_list_destroy(tk_node_t **list, int flag)
{
    tk_node_t *current, *tmp;
    
    current = *list;
    while(current->next != NULL){
        tmp = current->next;
        if (flag) free(current->data);
        free(current);
        current = tmp;
    }
    
    *list = NULL;
}

int tk_list_push_front(tk_node_t **list, void *data)
{
    tk_node_t *new_node; 
    
    new_node = malloc(sizeof(tk_node_t));
    if(!new_node){
        return -1;
    }
    
    new_node->data = data;
    new_node->next = *list;
    *list = new_node;
    
    return 0;
}

int tk_list_pop_front(tk_node_t **list, int flag)
{
    tk_node_t *tmp;
    
    if ((*list)->next == NULL) return 1;
    tmp = *list;
    
    *list = tmp->next;
    if (flag) free(tmp->data);
    free(tmp);
    
    return 0;
}

int tk_list_push_back(tk_node_t *list, void* data)
{
    tk_node_t *new_node, *back_node;
    
    new_node = malloc(sizeof(tk_node_t));
    if (!new_node){
        return -1;
    }
    
    new_node->data = data;
    new_node->next = NULL;
    
    back_node = tk_list_get_back(list); 
    back_node->next = new_node;
    
    return 0;
}

int tk_list_pop_back(tk_node_t *list, int flag)
{
    tk_node_t *current;
    
    if (list->next == NULL) return 1;
    
    current = list;
    while (current->next->next != NULL){
        current = current->next;
    }
    
    if (flag) free(current->next->data);
    free(current->next);
    current->next = NULL;
    
    return 0;
}

tk_node_t* tk_list_get_back(tk_node_t *list)
{
    tk_node_t *current;
    
    current = list;
    while (current->next != NULL){
        current = current->next;
    }
    
    return current;
}

tk_node_t* tk_list_find(tk_node_t *list, void *data)
{
    tk_node_t *current;
    
    current = list;
    while (current != NULL && current->data != data){
        current = current->next;
    }
    
    /* If current is null, it means the function could not find the data */
    return current;
}

int tk_list_insert_after(tk_node_t *list, void* data_to_insert, void *data_to_find)
{
    tk_node_t *current, *node_to_insert;
    
    current = tk_list_find(list, data_to_find);
    if (!current){
        return -1; /* return -1 on fail to find the node*/
    }
    
    node_to_insert = malloc(sizeof(tk_node_t));
    if (!node_to_insert){
        return -1;
    }
    
    node_to_insert->data = data_to_insert;
    node_to_insert->next = current->next;
    current->next = node_to_insert;
    
    return 0;
}
