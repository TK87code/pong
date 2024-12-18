#include "tk_sdl.h"
#include "tk_math.h"
#include "tk_collision.h"

#define PADDLE_SPEED 550

typedef enum game_state{
    COUNTDOWN,
    PLAY,
}game_state_t;

typedef struct entity{
    float x, y;
    float dx, dy;
    int w;
    int h;
}entity_t;

void launch_ball(entity_t *ball);

int main(int argc, char *argv[])
{
    double dt;
    double countdown_timer;
    game_state_t state = COUNTDOWN;
    
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
    ball.dx = 0.0;
    ball.dy = 0.0;
    
    while (!tk_app_should_quit()){
        dt = tk_get_deltatime();
        
        if (state == COUNTDOWN){
            ball.x = (float)((tk_get_window_width() / 2) - (ball.w / 2));
            ball.y = (float)((tk_get_window_height() / 2) - (ball.h / 2));
            ball.dx = ball.dy = 0.0;
            countdown_timer += dt;
            if (countdown_timer >= 3){
                launch_ball(&ball);
                state = PLAY;
                countdown_timer = 0.0;
            }
        }
        
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
        /* VS vertical walls */
        if (ball.y < 0 || ball.y + ball.h >= tk_get_window_height()){
            ball.dy *= -1;
        }
        /* VS horizontal walls */
        if (ball.x + ball.w < 0 || ball.x >= tk_get_window_width()){
            state = COUNTDOWN;
        }
        
        /* vs paddles */
        if (tkcol_rect_vs_rect(p1.x, p1.y, p1.w, p1.h, ball.x, ball.y, ball.w, ball.h) ||
            tkcol_rect_vs_rect(p2.x, p2.y, p2.w, p2.h, ball.x, ball.y, ball.w,ball.h))
        {
            ball.x = (ball.x >= tk_get_window_width() / 2) ? (p2.x - (ball.w)) -5 : (p1.x + p1.w) + 5;
            ball.dx *= -1.02;
            ball.dy = (ball.dy < 0) ? tkmt_randf(-350, 0) : tkmt_randf(0, 350);
        }
        
        /* === Rendering === */
        tk_clear_screen(BLACK);
        tk_draw_line(tk_get_window_width() / 2, 0,
                     tk_get_window_width() / 2, tk_get_window_height(), PEARL);
        /* Drawing count down */
        if (state == COUNTDOWN){
            if (countdown_timer > 0 && countdown_timer <= 1){
                tk_draw_rect(tk_get_window_width() / 2 - (15 / 2) - (15 * 2), 
                             tk_get_window_height() / 2 + (15 * 2),
                             15, 15, GREEN);
                tk_draw_rect(tk_get_window_width() / 2 - (15 / 2), 
                             tk_get_window_height() / 2 + (15 * 2),
                             15, 15, GREEN);
                tk_draw_rect(tk_get_window_width() / 2 - (15 / 2) + (15 * 2), 
                             tk_get_window_height() / 2 + (15 * 2),
                             15, 15, GREEN);
            }
            else if (countdown_timer > 1 && countdown_timer <= 2){
                tk_draw_rect(tk_get_window_width() / 2 - (15 / 2) - (15 * 2), 
                             tk_get_window_height() / 2 + (15 * 2),
                             15, 15, GREEN);
                tk_draw_rect(tk_get_window_width() / 2 - (15 / 2), 
                             tk_get_window_height() / 2 + (15 * 2),
                             15, 15, GREEN);
            }
            else if (countdown_timer > 2 && countdown_timer <= 3){
                tk_draw_rect(tk_get_window_width() / 2 - (15 / 2) - (15 * 2), 
                             tk_get_window_height() / 2 + (15 * 2),
                             15, 15, GREEN);
            }
        }
        
        /* Drawing paddle & ball */
        tk_draw_rect(p1.x, p1.y, p1.w, p1.h, RED);
        tk_draw_rect(p2.x, p2.y, p2.w, p2.h, BLUE);
        tk_draw_rect(ball.x, ball.y, ball.w, ball.h, GRAY);
        tk_end_drawing();
    }
    
    tk_app_destroy();
    
    return 0;
}

void launch_ball(entity_t *ball)
{
    ball->dx = (tkmt_rand(0,1)) ? -500.0 : 500.0;
    ball->dy = tkmt_randf(-150.0, 150.0);
}