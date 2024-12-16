#include "tk_sdl.h"
#include "tk_math.h"

typedef struct player{
    float x, y;
    float dy;
    int w;
    int h;
    float speed;
}player_t;

int main(int argc, char *argv[])
{
    double dt;
    
    tk_app_init("PongC", 800, 600);
    tk_set_fps_target(144);
    
    /* Player Initialization */
    player_t p1;
    p1.w = 15;
    p1.h = 60;
    p1.x = (float)p1.w;
    p1.y = (float)((tk_get_window_height() /2) - (p1.w / 2));
    p1.dy = 0.0;
    p1.speed = 350.0;
    
    player_t p2;
    p2.w = 15;
    p2.h = 60;
    p2.x = (float)tk_get_window_width() - (p2.w * 2);
    p2.y = (float)((tk_get_window_height() /2) - (p2.w / 2));
    p2.dy = 0.0;
    p2.speed = 350.0;
    
    while (!tk_app_should_quit()){
        dt = tk_get_deltatime();
        
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
        p1.y = tkmt_clampf(p1.y + (p1.dy * dt), 0.0, (float)(tk_get_window_height() - p1.h));
        p2.y = tkmt_clampf(p2.y + (p2.dy * dt), 0.0, (float)(tk_get_window_height() - p2.h));
        
        /* Rendering */
        tk_clear_screen(BLACK);
        tk_draw_line(tk_get_window_width() / 2, 0,
                     tk_get_window_width() / 2, tk_get_window_height(), PEARL);
        tk_draw_rect(p1.x, p1.y, p1.w, p1.h, PEARL);
        tk_draw_rect(p2.x, p2.y, p2.w, p2.h, PEARL);
        
        tk_end_drawing();
    }
    
    tk_app_destroy();
    
    return 0;
}
