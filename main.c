#include <SDL.h>
#include <stdlib.h> /* exit()*/
#include <stdbool.h>
#include "tk_math.h"

#define WHITE "ffffffff"
#define BLACK "00000000"

typedef enum tk_key_id{
    TK_KEY_UP,
    TK_KEY_DOWN,
    TK_KEY_W,
    TK_KEY_S,
}tk_key_id_t;

typedef struct key_state{
    /* 1 = key is down, 0 = not down */
    bool key_up;
    bool key_down;
    bool key_w;
    bool key_s;
}key_state_t;

/* === Function Prototypes ===*/
void tk_draw_rect(SDL_Renderer *renderer, int x, int y, int w, int h, char *color);
void tk_clear_screen(SDL_Renderer *renderer, char *color);
bool tk_is_key_down(tk_key_id_t key);
void tk_update_key_state(void);
void tk_draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, char *color);

/* === Global Variables === */
key_state_t key_state;
bool is_runnnig = 1;

int main(int argc, char *argv[])
{
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    SDL_Window *window;
    SDL_Renderer *renderer;
    /* Box variables */
    const int box_w = 15;
    const int box_h = 60;
    const int box_speed = 8;
    /* Player variables */
    const int p1_x = box_w;
    const int p2_x = WINDOW_WIDTH - (box_w * 2);
    int p1_y, p2_y;
    p1_y = p2_y = (WINDOW_HEIGHT /2) - (box_h / 2);
    
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    
    while (is_runnnig){
        tk_update_key_state();
        
        /* Handing the user input */
        if (tk_is_key_down(TK_KEY_UP)){
            p2_y = tkmt_clamp(p2_y - box_speed, 0, WINDOW_HEIGHT - box_h);
        }
        if (tk_is_key_down(TK_KEY_DOWN)){
            p2_y = tkmt_clamp(p2_y + box_speed, 0, WINDOW_HEIGHT - box_h);
        }
        
        if (tk_is_key_down(TK_KEY_W)){
            p1_y = tkmt_clamp(p1_y - box_speed, 0, WINDOW_HEIGHT - box_h);
        }
        if (tk_is_key_down(TK_KEY_S)){
            p1_y = tkmt_clamp(p1_y + box_speed, 0, WINDOW_HEIGHT - box_h);
        }
        
        tk_clear_screen(renderer, BLACK);
        
        tk_draw_line(renderer, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT, WHITE);
        tk_draw_rect(renderer, p1_x, p1_y, box_w, box_h, WHITE);
        tk_draw_rect(renderer, p2_x, p2_y, box_w, box_h, WHITE);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


void _set_key_state(SDL_Scancode scancode, bool is_down){
    switch (scancode){
        case SDL_SCANCODE_UP:{ key_state.key_up = is_down; }break;
        case SDL_SCANCODE_DOWN:{ key_state.key_down = is_down; }break;
        case SDL_SCANCODE_W:{ key_state.key_w = is_down; }break;
        case SDL_SCANCODE_S:{ key_state.key_s = is_down; }break;
        default: { }break;
    }
}

void tk_update_key_state(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT: { is_runnnig = 0; }break;
            
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

bool tk_is_key_down(tk_key_id_t key){
    switch (key){
        case TK_KEY_UP:{ return key_state.key_up; }break;
        case TK_KEY_DOWN:{ return key_state.key_down; }break;
        case TK_KEY_W:{ return key_state.key_w; }break;
        case TK_KEY_S:{ return key_state.key_s; }break;
        default: { return 0; }break;
    }
}

void _hex_to_rgba(char *hex, int* rgba){
    int i, j;
    char tmp[2];
    
    j = 0;
    for (i = 0; i < 7; i += 2){
        tmp[0] = hex[i];
        tmp[1] = hex[i + 1];
        rgba[j] = (int)strtol(tmp, NULL, 16);
        j++;
    }
}

void tk_draw_rect(SDL_Renderer *renderer, int x, int y, int w, int h, char *color)
{
    int rgba[4];
    _hex_to_rgba(color, rgba);
    SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    SDL_RenderFillRect(renderer, &((SDL_Rect){x, y, w, h}));
}

void tk_clear_screen(SDL_Renderer *renderer, char *color)
{
    int rgba[4];
    _hex_to_rgba(color, rgba);
    SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    SDL_RenderClear(renderer);
}

void tk_draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, char *color)
{
    int rgba[4];
    _hex_to_rgba(color, rgba);
    SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}