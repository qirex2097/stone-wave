#ifndef _STONE_H_
#define _STONE_H_

#define FIELD_W 1000
#define FIELD_H 1000
#define WINDOW_W 800
#define WINDOW_H 800

#define BUTTON_LEFT_ON (1L << 0)
#define BUTTON_RIGHT_ON (1L << 1)
#define PI 3.14159265358979323846

#define COLLINEAR 0
#define CLOCKWISE 1
#define COUNTERCLOCKWISE 2

typedef struct
{
    int x, y;
} t_pos;

typedef struct
{
    t_pos p0, p1;
} t_line;

typedef struct
{
    void *img;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
    int w, h;
    int field_x, field_y;
    int field_w, field_h;
} t_img;

typedef struct
{
    int x, y;
    int angle;
} t_player;

typedef struct
{
    int x, y;
    unsigned long button;
} t_mouse;

typedef struct
{
    void *mlx;
    void *mlx_win;
    t_img img;
    t_player player;
    t_mouse mouse;
} t_vars;

/* drawing.c */
void my_mlx_pixel_put(t_img *data, int x, int y, int color);
void my_mlx_draw_line(t_img *data, int x0, int y0, int x1, int y1, int color);
void draw_line_s(t_img *data, t_line *line, int color);
void draw_circle_s(t_img *img, t_pos *center, int radius, int color);
int do_intersect(t_line *line1, t_line *line2);
int get_intersection(t_line *line0, t_line *line1, t_pos *cross_point);
void convert_to_screen(t_pos *field, t_pos *screen, int x0, int y0, int w, int h);
void convert_to_field(t_pos *screen, t_pos *field, int x0, int y0, int w, int h);
void draw_line(t_img *data, t_line *line, int color);
void draw_circle(t_img *img, t_pos *center, int radius, int color);

/* player.c */
int init_player(t_player *player);
int init_mouse(t_mouse *mouse);
int update_player(t_vars *vars);
int draw_player(t_vars *vars);

#endif //_STONE_H_