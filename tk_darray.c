#include <stdlib.h> /* malloc, exit, size_t */
#include <string.h> /* memset, memcpy */
#include "tk_container.h"

#define DEFAULT_CAPACITY 1
#define DEFAULT_RESIZE_FACTOR 2 /* Whenever darray is full, double the size */

enum { /* Dynamic Array's header elements */
    DARRAY_CAPACITY,
    DARRAY_LENGTH,
    DARRAY_ITEM_SIZE,
    DARRAY_ELEMENT_COUNT,
};

void* tk_darray_create(size_t item_size)
{
    /*
Memory layout of darray:
size_t capacity = number of items that can be held.
size_t length = number of items that array currently has.
size_t item_size = the size of each items in bytes.
void * items
*/
    size_t header_size = DARRAY_ELEMENT_COUNT * sizeof(size_t);
    size_t array_size = DEFAULT_CAPACITY * item_size;
    size_t *new_array;
    
    new_array = malloc(header_size + array_size);
    if (!new_array) exit(1);
    memset(new_array, 0, header_size + array_size);
    
    new_array[DARRAY_CAPACITY] = DEFAULT_CAPACITY;
    new_array[DARRAY_LENGTH] = 0;
    new_array[DARRAY_ITEM_SIZE] = item_size;
    
    /* returns the address where user can actually store data */
    return (void*)(new_array + DARRAY_ELEMENT_COUNT);
}

void tk_darray_destroy(void *darray)
{
    /* Back to the adress at the very first of darray header(which was gave from malloc), and free it */
    darray = (size_t*)darray - DARRAY_ELEMENT_COUNT;
    free(darray);
}

static size_t __get_header_element(void *darray, int element)
{
    /* Back to the adress at the very first of darray header */
    size_t *arr = (size_t*)darray - DARRAY_ELEMENT_COUNT;
    
    return arr[element];
}

static void __set_header_element(void *darray, int element, size_t value)
{
    /* Back to the adress at the very first of darray header */
    size_t *arr = (size_t*)darray - DARRAY_ELEMENT_COUNT;
    
    arr[element] = value;
}

static void* __darray_resize(void *darray, int resize_factor)
{
    size_t capacity =__get_header_element(darray, DARRAY_CAPACITY);
    size_t item_size = __get_header_element(darray, DARRAY_ITEM_SIZE);
    size_t length = __get_header_element(darray, DARRAY_LENGTH);
    size_t header_size = DARRAY_ELEMENT_COUNT * sizeof(size_t);
    size_t new_array_size = header_size + capacity * item_size * resize_factor;
    size_t *resized_array;
    size_t *tmp;
    /* A address of the head of the header*/
    size_t *addr = (size_t*)darray - DARRAY_ELEMENT_COUNT;
    
    resized_array = malloc(new_array_size);
    if (!resized_array) exit(1);
    
    memset(resized_array, 0, new_array_size);
    memcpy(resized_array, addr, header_size + (length * item_size));
    
    /* Update capacity header data */
    tmp = resized_array + DARRAY_ELEMENT_COUNT;
    __set_header_element(tmp, DARRAY_CAPACITY, capacity * resize_factor);
    
    /* The address that  was given by malloc when created */
    free(addr);
    
    return (void*)(resized_array + DARRAY_ELEMENT_COUNT);
}

void tk_darray_push(void **darray, const void *item)
{
    size_t item_size = __get_header_element(*darray, DARRAY_ITEM_SIZE);
    size_t capacity = __get_header_element(*darray, DARRAY_CAPACITY);
    size_t length = __get_header_element(*darray, DARRAY_LENGTH);
    char *addr_to_add;
    
    /* Resize the darray if needed */
    if (length >= capacity){
        *darray = __darray_resize(*darray, DEFAULT_RESIZE_FACTOR);
    }
    
    /* Move the pointer to the end, casting char* (1 byte)*/
    addr_to_add = (char*)*darray + (length * item_size);
    memcpy((void*)addr_to_add, item, item_size);
    
    /* update length data in header */
    __set_header_element(*darray, DARRAY_LENGTH, length + 1);
}

size_t tk_darray_count(void *darray)
{
    return __get_header_element(darray, DARRAY_LENGTH);
}

void tk_darray_pop(void *darray)
{
    size_t item_size = __get_header_element(darray, DARRAY_ITEM_SIZE);
    size_t length = __get_header_element(darray, DARRAY_LENGTH);
    /*Move the pointer to the end, casting char* (1byte) */
    char *addr_to_delete = (char*)darray + ((length - 1) * item_size);
    memset(addr_to_delete, 0, item_size);
    
    /* Update length data in header */
    __set_header_element(darray, DARRAY_LENGTH, length - 1);
}

void tk_darray_insert_at(void** darray, void* item, size_t index){
    size_t item_size = __get_header_element(*darray, DARRAY_ITEM_SIZE);
    size_t length = __get_header_element(*darray, DARRAY_LENGTH);
    size_t capacity = __get_header_element(*darray, DARRAY_CAPACITY);
    
    /* If the darray already at the full capacity, resize */
    if (index >= capacity){
        int resize_factor = 0; 
        size_t new_capacity = capacity;
        do{
            resize_factor += 2;
            new_capacity =  new_capacity * resize_factor;
        }while (index >= new_capacity);
        
        *darray = __darray_resize(*darray, resize_factor);
        capacity = __get_header_element(*darray, DARRAY_CAPACITY);
    }
    
    char *addr = (char*)*darray;
    /* If index is not the last item in the array, copy the memory onwards */
    if (index != length - 1){
        memcpy((void*)(addr + ((index + 1) * item_size)),
               (void*)(addr + index * item_size),
               item_size * (capacity - index - 1));
    }
    
    memcpy((void*)(addr + index * item_size), item, item_size);
    
    /* Update header data*/
    __set_header_element(*darray, DARRAY_LENGTH, (index + 1 > length + 1) ? index + 1 : length + 1);
}

void tk_darray_erase_at(void* darray, size_t index)
{
    size_t item_size = __get_header_element(darray, DARRAY_ITEM_SIZE);
    size_t capacity = __get_header_element(darray, DARRAY_CAPACITY);
    size_t length = __get_header_element(darray, DARRAY_LENGTH);
    char *addr = (char*)darray;
    /*If index is not the last item in the array, copy the memory onwards */
    
    if (index != length -1){
        memcpy((void*)(addr + (index * item_size)), 
               (void*)(addr + ((index + 1) * item_size)),
               item_size * (capacity - index));
    }
    /* Erase the last item */
    tk_darray_pop(darray);
    /* Update the header */
    __set_header_element(darray, DARRAY_LENGTH, length - 1);
}