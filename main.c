#include "tk_sdl.h"
#include "tk_math.h"
#include "tk_collision.h"

#define PADDLE_SPEED 550

typedef struct entity{
    float x, y;
    float dx, dy;
    int w;
    int h;
}entity_t;

int main(int argc, char *argv[])
{
    double dt;
    
    tkmt_srand();
    
    tk_app_init("PongC", 800, 600);
    tk_set_fps_target(300);
    
    /* Player & ball Initialization */
    entity_t p1;
    p1.w = 15;
    p1.h = 60;
    p1.x = (float)p1.w;
    p1.y = (float)((tk_get_window_height() /2) - (p1.w / 2));
    p1.dx = p1.dy = 0.0;
    
    entity_t p2;
    p2.w = 15;
    p2.h = 60;
    p2.x = (float)tk_get_window_width() - (p2.w * 2);
    p2.y = (float)((tk_get_window_height() /2) - (p2.w / 2));
    p2.dx = p2.dy = 0.0;
    
    entity_t ball;
    ball.w = ball.h = 15;
    ball.x = (float)((tk_get_window_width() / 2) - (ball.w / 2));
    ball.y = (float)((tk_get_window_height() / 2) - (ball.h / 2));
    ball.dx = (tkmt_rand(0,1)) ? -500.0 : 500.0;
    ball.dy = tkmt_randf(-150.0, 150.0);
    
    while (!tk_app_should_quit()){
        dt = tk_get_deltatime();
        p1.dy = p2.dy = 0.0;
        /* Handing the user input */
        if (tk_is_key_down(TK_KEY_ESC)){
            tk_set_should_quit();
        }
        if (tk_is_key_down(TK_KEY_UP)){
            p2.dy = -PADDLE_SPEED;
        }
        if (tk_is_key_down(TK_KEY_DOWN)){
            p2.dy = PADDLE_SPEED;
        }
        if (tk_is_key_down(TK_KEY_W)){
            p1.dy = -PADDLE_SPEED;
        }
        if (tk_is_key_down(TK_KEY_S)){
            p1.dy = PADDLE_SPEED;
        }
        
        /* update player position */
        p1.y = tkmt_clampf(p1.y + (p1.dy * dt), 0.0, (float)(tk_get_window_height() - p1.h));
        p2.y = tkmt_clampf(p2.y + (p2.dy * dt), 0.0, (float)(tk_get_window_height() - p2.h));
        
        /* Update ball position */
        ball.y += ball.dy * dt;
        ball.x += ball.dx * dt;
        
        /* Handling collision */
        if (ball.y < 0 || ball.y + ball.h >= tk_get_window_height()){
            ball.dy *= -1;
        }
        
        if (ball.x + ball.w < 0 || ball.x >= tk_get_window_width()){
            ball.x = (float)((tk_get_window_width() / 2) - (ball.w / 2));
            ball.y = (float)((tk_get_window_height() / 2) - (ball.h / 2));
            ball.dx = (tkmt_rand(0,1)) ? -500.0 : 500.0;
            ball.dy = tkmt_randf(-150.0, 150.0);
        }
        
        if (tkcol_rect_vs_rect(p1.x, p1.y, p1.w, p1.h, ball.x, ball.y, ball.w, ball.h)){
            ball.x = (p1.x + p1.w) + 5;
            ball.dx *= -1.02;
            if (ball.dy < 0){
                ball.dy = tkmt_randf(-350, 0);
            }else{
                ball.dy = tkmt_randf(0, 350);
            }
        }else if(tkcol_rect_vs_rect(p2.x, p2.y, p2.w, p2.h, ball.x, ball.y, ball.w,ball.h)){
            ball.x = (p2.x - (ball.w)) -5;
            ball.dx *= -1.02;
            if (ball.dy < 0){
                ball.dy = tkmt_randf(-350, 0);
            }else{
                ball.dy = tkmt_randf(0, 350);
            }
        }
        
        /* Rendering */
        tk_clear_screen(BLACK);
        tk_draw_line(tk_get_window_width() / 2, 0,
                     tk_get_window_width() / 2, tk_get_window_height(), PEARL);
        tk_draw_rect(p1.x, p1.y, p1.w, p1.h, RED);
        tk_draw_rect(p2.x, p2.y, p2.w, p2.h, BLUE);
        tk_draw_rect(ball.x, ball.y, ball.w, ball.h, GRAY);
        tk_end_drawing();
    }
    
    tk_app_destroy();
    
    return 0;
}