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

typedef struct player{
    float x, y;
    float dy;
    int w;
    int h;
    float speed;
}player_t;

/* === FunctionPrototypes ===*/
void tk_draw_rect(SDL_Renderer *renderer, int x, int y, int w, int h, char *color);
void tk_clear_screen(SDL_Renderer *renderer, char *color);
bool tk_is_key_down(tk_key_id_t key);
void tk_update_key_state(void);
void tk_draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, char *color);
double tk_get_deltatime(void);
void tk_cap_fps(int fps);

/* === Global Variables === */
key_state_t key_state;
bool is_runnnig = 1;
Uint64 now;
Uint64 last;

int main(int argc, char *argv[])
{
    double deltatime = 0.0;
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    SDL_Window *window;
    SDL_Renderer *renderer;
    
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    now = SDL_GetPerformanceCounter();
    
    /* Player Initialization */
    player_t p1;
    p1.w = 15;
    p1.h = 60;
    p1.x = (float)p1.w;
    p1.y = (float)((WINDOW_HEIGHT /2) - (p1.w / 2));
    p1.dy = 0.0;
    p1.speed = 350.0;
    
    player_t p2;
    p2.w = 15;
    p2.h = 60;
    p2.x = (float)WINDOW_WIDTH - (p2.w * 2);
    p2.y = (float)((WINDOW_HEIGHT /2) - (p2.w / 2));
    p2.dy = 0.0;
    p2.speed = 350.0;
    
    while (is_runnnig){
        tk_cap_fps(144);
        deltatime = tk_get_deltatime();
        tk_update_key_state();
        
        p1.dy = p2.dy = 0.0;
        /* Handing the user input */
        if (tk_is_key_down(TK_KEY_UP)){
            p2.dy = -1 * p2.speed;
        }
        if (tk_is_key_down(TK_KEY_DOWN)){
            p2.dy = p2.speed;
        }
        
        if (tk_is_key_down(TK_KEY_W)){
            p1.dy = -1 * p1.speed;
        }
        if (tk_is_key_down(TK_KEY_S)){
            p1.dy = p1.speed;
        }
        
        /* update player position */
        p1.y = tkmt_clampf(p1.y + (p1.dy * deltatime), 0.0, (float)(WINDOW_HEIGHT - p1.h));
        p2.y = tkmt_clampf(p2.y + (p2.dy * deltatime), 0.0, (float)(WINDOW_HEIGHT - p2.h));
        
        tk_clear_screen(renderer, BLACK);
        
        tk_draw_line(renderer, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT, WHITE);
        tk_draw_rect(renderer, p1.x, p1.y, p1.w, p1.h, WHITE);
        tk_draw_rect(renderer, p2.x, p2.y, p2.w, p2.h, WHITE);
        
        SDL_RenderPresent(renderer);
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

/* Get deltatime in seconds */
double tk_get_deltatime(void){
    last = now;
    now = SDL_GetPerformanceCounter();
    
    return ((double)(now - last) / (double)SDL_GetPerformanceFrequency());
}

void tk_cap_fps(int fps)
{
    int time_to_wait = (1000 / fps) - ((SDL_GetPerformanceCounter() - last) / SDL_GetPerformanceFrequency());
    
    if (time_to_wait > 0 && time_to_wait <= 1000 / fps){
        SDL_Delay(time_to_wait);
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