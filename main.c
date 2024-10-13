#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stone.h"

int render_next_frame(void *param)
{
    t_vars *vars = (t_vars *)param;
    int x0, y0;
    int x1, y1;

    memset(vars->img.addr, 0, WINDOW_W * WINDOW_H * (vars->img.bits_per_pixel / 8));

    x0 = rand() % WINDOW_W;
    y0 = rand() % WINDOW_H;
    x1 = rand() % WINDOW_W;
    y1 = rand() % WINDOW_H;
    draw_line(&vars->img, x0, y0, x1, y1, 0x0000ff00);

    mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->img.img, 0, 0);

    return 0;
}

int key_handler(int keycode, void *param)
{
    printf("%d\n", keycode);
    if (keycode == 65307)
    {
        exit(0);
    }
    return 0;
}

int main(void)
{
    static t_vars vars;

    vars.mlx = mlx_init();
    if (vars.mlx == NULL)
    {
        perror("Unable to initialize mlx");
        exit(1);
    }
    vars.mlx_win = mlx_new_window(vars.mlx, WINDOW_W, WINDOW_H, "HELLO");
    if (vars.mlx_win == NULL)
    {
        perror("Unable to initialize mlx_win");
        exit(1);
    }
    vars.img.img = mlx_new_image(vars.mlx, WINDOW_W, WINDOW_H);
    if (vars.img.img == NULL)
    {
        perror("Unable to initialize image");
        exit(1);
    }
    vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);

    mlx_key_hook(vars.mlx_win, key_handler, &vars);
    mlx_loop_hook(vars.mlx, render_next_frame, &vars);
    mlx_loop(vars.mlx);

    return 0;
}