#define WINDOW_W 800
#define WINDOW_H 800

typedef struct
{
    void *img;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_img;

typedef struct
{
    void *mlx;
    void *mlx_win;
    t_img img;
} t_vars;

/* drawing.c */
void my_mlx_pixel_put(t_img *data, int x, int y, int color);
void draw_line(t_img *data, int x0, int y0, int x1, int y1, int color);
