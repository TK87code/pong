/* date = December 17th 2024 1:08 am */

#ifndef TICKET_H
#define TICKET_H

/* Includes */
#include <SDL.h>
#include <stdbool.h>
#include <stddef.h> /* size_t */

/* === Colors === */
#define WHITE "fcfcfc"
#define BLACK "000000"
#define PEARL "f8f8f8"
#define LIGHTGRAY "bcbcbc"
#define GRAY "7c7c7c"
#define PALEBLUE "a4e4fc"
#define SKYBLUE "3cbcfc"
#define LIGHTBLUE "0078f8"
#define BLUE "0000fc"
#define RED "a80020"
#define GREEN "00a800"
#define YELLOW "f8b800"
#define BROWN "503000"

/* === Structs === */
typedef enum tk_key_id{ /* Key ids */
    TK_KEY_UP,
    TK_KEY_DOWN,
    TK_KEY_W,
    TK_KEY_S,
    TK_KEY_ESC,
}tk_key_id_t;

typedef struct tk_node_t{ /* A node of linked list */
    void *data;
    struct tk_node_t *next;
}tk_node_t;

/* === App init & destrution functions === */
extern void tk_app_init(char *title, int window_width, int window_height);
extern void tk_app_destroy(void);

/* === App data getters === */
extern bool tk_app_should_quit(void);
extern int tk_get_window_width(void);
extern int tk_get_window_height(void);
extern double tk_get_deltatime(void);

/* === App data setters === */
extern void tk_set_fps_target(int fps);
extern void tk_set_should_quit(void);

/* === Input related === */
extern bool tk_is_key_down(tk_key_id_t key);

/* === Drawing functions === */
extern void tk_clear_screen(char *color);
extern void tk_draw_rect(int x, int y, int w, int h, char *color);
void tk_draw_rect_a(int x, int y, int w, int h, int alpha, char *color);
extern void tk_draw_line(int x1, int y1, int x2, int y2, char *color);
extern void tk_end_drawing(void);

/* === Math functions === */
/**
* @brief Clamp the passed value.
* @param value_to_clamp A value to clamp.
* @param A minimum value to return.
* @param A maximum value to return.
* @return A clamped value.
*/
int tkmt_clamp(int value_to_clamp, int min, int max);
/**
* @brief Clamp the passed value.
* @param value_to_clamp A value to clamp.
* @param A minimum value to return.
* @param A maximum value to return.
* @return A clamped value.
*/
float tkmt_clampf(float value_to_clamp, float min, float max);

/**
* @brief Set random seed with current time
*/
void tkmt_srand(void);

/**
* @brief Return pseudo randomly generated integer.
 * @param min A minimum value of random value
* @param max A maximum value of random value
*/
int tkmt_rand(int min, int max);

/**
* @brief Return pseudo randomly generated float.
 * @param min A minimum value of random value
* @param max A maximum value of random value
*/
float tkmt_randf(float min, float max);

/*=== Collition related functions ===*/
char tkcol_point_vs_rect(int px, int py, int rx, int ry, int rw, int rh);
char tkcol_rect_vs_rect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

/*=== Dynamic Array functions ===*/
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

/* === Linked List functions ===*/
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
#endif /* TICKET_H */

