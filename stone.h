#define WINDOW_W 800
#define WINDOW_H 800

typedef struct
{
    void *img;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_data;

/* drawing.c */
void my_mlx_pixel_put(t_data *data, int x, int y, int color);
void draw_line(t_data *data, int x0, int y0, int x1, int y1, int color);
