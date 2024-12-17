/* date = December 15th 2024 1:18 pm */

#ifndef TK_COLLISION_H
#define TK_COLLISION_H


char tkcol_point_vs_rect(int px, int py, int rx, int ry, int rw, int rh);
char tkcol_rect_vs_rect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

#endif /* TK_COLLISION_H */
