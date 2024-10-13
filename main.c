#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/X.h>
#include "stone.h"

#define BUTTON_LEFT 1
#define BUTTON_RIGHT 3

int cleanup(t_vars *vars);

int render_next_frame(void *param)
{
    t_vars *vars = (t_vars *)param;

    update_player(vars);

    memset(vars->img.addr, 0, WINDOW_W * WINDOW_H * (vars->img.bits_per_pixel / 8));
    draw_player(vars);
    mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->img.img, 0, 0);

    return 0;
}

int key_handler(int keycode, void *param)
{
    t_vars *vars = param;

    printf("%d\n", keycode);
    if (keycode == 65307)
    {
        cleanup(vars);
        exit(0);
    }
    return 0;
}

int mouse_move_handler(int x, int y, void *param)
{
    t_vars *vars = (t_vars *)param;
    t_mouse *mouse = &vars->mouse;
    mouse->x = x;
    mouse->y = y;
    return 0;
}

int mouse_down_handler(int button, int x, int y, void *param)
{
    t_vars *vars = param;
    t_mouse *mouse = &vars->mouse;

    if (button == BUTTON_LEFT)
        mouse->button |= BUTTON_LEFT_ON;
    if (button == BUTTON_RIGHT)
        mouse->button |= BUTTON_RIGHT_ON;
    return 0;
}

int mouse_up_handler(int button, int x, int y, void *param)
{
    t_vars *vars = param;
    t_mouse *mouse = &vars->mouse;

    if (button == BUTTON_LEFT)
        mouse->button &= ~(BUTTON_LEFT_ON);
    if (button == BUTTON_RIGHT)
        mouse->button &= ~(BUTTON_RIGHT_ON);
    return 0;
}

int cleanup(t_vars *vars)
{
    mlx_destroy_image(vars->mlx, vars->img.img);
    mlx_destroy_window(vars->mlx, vars->mlx_win);
    mlx_destroy_display(vars->mlx);
    free(vars->mlx);
    return 0;
}

int main(void)
{
    t_vars vars;

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
    vars.img.w = WINDOW_W;
    vars.img.h = WINDOW_H;
    if (vars.img.img == NULL)
    {
        perror("Unable to initialize image");
        exit(1);
    }
    vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);

    init_player(&vars.player);
    init_mouse(&vars.mouse);

    mlx_key_hook(vars.mlx_win, key_handler, &vars);
    mlx_loop_hook(vars.mlx, render_next_frame, &vars);
    mlx_hook(vars.mlx_win, MotionNotify, PointerMotionMask, mouse_move_handler, &vars);
    mlx_hook(vars.mlx_win, ButtonPress, ButtonPressMask, mouse_down_handler, &vars);
    mlx_hook(vars.mlx_win, ButtonRelease, ButtonReleaseMask, mouse_up_handler, &vars);
    mlx_loop(vars.mlx);

    return 0;
}