#include "ticket.h"

#define PADDLE_SPEED 550.0
#define INITIAL_BALL_SPEED 500.0

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

int main(int argc, char *argv[])
{
    int i, j;                       /* For looping */
    double projectile_timer = 0.0;  /* For saving projectile pos at fixed seconds */
    double countdown_timer;         /* For countdown */
    double dt;                      /* Deltatime */
    game_state_t state = COUNTDOWN; /* Game State */
    entity_t *projectile;           /* A container of projectiles */
    
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
    
    projectile = (entity_t*)tk_darray_create(sizeof(entity_t));
    
    while (!tk_app_should_quit()){
        dt = tk_get_deltatime();
        projectile_timer += dt;
        
        if (state == COUNTDOWN){
            ball.x = (float)((tk_get_window_width() / 2) - (ball.w / 2));
            ball.y = (float)((tk_get_window_height() / 2) - (ball.h / 2));
            ball.dx = ball.dy = 0.0;
            countdown_timer += dt;
            if (countdown_timer >= 3){
                /* Launching a ball */
                ball.dx = (tkmt_rand(0,1)) ? -1 * INITIAL_BALL_SPEED : INITIAL_BALL_SPEED;
                ball.dy = tkmt_randf(-150.0, 150.0);
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
        
        /* add push 15 old ball positions to darray every 0.1 sec */
        if (projectile_timer >= 0.01){
            if (tk_darray_count(projectile) < 15){
                tk_darray_push((void**)&projectile, &ball);
            }
            else{
                tk_darray_push((void**)&projectile, &ball);
                tk_darray_erase_at((void*)projectile, 0);
            }
            projectile_timer = 0.0;
        }
        
        
        /* Update ball position */
        ball.y += ball.dy * dt;
        ball.x += ball.dx * dt;
        
        /*=== Handling collision ===*/
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
            int i, j;
            const int middle_w = tk_get_window_width() / 2;
            const int middle_h = tk_get_window_height() / 2;
            const int square_size = 15;
            if (countdown_timer > 0 && countdown_timer <= 1){
                for (i = 0, j = -1; i < 3; i++, j++){
                    tk_draw_rect(middle_w - (square_size / 2) + (j * (square_size * 2)), 
                                 middle_h + (square_size * 2),
                                 square_size, square_size, GREEN);
                }
            }
            else if (countdown_timer > 1 && countdown_timer <= 2){
                for (i = 0, j = -1; i < 2; i++, j++){
                    tk_draw_rect(middle_w - (square_size / 2) + (j * (square_size * 2)), 
                                 middle_h + (square_size * 2),
                                 square_size, square_size, GREEN);
                }
            }
            else if (countdown_timer > 2 && countdown_timer <= 3){
                tk_draw_rect(middle_w - (square_size / 2) - (square_size * 2), 
                             middle_h + (square_size * 2),
                             square_size, square_size, GREEN);
            }
        }
        
        /* Drawing paddles */
        tk_draw_rect(p1.x, p1.y, p1.w, p1.h, RED);
        tk_draw_rect(p2.x, p2.y, p2.w, p2.h, BLUE);
        /* Drawing projectile */
        for (i = tk_darray_count(projectile), j = 0; i >= 0 ; i--, j += 5){
            tk_draw_rect_a(projectile[i].x, projectile[i].y, projectile[i].w - 5, projectile[i].h, 110 - j,WHITE);
        }
        /* Drawing a ball */
        tk_draw_rect(ball.x, ball.y, ball.w, ball.h, WHITE);
        tk_end_drawing();
    }
    
    tk_app_destroy();
    
    return 0;
}
