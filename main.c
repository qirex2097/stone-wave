#include <mlx.h>
#include <stdio.h>

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

void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char *dst;
    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}

void draw_line(t_data *data, int x0, int y0, int x1, int y1, int color)
{
    int dx, dy;
    int sx, sy;
    int err;

    if (x0 < x1)
    {
        dx = x1 - x0;
        sx = 1;
    }
    else
    {
        dx = x0 - x1;
        sx = -1;
    }
    if (y0 < y1)
    {
        dy = y1 - y0;
        sy = 1;
    }
    else
    {
        dy = y0 - y1;
        sy = -1;
    }
    err = dx - dy;

    while (x0 != x1 && y0 != y1)
    {
        int e2;

        my_mlx_pixel_put(data, x0, y0, color);
        e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

int main(void)
{
    void *mlx;
    void *mlx_win;
    t_data data;

    mlx = mlx_init();
    if (mlx == NULL)
        perror("Unable to initialize mlx");
    mlx_win = mlx_new_window(mlx, WINDOW_W, WINDOW_H, "HELLO");
    if (mlx_win == NULL)
        perror("Unable to initialize mlx_win");
    data.img = mlx_new_image(mlx, WINDOW_W, WINDOW_H);
    if (data.img == NULL)
        perror("Unable to initialize image");
    data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel, &data.line_length, &data.endian);

    my_mlx_pixel_put(&data, 5, 5, 0x00FF0000);
    draw_line(&data, 5, 5, WINDOW_W - 5, WINDOW_H - 5, 0x0000ff00);
    mlx_put_image_to_window(mlx, mlx_win, data.img, 0, 0);

    mlx_loop(mlx);

    return 0;
}