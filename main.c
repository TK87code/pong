#include <SDL.h>
#include <stdlib.h> /* exit()*/

#define WHITE "ffffff"
#define BLACK "000000"

void draw_rect(SDL_Renderer *renderer, int x, int y, int w, int h, char *color);
void clear_screen(SDL_Renderer *renderer, char *color);

int main(int argc, char *argv[])
{
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    SDL_Window *window;
    SDL_Renderer *renderer;
    /* Box w&h */
    const int box_w = 15;
    const int box_h = 60;
    /* Player variables */
    const int p1_x = box_w;
    const int p2_x = WINDOW_WIDTH - (box_w * 2);
    int p1_y, p2_y;
    p1_y = p2_y = (WINDOW_HEIGHT /2) - (box_h / 2);
    
    char p2_up = 0;
    
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    
    
    while (1){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT: { exit(0); }break;
                
                case SDL_KEYDOWN:{
                    if (event.key.repeat == 0){
                        if (event.key.keysym.scancode == SDL_SCANCODE_UP){
                            p2_up = 1;
                        }
                    }
                }break;
                
                case SDL_KEYUP:{
                    if (event.key.repeat == 0){
                        if (event.key.keysym.scancode == SDL_SCANCODE_UP){
                            p2_up = 0;
                        }
                    }
                }
            }
        }
        
        if (p2_up){
            p2_y -= 4;
        }
        
        clear_screen(renderer, BLACK);
        
        draw_rect(renderer, p1_x, p1_y, box_w, box_h, WHITE);
        draw_rect(renderer, p2_x, p2_y, box_w, box_h, WHITE);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


void hex_to_dec(char *hex, int* rgba){
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

void draw_rect(SDL_Renderer *renderer, int x, int y, int w, int h, char *color)
{
    int rgba[4];
    hex_to_dec(color, rgba);
    
    SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    SDL_RenderFillRect(renderer, &((SDL_Rect){x, y, w, h}));
}

void clear_screen(SDL_Renderer *renderer, char *color)
{
    int rgba[4];
    hex_to_dec(color, rgba);
    SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    SDL_RenderClear(renderer);
}