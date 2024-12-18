#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stone.h"

#define PLAYER_RADIUS 10
#define PLAYER_COLOR 0x00ff0000
#define TEXT_COLOR 0x00ffffff
#define DEFAULT_SCREEN_DISTANCE 100

int init_player(t_player *player, int x, int y)
{
    if (player == NULL)
        return -1;
    player->x = x;
    player->y = y;
    player->angle = 232;

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

t_line calculate_screen_line(t_vars *vars, int distance)
{
    t_player *player = &vars->player;
    t_line screen_line;

    double player_angle = player->angle * PI / 180;
    int screen_size = 100;
    screen_line.x0 = player->x + distance * cos(player_angle) + screen_size * cos(player_angle - PI / 2.0);
    screen_line.y0 = player->y + distance * sin(player_angle) + screen_size * sin(player_angle - PI / 2.0);
    screen_line.x1 = player->x + distance * cos(player_angle) + screen_size * cos(player_angle + PI / 2.0);
    screen_line.y1 = player->y + distance * sin(player_angle) + screen_size * sin(player_angle + PI / 2.0);

    return screen_line;
}

void draw_player_lines(t_vars *vars)
{
    t_player *player = &vars->player;
    t_line way;
    t_pos center;

    center.x = player->x;
    center.y = player->y;
    int radius = PLAYER_RADIUS;
    draw_circle(&vars->img, &vars->camera, &center, radius, PLAYER_COLOR);
}

void render_player_info(t_vars *vars)
{
    t_player *player = &vars->player;

    my_string_put("HELLO");

    char str[100];
    snprintf(str, sizeof(str), "(%3d,%3d) %4d", player->x, player->y, player->angle);
    my_string_put(str);
}

int draw_player(t_vars *vars)
{
    t_line screen_line;
    draw_player_lines(vars);
    screen_line = calculate_screen_line(vars, DEFAULT_SCREEN_DISTANCE);
    draw_line(&vars->img, &vars->camera, &screen_line, 0x00ffffff); // screen_line
    draw_player_view(vars, screen_line);                            // mini_window.c
    render_player_info(vars);
    return 0;
}
