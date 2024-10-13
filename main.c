#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include "stone.h"

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
    for (int i = 0; i < 10; i++)
    {
        int x0, y0;
        int x1, y1;
        x0 = rand() % WINDOW_W;
        y0 = rand() % WINDOW_H;
        x1 = rand() % WINDOW_W;
        y1 = rand() % WINDOW_H;
        printf("(%d,%d)-(%d,%d)\n", x0, y0, x1, y1);
        draw_line(&data, x0, y0, x1, y1, 0x0000ff00);
    }
    mlx_put_image_to_window(mlx, mlx_win, data.img, 0, 0);

    mlx_loop(mlx);

    return 0;
}