#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stone.h"

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
    t_player *player = &vars->player;
    t_mouse *mouse = &vars->mouse;
    t_img *img = &vars->img;
    int dx, dy, dangle;

    dx = dy = 5;
    dangle = 1;
    if (mouse->button & TURN_LEFT)
        player->angle -= dangle;
    if (mouse->button & TURN_RIGHT)
        player->angle += dangle;
    if (mouse->button & MOVE_UP)
        player->y -= dy;
    if (mouse->button & MOVE_DOWN)
        player->y += dy;
    if (mouse->button & MOVE_LEFT)
        player->x -= dx;
    if (mouse->button & MOVE_RIGHT)
        player->x += dx;
    if (mouse->button & SCALE_UP)
    {
        img->field_h -= 8;
        img->field_w -= 8;
        img->field_x += 4;
        img->field_y += 4;
        mouse->button &= ~(SCALE_UP);
    }
    if (mouse->button & SCALE_DOWN)
    {
        img->field_h += 8;
        img->field_w += 8;
        img->field_x -= 4;
        img->field_y -= 4;
        mouse->button &= ~(SCALE_DOWN);
    }
    if (mouse->button & CENTER)
    {
        t_pos pos;
        convert_to_field(&mouse->pos, &pos, img->field_x, img->field_y, img->field_w, img->field_h);
        img->field_x = pos.x - img->field_w / 2;
        img->field_y = pos.y - img->field_h / 2;
        printf("(%d,%d)\n", img->field_x, img->field_y);
        mouse->button &= ~(CENTER);
    }

    return 0;
}

int draw_player(t_vars *vars)
{
    t_player *player = &vars->player;
    t_line way;

    t_line line0;
    line0.p0.x = -500;
    line0.p0.y = -250;
    line0.p1.x = 0;
    line0.p1.y = 0;
    draw_line(&vars->img, &line0, 0x0000ff80);

    way.p0.x = player->x;
    way.p0.y = player->y;
    int radius = 20;
    draw_circle(&vars->img, &way.p0, radius, 0x00ff0000);

    int line_length = radius * 5;
    int i = 0;
    while (i < 7)
    {
        t_pos cross_point;
        int color = 0x0000ff00;
        double radian = (player->angle + (i - 3) * 10) * PI / 180.0;
        way.p1.x = player->x + (int)(line_length * cos(radian));
        way.p1.y = player->y + (int)(line_length * sin(radian));
        if (do_intersect(&line0, &way) && get_intersection(&line0, &way, &cross_point))
        {
            color = 0x00ff0000;
            draw_circle(&vars->img, &cross_point, 10, color);
        }
        draw_line(&vars->img, &way, color);
        i++;
    }

    char str[100];
    int color = 0x00ffffff;
    snprintf(str, sizeof(str), "(%3d,%3d) %4d -> (%3d,%3d)", player->x, player->y, player->angle, way.p1.x, way.p1.y);
    mlx_string_put(vars->mlx, vars->mlx_win, 10, 10, color, str);

    return 0;
}