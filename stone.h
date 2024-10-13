#define WINDOW_W 800
#define WINDOW_H 800

#define BUTTON_LEFT_ON (1L << 0)
#define BUTTON_RIGHT_ON (1L << 1)

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
    int x;
    int y;
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
void draw_line(t_img *data, int x0, int y0, int x1, int y1, int color);
void draw_circle(t_img *img, int x_center, int y_center, int radius, int color);

/* player.c */
int init_player(t_player *player);
int init_mouse(t_mouse *mouse);
int update_player(t_vars *vars);
int draw_player(t_vars *vars);
