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
    mlx_put_image_to_window(mlx, mlx_win, data.img, 0, 0);

    mlx_loop(mlx);

    return 0;
}