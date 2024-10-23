#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stone.h"

#define PLAYER_RADIUS 10
#define VIEW_LENGTH 300
#define PLAYER_COLOR 0x00ff0000
#define TEXT_COLOR 0x00ffffff

int init_player(t_player *player)
{
    if (player == NULL)
        return -1;
    player->x = 0;
    player->y = 0;
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

int find_intersection_point(t_vars *vars, t_line *line, t_pos *cross_point, int *color)
{
    t_wall *wall;
    int j = 0;
    int flg = 0;
    while (wall = get_wall(j))
    {
        if (get_intersection(&wall->line, line, cross_point))
        {
            *color = wall->color;
            flg = 1;
        }
        j++;
    }
    return flg;
}

void draw_player_view_line(t_vars *vars)
{
    t_player *player = &vars->player;
    t_line way, player_ray;

    t_line screen_line;

    double radian = (player->angle * PI) / 180.0;
    screen_line.p0.x = player->x + VIEW_LENGTH * cos(radian - PI / 4.0);
    screen_line.p0.y = player->y + VIEW_LENGTH * sin(radian - PI / 4.0);
    screen_line.p1.x = player->x + VIEW_LENGTH * cos(radian + PI / 4.0);
    screen_line.p1.y = player->y + VIEW_LENGTH * sin(radian + PI / 4.0);
    draw_line(&vars->img, &vars->camera, &screen_line, 0x00ffffff);

    int sx = 0;
    while (sx < vars->img2.w)
    {
        t_line ray;
        ray.p0.x = player->x;
        ray.p0.y = player->y;
        map_point_on_line(&screen_line, vars->img2.w, sx, &ray.p1);

        int j = 0;
        t_wall *wall;
        wall = get_wall(j);
        while (wall)
        {
            draw_miniwindow(vars, wall, &ray, sx);
            wall = get_wall(++j);
        }

        sx++;
    }
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
