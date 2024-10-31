#ifndef _STONE_H_
#define _STONE_H_
#include <stdint.h>

#define FIELD_W 1000
#define FIELD_H 1000
#define WINDOW_W 800
#define WINDOW_H 800

#define TURN_LEFT (1L << 0)
#define TURN_RIGHT (1L << 1)
#define MOVE_UP (1L << 2)
#define MOVE_DOWN (1L << 3)
#define MOVE_LEFT (1L << 4)
#define MOVE_RIGHT (1L << 5)
#define SCALE_UP (1L << 6)
#define SCALE_DOWN (1L << 7)
#define CENTER (1L << 8)
#define SHOW_MINIWINDOW (1L << 9)
#define MOVE_FORWARD (1L << 10)
#define MOVE_BACKWARD (1L << 11)
#define PI 3.14159265358979323846

#define BUTTON_LEFT 1
#define BUTTON_RIGHT 3
#define BUTTON_UP 4
#define BUTTON_DOWN 5
#define KEY_ESC 65307
#define KEY_PGUP 65365
#define KEY_PGDN 65366
#define KEY_LEFT 65361
#define KEY_UP 65362
#define KEY_RIGHT 65363
#define KEY_DOWN 65364

#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_W 119
#define KEY_H 104
#define KEY_J 106
#define KEY_K 107
#define KEY_L 108

typedef enum
{
    COLLINEAR = 0,
    CLOCKWISE,
    COUNTERCLOCKWISE,
} Orientation;

typedef struct
{
    int x, y;
} t_pos_s;

typedef struct
{
    int x, y;
} t_pos, t_vec;

typedef struct
{
    int x0, y0, x1, y1;
} t_line, t_pair;

typedef struct
{
    int x, y;
    int w, h;
} t_rect;

typedef struct
{
    void *img;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
    int w, h;
} t_img;

typedef struct
{
    int x, y;
    int angle;
} t_player;

typedef struct
{
    t_line line;
    int color;
} t_wall;

typedef struct
{
    t_pos_s pos;
    uint64_t button;
} t_mouse;

typedef struct
{
    int x, y;
    int w, h;
} t_camera;

typedef struct
{
    char **data;
    int x, y;
    int w, h;
    int grid_size;
} t_map;

typedef struct
{
    void *mlx;
    void *mlx_win;
    t_camera camera;
    t_img img;
    t_img img2;
    t_player player;
    t_mouse mouse;
    int mini_x, mini_y;
    t_map *map;
} t_vars;

/* main.c */
void cleanup(t_vars *vars);

/* utils.c */
long long distance_squared(int x1, int y1, int x2, int y2);
double cosine_angle(int x1, int y1, int x2, int y2, int x3, int y3);

/* drawing.c */
void my_mlx_pixel_put(t_img *data, int x, int y, int color);
void my_mlx_draw_line(t_img *data, int x0, int y0, int x1, int y1, int color);
void draw_line_s(t_img *data, t_line *line, int color);
void draw_circle_s(t_img *img, t_pos *center, int radius, int color);
void draw_line(t_img *data, t_camera *camera, t_line *line, int color);
void draw_circle(t_img *img, t_camera *camera, t_pos *center, int radius, int color);
void draw_rect(t_img *data, t_camera *camera, t_rect *rect, int color);

/* drawing_utils.c */
int get_intersection(t_line *line0, t_line *line1, t_pos *cross_point);
void convert_to_screen(t_pos *field, t_pos_s *screen, t_img *img, t_camera *camera);
void convert_to_field(t_pos_s *screen, t_pos *field, t_img *img, t_camera *camera);
t_pos map_point_on_line(t_line *line, int w, int a);
void scale_segment(t_line *line, int a, t_line *line_new);

/* player.c */
int init_player(t_player *player, int x, int y);
int init_mouse(t_mouse *mouse);
int update_player(t_vars *vars);
int draw_player(t_vars *vars);
double cosine_angle(int x1, int y1, int x2, int y2, int x3, int y3);

/* wall.c */
int init_wall(t_wall *param, int kazu);
int update_wall(t_vars *vars);
int draw_wall(t_vars *vars);
t_wall *get_wall(int idx);
int find_intersection_point(t_vars *vars, t_line *line, t_pos *cross_point, t_wall *wall_);

/* key_hendler.c */
int key_press_handler(int keycode, void *param);
int key_release_handler(int keycode, void *param);
int mouse_down_handler(int button, int x, int y, void *param);

/* camera.c */
void init_camera(t_camera *camera, int x, int y, int w, int h);
int update_camera(t_vars *vars);

/* mini_window.c */
int init_mini_window(t_vars *vars);
int render_mini_window(t_vars *vars);
void draw_miniwindow(t_vars *vars, t_wall *wall, t_line *way, int sx);
void draw_player_view(t_vars *vars, t_line *screen);

/* map.c */
t_map *init_map(int w, int h);
void free_map(t_map *map);
int draw_map(t_vars *);
t_pos find_next_grid_crossing(t_pos ray_origin, t_vec ray_direction, int grid_size);
t_pos detect_ray_wall_intersection(t_map *map, t_pos origin, t_vec direction);
int get_wall_color(t_map *map, t_pos cross_point, t_vec direction);

/* buff.c */
void init_buff();
void cleanup_buff();
void draw_buff(t_vars *vars);
void my_string_put(const char *str);

#endif //_STONE_H_