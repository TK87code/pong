#include "tk_sdl.h"

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

/*=== App init & destruction ===*/

void tk_app_init(char *title, int window_width, int window_height)
{
    SDL_Init(SDL_INIT_VIDEO);
    app.window =SDL_CreateWindow(title, 
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, 
                                 window_width, window_height, 0);
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    
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

static void _calculate_deltatime(void){
    last = now;
    now = SDL_GetPerformanceCounter();
    
    app.deltatime = ((double)(now - last) / (double)SDL_GetPerformanceFrequency());
}

double tk_get_deltatime(void)
{
    return app.deltatime;
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

static void _cap_fps(int fps)
{
    int time_to_wait = (1000 / fps) - ((SDL_GetPerformanceCounter() - last) / SDL_GetPerformanceFrequency());
    
    if (time_to_wait > 0 && time_to_wait <= 1000 / fps){
        SDL_Delay(time_to_wait);
    }
}

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

void tk_clear_screen(char *color)
{
    int rgba[3];
    _hex_to_rgba(color, rgba);
    SDL_SetRenderDrawColor(app.renderer, rgba[0], rgba[1], rgba[2], 255);
    SDL_RenderClear(app.renderer);
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

void tk_set_fps_target(int fps){
    app.fps_cap = fps;
}

void tk_set_should_quit(void)
{
    app.should_quit = true;
}