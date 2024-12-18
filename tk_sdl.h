/* date = December 17th 2024 1:08 am */

#ifndef TK_SDL_H
#define TK_SDL_H

#include <SDL.h>
#include <stdbool.h>

/* === Colors === */
#define WHITE "fcfcfc"
#define BLACK "000000"
#define PEARL "f8f8f8"
#define LIGHTGRAY "bcbcbc"
#define GRAY "7c7c7c"
#define PALEBLUE "a4e4fc"
#define SKYBLUE "3cbcfc"
#define LIGHTBLUE "0078f8"
#define BLUE "0000fc"
#define RED "a80020"
#define GREEN "00a800"
#define YELLOW "f8b800"
#define BROWN "503000"

/* === Key IDs === */
typedef enum tk_key_id{
    TK_KEY_UP,
    TK_KEY_DOWN,
    TK_KEY_W,
    TK_KEY_S,
    TK_KEY_ESC,
}tk_key_id_t;

/* === App init & destrution === */
extern void tk_app_init(char *title, int window_width, int window_height);
extern void tk_app_destroy(void);
/* === App data getters === */
extern bool tk_app_should_quit(void);
extern int tk_get_window_width(void);
extern int tk_get_window_height(void);
extern double tk_get_deltatime(void);
/* === App data setters === */
extern void tk_set_fps_target(int fps);
extern void tk_set_should_quit(void);
/* === Input related === */
extern bool tk_is_key_down(tk_key_id_t key);
/* === Drawing functions === */
extern void tk_clear_screen(char *color);
extern void tk_draw_rect(int x, int y, int w, int h, char *color);
void tk_draw_rect_a(int x, int y, int w, int h, int alpha, char *color);
extern void tk_draw_line(int x1, int y1, int x2, int y2, char *color);
extern void tk_end_drawing(void);

#endif /* TK_SDL_H */
