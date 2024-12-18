/**
 * @file tk_container.h
* @brief This is a implementation of singly linked list, dynamic array in C.
* @author Takuya Kimura (https://github.com/TK87code)
 * @date December 4th 2024 7:11 pm 
*/
#ifndef TK_CONTAINER_H
#define TK_CONTAINER_H

#ifndef NULL
#define NULL 0
#endif /* NULL */

#include <stddef.h> /* size_t */

/* === tk_darray.c ===*/

/**
* @brief Create a dynamic array.
* @param item_size The size of the items that will be contained the array in bytes.
* @return void An adress of where user can actually store the data.
*/
void* tk_darray_create(size_t item_size);

/**
* @brief Destroy a dynamic array. This function will free the memory allocated by tk_darray_create().
* @param darray A pointer of darray.
*/
void tk_darray_destroy(void *darray);

/**
* @brief push item at the end of the darray.
* @param darray A double pointer to the darray.
* @param item A pointer to the item to push.
*/
void tk_darray_push(void **darray, const void *item);

/**
* @brief return the number of items currently contain in the array.(This is not the capacity)
* @param darray A pointer to darray.
* @return Number of items that the array currently has.
*/
size_t tk_darray_count(void *darray);

/**
* @brief Pop the last item in the darray.
* @param darray A pointer to the darray.
*/
void tk_darray_pop(void *darray);

/**
* @brief Insert a item to a darray at given index.
* @param darray A double pointer to the darray.
* @param item A pointer to the item to insert.
* @param index An Index to insert at.
*/
void tk_darray_insert_at(void** darray, void* item, size_t index);

/**
* @brief Erase a item at given index in a darray.
* @param darray A pointer to the darray.
* @param index An index to earase at.
*/
void tk_darray_erase_at(void* darray, size_t index);

/* === tk_list.c ===*/

typedef struct tk_node_t{
    void *data;
    struct tk_node_t *next;
}tk_node_t;

/**
* @brief Create a head node of a singly linked list.
 * @param data A ptr to the data to hold in the list node.
* @return A ptr to the head node of the list.
*/
extern tk_node_t* tk_list_create(void *data);

/**
* @brief Destroy the list. This function will free all allocated memory for the list, and set the list ptr to NULL.
* @param list A double ptr to the head node of the list.
* @param flag A flag to tell if you want to free the adress of data contained.(1 = true, 0 = false)
*/
extern void tk_list_destroy(tk_node_t **list, int flag);

/**
* @brief Insert new node at the front of the list.
* @param list A double ptr to the head node.
* @param data A ptr to the data to insert.
* @return return 0 on success, -1 on malloc error.
*/
extern int tk_list_push_front(tk_node_t **list, void *data);

/**
* @brief Delete the node at the front.
* @param list A double ptr to the head node.
* @param flag A flag to tell if you want to free the adress of data contained.(1 = true, 0 = false)
* @return if there is only head node left in the list, return 1. otherwise return 0 on suceess.
*/
extern int tk_list_pop_front(tk_node_t **list, int flag);

/**
* @brief Insert a new node at the end of the list.
* @param list A ptr to the head node.
* @param data A ptr to the data to insert.
* @return return 0 on success, -1 on malloc error.
*/
extern int tk_list_push_back(tk_node_t *list, void *data);

/**
* @brief Delete the node at the end of the list.
* @param list A ptr to the head node.
* @param flag A flag to tell if you want to free the adress of data contained.(1 = true, 0 = false)
* @return if there is only head node left in the list, return 1. otherwise return 0 on suceess.
*/
extern int tk_list_pop_back(tk_node_t *list, int flag);

/**
* @brief Return the node at the end of the list.
* @param list A ptr to the head node.
* @return A ptr to the node at the end of the list.
*/
extern tk_node_t* tk_list_get_back(tk_node_t *list);

/**
* @brief Find a node that contains the data passed in the param.
* @param list A ptr to the head node.
* @param data A ptr to the data to find.
* @return A ptr to the node that contains the data passed in the param. If the function could not find the node, returns NULL.
*/
extern tk_node_t* tk_list_find(tk_node_t *list, void *data);

/**
* @brief Insert a new node after the node which contains the data passed in the param.
* @param list A ptr to the head node.
* @param data_to_insert A ptr to the data to insert.
* @param data_to_find A ptr to the data to find.
* @return Return 0 on success, -1 on malloc error.
*/
extern int tk_list_insert_after(tk_node_t *list, void *data_to_insert, void *data_to_find);

#endif /* TK_CONTAINER_H */
