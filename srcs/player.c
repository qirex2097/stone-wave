#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stone.h"

#define PLAYER_RADIUS 10
#define PLAYER_COLOR 0x00ff0000
#define TEXT_COLOR 0x00ffffff

int init_player(t_player *player, int x, int y)
{
    if (player == NULL)
        return -1;
    player->x = x;
    player->y = y;
    player->angle = 0;

    return 0;
}

int init_mouse(t_mouse *mouse)
{
    if (mouse == NULL)
        return -1;
    mouse->pos.x = 0;
    mouse->pos.y = 0;
    mouse->button = 0;

    return 0;
}

int update_player(t_vars *vars)
{
    if (vars == NULL)
        return -1;
    t_player *player = &vars->player;
    t_mouse *mouse = &vars->mouse;
    t_img *img = &vars->img;
    t_camera *camera = &vars->camera;
    int dx, dy, dangle;
    double radian = (player->angle) * PI / 180.0;

    dx = dy = 2;
    dangle = 1;
    if (mouse->button & TURN_LEFT)
        player->angle -= dangle;
    if (mouse->button & TURN_RIGHT)
        player->angle += dangle;
    player->angle = (player->angle + 360) % 360;
    if (mouse->button & MOVE_UP)
        player->y -= dy;
    if (mouse->button & MOVE_DOWN)
        player->y += dy;
    if (mouse->button & MOVE_LEFT)
        player->x -= dx;
    if (mouse->button & MOVE_RIGHT)
        player->x += dx;
    if (mouse->button & MOVE_FORWARD)
    {
        player->x += (int)(dx * cos(radian));
        player->y += (int)(dx * sin(radian));
    }
    if (mouse->button & MOVE_BACKWARD)
    {
        player->x -= (int)(dx * cos(radian));
        player->y -= (int)(dx * sin(radian));
    }

    return 0;
}

void draw_player_lines(t_vars *vars)
{
    t_player *player = &vars->player;
    t_line way;

    way.p0.x = player->x;
    way.p0.y = player->y;
    int radius = PLAYER_RADIUS;
    draw_circle(&vars->img, &vars->camera, &way.p0, radius, PLAYER_COLOR);
    draw_player_view_line(vars);
}

void render_player_info(t_vars *vars)
{
    t_player *player = &vars->player;

    char str[100];
    snprintf(str, sizeof(str), "(%3d,%3d) %4d", player->x, player->y, player->angle);
    mlx_string_put(vars->mlx, vars->mlx_win, 10, 10, TEXT_COLOR, str);
}

int draw_player(t_vars *vars)
{
    draw_player_lines(vars);
    render_player_info(vars);

    return 0;
}
