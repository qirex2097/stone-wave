#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/X.h>
#include "stone.h"

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

void cleanup(t_vars *vars);

int render_next_frame(void *param)
{
    t_vars *vars = (t_vars *)param;
    t_img *img = (t_img *)&vars->img;

    mlx_put_image_to_window(vars->mlx, vars->mlx_win, img->img, 0, 0);

    update_player(vars);

    memset(img->addr, 0, WINDOW_W * WINDOW_H * (img->bits_per_pixel / 8));
    draw_player(vars);
    draw_wall(vars);

    return 0;
}

int key_press_handler(int keycode, void *param)
{
    t_vars *vars = param;
    t_mouse *mouse = &vars->mouse;

    // printf("press:%d\n", keycode);

    if (keycode == KEY_LEFT)
        mouse->button |= TURN_LEFT;
    if (keycode == KEY_RIGHT)
        mouse->button |= TURN_RIGHT;
    if (keycode == KEY_W)
        mouse->button |= MOVE_UP;
    if (keycode == KEY_S)
        mouse->button |= MOVE_DOWN;
    if (keycode == KEY_A)
        mouse->button |= MOVE_LEFT;
    if (keycode == KEY_D)
        mouse->button |= MOVE_RIGHT;

    return 0;
}

int key_release_handler(int keycode, void *param)
{
    t_vars *vars = param;
    t_mouse *mouse = &vars->mouse;

    // printf("release:%d\n", keycode);
    if (keycode == KEY_ESC)
    {
        cleanup(vars);
        exit(0);
    }

    if (keycode == KEY_LEFT)
        mouse->button &= ~(TURN_LEFT);
    if (keycode == KEY_RIGHT)
        mouse->button &= ~(TURN_RIGHT);
    if (keycode == KEY_W)
        mouse->button &= ~(MOVE_UP);
    if (keycode == KEY_S)
        mouse->button &= ~(MOVE_DOWN);
    if (keycode == KEY_A)
        mouse->button &= ~(MOVE_LEFT);
    if (keycode == KEY_D)
        mouse->button &= ~(MOVE_RIGHT);

    return 0;
}

int mouse_down_handler(int button, int x, int y, void *param)
{
    t_vars *vars = param;
    t_mouse *mouse = &vars->mouse;

    if (button == BUTTON_LEFT)
    {
        mouse->pos.x = x;
        mouse->pos.y = y;
        mouse->button |= CENTER;
    }
    if (button == BUTTON_UP)
    {
        mouse->button |= SCALE_UP;
    }
    if (button == BUTTON_DOWN)
    {
        mouse->button |= SCALE_DOWN;
    }

    return 0;
}

void cleanup(t_vars *vars)
{
    mlx_destroy_image(vars->mlx, vars->img.img);
    mlx_destroy_window(vars->mlx, vars->mlx_win);
    mlx_destroy_display(vars->mlx);
    free(vars->mlx);
}

int cross_button_handler(void *param)
{
    t_vars *vars = param;
    cleanup(vars);
    exit(0);
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
    vars.mlx_win = mlx_new_window(vars.mlx, WINDOW_W, WINDOW_H, "Press ESC to exit");
    if (vars.mlx_win == NULL)
    {
        perror("Unable to initialize mlx_win");
        mlx_destroy_display(vars.mlx);
        free(vars.mlx);
        exit(1);
    }
    vars.img.img = mlx_new_image(vars.mlx, WINDOW_W, WINDOW_H);
    if (vars.img.img == NULL)
    {
        perror("Unable to initialize image");
        mlx_destroy_window(vars.mlx, vars.mlx_win);
        mlx_destroy_display(vars.mlx);
        free(vars.mlx);
        exit(1);
    }
    vars.img.w = WINDOW_W;
    vars.img.h = WINDOW_H;
    vars.img.field_x = 0 - FIELD_W / 2;
    vars.img.field_y = 0 - FIELD_H / 2;
    vars.img.field_w = FIELD_W;
    vars.img.field_h = FIELD_H;
    vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);

    init_player(&vars.player);
    init_wall();
    init_mouse(&vars.mouse);

    mlx_do_key_autorepeatoff(vars.mlx);
    mlx_hook(vars.mlx_win, KeyPress, KeyPressMask, key_press_handler, &vars);
    mlx_hook(vars.mlx_win, KeyRelease, KeyReleaseMask, key_release_handler, &vars);
    // mlx_hook(vars.mlx_win, MotionNotify, PointerMotionMask, mouse_move_handler, &vars);
    mlx_hook(vars.mlx_win, ButtonPress, ButtonPressMask, mouse_down_handler, &vars);
    // mlx_hook(vars.mlx_win, ButtonRelease, ButtonReleaseMask, mouse_up_handler, &vars);
    mlx_loop_hook(vars.mlx, render_next_frame, &vars);
    mlx_hook(vars.mlx_win, DestroyNotify, StructureNotifyMask, cross_button_handler, &vars);
    mlx_loop(vars.mlx);

    return 0;
}