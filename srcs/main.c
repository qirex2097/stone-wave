#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/X.h>
#include "stone.h"

int render_next_frame(void *param)
{
    t_vars *vars = (t_vars *)param;
    t_img *img = (t_img *)&vars->img;

    mlx_put_image_to_window(vars->mlx, vars->mlx_win, img->img, 0, 0);

    update_camera(vars);
    update_player(vars);
    update_wall(vars);

    memset(img->addr, 0, WINDOW_W * WINDOW_H * (img->bits_per_pixel / 8));
    draw_player(vars);
    draw_wall(vars);

    if (vars->mouse.button & SHOW_MINIWINDOW)
    {
        render_mini_window(vars);
    }

    return 0;
}

void cleanup(t_vars *vars)
{
    if (vars->img2.img)
        mlx_destroy_image(vars->mlx, vars->img2.img);
    if (vars->img.img)
        mlx_destroy_image(vars->mlx, vars->img.img);
    if (vars->mlx_win)
        mlx_destroy_window(vars->mlx, vars->mlx_win);
    if (vars->mlx)
        mlx_destroy_display(vars->mlx);
    free(vars->mlx);
}

int cross_button_handler(void *param)
{
    t_vars *vars = param;
    cleanup(vars);
    exit(0);
}

int initialize_mlx(t_vars *vars)
{
    vars->mlx = NULL;
    vars->mlx_win = NULL;
    vars->img.img = NULL;
    vars->img2.img = NULL;

    vars->mlx = mlx_init();
    if (vars->mlx == NULL)
    {
        perror("Unable to initialize mlx");
        return -1;
    }
    vars->mlx_win = mlx_new_window(vars->mlx, WINDOW_W, WINDOW_H, "Press ESC to exit");
    if (vars->mlx_win == NULL)
    {
        perror("Unable to initialize mlx_win");
        return -1;
    }
    vars->img.img = mlx_new_image(vars->mlx, WINDOW_W, WINDOW_H);
    if (vars->img.img == NULL)
    {
        perror("Unable to initialize image");
        return -1;
    }
    vars->img.w = WINDOW_W;
    vars->img.h = WINDOW_H;
    vars->img.addr = mlx_get_data_addr(vars->img.img, &vars->img.bits_per_pixel, &vars->img.line_length, &vars->img.endian);

    if (init_mini_window(vars) != 0)
    {
        perror("Unable to initialize mini window");
        return -1;
    }
    return 0;
}

static t_wall wall[] = {
    {{{100, 100}, {900, 100}}, 0x00ff0000},
    {{{900, 100}, {900, 900}}, 0x0080ff00},
    {{{900, 900}, {100, 900}}, 0x008060ff},
    {{{100, 900}, {100, 100}}, 0x00ff8000},
};

int main(void)
{
    t_vars vars;

    if (initialize_mlx(&vars) != 0)
    {
        cleanup(&vars);
        exit(1);
    }

    vars.map = init_map(10, 10);
    init_camera(&vars.camera, 0, 0, 10 * 100, 10 * 100);
    init_player(&vars.player, 5 * 100, 5 * 100);
    init_wall(wall, sizeof(wall) / sizeof(t_wall));
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