#include "ticket.h"
#include <time.h> /* time */
#include <stdlib.h> /* malloc, exit, size_t, rand*/
#include <string.h> /* memset, memcpy */
#include <stdio.h> /* printf */

/* Internal Structs */
typedef struct key_state{
    /* 1 = key is down, 0 = not down */
    bool key_up;
    bool key_down;
    bool key_w;
    bool key_s;
    bool key_esc;
}key_state_t;

typedef struct app{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int window_width, window_height;
    int fps_cap;
    double deltatime;
    bool should_quit;
}app_t;

/* Globals */
static app_t app;
static key_state_t key_state;
static Uint64 now;
static Uint64 last;

/* Internal function prototypes */
static void _set_key_state(SDL_Scancode scancode, bool is_down);
static void _cap_fps(int fps);
static void _hex_to_rgba(char *hex, int* rgba);
static void _update_key_state(void);
static void _calculate_deltatime(void);

/*=== App init & destruction functions ===*/
void tk_app_init(char *title, int window_width, int window_height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    
    app.window =SDL_CreateWindow(title, 
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, 
                                 window_width, window_height, 0);
    if (!app.window){
        printf("Could not create SDL window: %s\n", SDL_GetError());
        exit(1);
    }
    
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    if (!app.renderer){
        printf("Could not create SDL renderer: %s\n", SDL_GetError());
        exit(1);
    }
    
    SDL_SetRenderDrawBlendMode(app.renderer,SDL_BLENDMODE_BLEND);
    
    app.window_width = window_width;
    app.window_height = window_height;
    
    /* Start counting timer */
    now = SDL_GetPerformanceCounter();
}

void tk_app_destroy(void)
{
    SDL_DestroyWindow(app.window);
    SDL_DestroyRenderer(app.renderer);
    SDL_Quit();
}

/* === Add data getters === */
bool tk_app_should_quit(void)
{
    return app.should_quit;
}

int tk_get_window_width(void)
{
    return app.window_width;
}

int tk_get_window_height(void)
{
    return app.window_height;
}

double tk_get_deltatime(void)
{
    return app.deltatime;
}

/* === App data setters === */
void tk_set_fps_target(int fps){
    app.fps_cap = fps;
}

void tk_set_should_quit(void)
{
    app.should_quit = true;
}

/* === Input Related functions ===*/
bool tk_is_key_down(tk_key_id_t key){
    switch (key){
        case TK_KEY_UP:{ return key_state.key_up; }break;
        case TK_KEY_DOWN:{ return key_state.key_down; }break;
        case TK_KEY_W:{ return key_state.key_w; }break;
        case TK_KEY_S:{ return key_state.key_s; }break;
        case TK_KEY_ESC:{ return key_state.key_esc; }break;
        default: { return 0; }break;
    }
}

/* === Drawing functions === */
void tk_clear_screen(char *color)
{
    int rgba[3];
    _hex_to_rgba(color, rgba);
    SDL_SetRenderDrawColor(app.renderer, rgba[0], rgba[1], rgba[2], 255);
    SDL_RenderClear(app.renderer);
}

void tk_draw_rect(int x, int y, int w, int h, char *color)
{
    int rgba[3];
    _hex_to_rgba(color, rgba);
    SDL_SetRenderDrawColor(app.renderer, rgba[0], rgba[1], rgba[2], 255);
    SDL_RenderFillRect(app.renderer, &((SDL_Rect){x, y, w, h}));
}

void tk_draw_rect_a(int x, int y, int w, int h, int alpha, char *color)
{
    int rgba[3];
    _hex_to_rgba(color, rgba);
    SDL_SetRenderDrawColor(app.renderer, rgba[0], rgba[1], rgba[2], alpha);
    SDL_RenderFillRect(app.renderer, &((SDL_Rect){x, y, w, h}));
}

void tk_draw_line(int x1, int y1, int x2, int y2, char *color)
{
    int rgba[3];
    _hex_to_rgba(color, rgba);
    SDL_SetRenderDrawColor(app.renderer, rgba[0], rgba[1], rgba[2], 255);
    SDL_RenderDrawLine(app.renderer, x1, y1, x2, y2);
}

void tk_end_drawing(void){
    SDL_RenderPresent(app.renderer);
    _cap_fps(app.fps_cap);
    _calculate_deltatime();
    _update_key_state();
}

/* === Math functions === */
int tkmt_clamp(int value_to_clamp, int min, int max)
{
    if (value_to_clamp < min) 
        return  min;
    if (value_to_clamp > max)
        return max;
    
    return value_to_clamp;
}

float tkmt_clampf(float value_to_clamp, float min, float max)
{
    if (value_to_clamp < min) 
        return  min;
    if (value_to_clamp > max)
        return max;
    
    return value_to_clamp;
}

void tkmt_srand(void)
{
    srand((unsigned int)time(NULL));
}

int tkmt_rand(int min, int max){
    return min + (int)(rand() * (max - min + 1) / (1 + RAND_MAX));
}

float tkmt_randf(float min, float max)
{
    return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}

/*=== Collition Related Functions ===*/
char tkcol_point_vs_rect(int px, int py, int rx, int ry, int rw, int rh)
{
    return (px >= rx && py >= ry && px <= rx + rw && py <= ry + rh);
}

char tkcol_rect_vs_rect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    return (x1 < x2 + w2 && x2 < x1 + w1 && y1 < y2 + h2 && y2 < y1 + h1);
}

/*=== Darray Functions ===*/
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

/*=== Linked List functions ===*/
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

/* === Internal functions === */
static void _hex_to_rgba(char *hex, int* rgba){
    int i, j;
    char tmp[2];
    
    j = 0;
    for (i = 0; i < 5; i += 2){
        tmp[0] = hex[i];
        tmp[1] = hex[i + 1];
        rgba[j] = (int)strtol(tmp, NULL, 16);
        j++;
    }
}

static void _cap_fps(int fps)
{
    int time_to_wait = (1000 / fps) - ((SDL_GetPerformanceCounter() - last) / SDL_GetPerformanceFrequency());
    
    if (time_to_wait > 0 && time_to_wait <= 1000 / fps){
        SDL_Delay(time_to_wait);
    }
}

static void _update_key_state(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT: { app.should_quit = true; }break;
            
            case SDL_KEYDOWN:{
                if (!event.key.repeat){
                    _set_key_state(event.key.keysym.scancode, 1);
                }
            }break;
            
            case SDL_KEYUP:{
                if (!event.key.repeat){
                    _set_key_state(event.key.keysym.scancode, 0);
                }
            }break;
        }
    }
}

static void _calculate_deltatime(void){
    last = now;
    now = SDL_GetPerformanceCounter();
    
    app.deltatime = ((double)(now - last) / (double)SDL_GetPerformanceFrequency());
}

static void _set_key_state(SDL_Scancode scancode, bool is_down){
    switch (scancode){
        case SDL_SCANCODE_UP:{ key_state.key_up = is_down; }break;
        case SDL_SCANCODE_DOWN:{ key_state.key_down = is_down; }break;
        case SDL_SCANCODE_W:{ key_state.key_w = is_down; }break;
        case SDL_SCANCODE_S:{ key_state.key_s = is_down; }break;
        case SDL_SCANCODE_ESCAPE:{ key_state.key_esc = is_down; }break;
        default: { }break;
    }
}