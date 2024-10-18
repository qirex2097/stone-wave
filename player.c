#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stone.h"

#define PLAYER_RADIUS 10
#define LINE_LENGTH 100
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

    dx = dy = 5;
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

int draw_player_lines(t_vars *vars)
{
    t_player *player = &vars->player;
    t_line way;

    way.p0.x = player->x;
    way.p0.y = player->y;
    int radius = PLAYER_RADIUS;
    draw_circle(&vars->img, &vars->camera, &way.p0, radius, PLAYER_COLOR);

    int line_length = LINE_LENGTH;
    int i = 0;
    int kazu = 15;
    int kakudo = 7;
    while (i < kazu)
    {
        t_pos cross_point;
        int color = 0x0000ff00;
        double radian = (player->angle + (i - kazu / 2) * kakudo) * PI / 180.0;
        way.p1.x = player->x + (int)(line_length * cos(radian));
        way.p1.y = player->y + (int)(line_length * sin(radian));
        draw_line(&vars->img, &vars->camera, &way, color);

        int j = 0;
        t_line *wall;
        while (wall = get_wall(j))
        {
            if (do_intersect(wall, &way) && get_intersection(wall, &way, &cross_point))
            {
                color = 0x00ff0000;
                draw_circle(&vars->img, &vars->camera, &cross_point, 3, color);
                // ミニウィンドウにラインを描画
                // TODO 距離に応じてラインの長さを変える
                double theta = (i - kazu / 2) * kakudo * PI / 180.0;
                double distance = sqrt(distance_squared(way.p0.x, way.p0.y, cross_point.x, cross_point.y)) * cos(theta);
                int line_length = (int)2800 / distance;
                my_mlx_draw_line(&vars->img2,
                                 vars->img2.w / 2 + (i - kazu / 2) * 10, 60 - line_length / 2,
                                 vars->img2.w / 2 + (i - kazu / 2) * 10, 60 + line_length / 2, 0x00ff0000);
            }
            j++;
        }
        i++;
    }
}

int render_player_info(t_vars *vars)
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