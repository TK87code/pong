/* date = December 17th 2024 1:08 am */

#ifndef TK_SDL_H
#define TK_SDL_H

#include <SDL.h>
#include <stdbool.h>

/* === Colors === */
#define WHITE "fcfcfcff"
#define BLACK "000000ff"
#define PEARL "f8f8f8ff"
#define LIGHTGRAY "bcbcbcff"
#define GRAY "7c7c7cff"
#define PALEBLUE "a4e4fcff"
#define SKYBLUE "3cbcfcff"
#define LIGHTBLUE "0078f8ff"
#define BLUE "0000fcff"

/* === Key IDs === */
typedef enum tk_key_id{
    TK_KEY_UP,
    TK_KEY_DOWN,
    TK_KEY_W,
    TK_KEY_S,
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
/* === Input related === */
extern bool tk_is_key_down(tk_key_id_t key);
/* === Drawing functions === */
extern void tk_clear_screen(char *color);
extern void tk_draw_rect(int x, int y, int w, int h, char *color);
extern void tk_draw_line(int x1, int y1, int x2, int y2, char *color);
extern void tk_end_drawing(void);

#endif /* TK_SDL_H */
