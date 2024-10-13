#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include "stone.h"

int init_player(t_player *player)
{
    player->x = 0;
    player->y = 0;
    player->angle = 0;

    return 0;
}

int draw_player(t_vars *vars)
{
    t_player *player = &vars->player;
    int x0, y0;
    int x1, y1;

    x0 = rand() % WINDOW_W;
    y0 = rand() % WINDOW_H;
    x1 = rand() % WINDOW_W;
    y1 = rand() % WINDOW_H;
    draw_line(&vars->img, x0, y0, x1, y1, 0x0000ff00);

    char str[100];
    int color = 0x000000ff;
    sprintf(str, "(%3d,%3d) %4d", player->x, player->y, player->angle);
    mlx_string_put(vars->mlx, vars->mlx_win, 10, 10, color, str);

    return 0;
}