#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stone.h"

#define SCALE_STEP 8
#define PLAYER_RADIUS 20
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
    int dx, dy, dangle;

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
    if (mouse->button & SCALE_UP)
    {
        img->field_h -= SCALE_STEP;
        img->field_w -= SCALE_STEP;
        img->field_x += SCALE_STEP / 2;
        img->field_y += SCALE_STEP / 2;
        mouse->button &= ~(SCALE_UP);
    }
    if (mouse->button & SCALE_DOWN)
    {
        img->field_h += SCALE_STEP;
        img->field_w += SCALE_STEP;
        img->field_x -= SCALE_STEP / 2;
        img->field_y -= SCALE_STEP / 2;
        mouse->button &= ~(SCALE_DOWN);
    }
    if (mouse->button & CENTER)
    {
        t_pos pos;
        convert_to_field(&mouse->pos, &pos, img->field_x, img->field_y, img->field_w, img->field_h);
        img->field_x = pos.x - img->field_w / 2;
        img->field_y = pos.y - img->field_h / 2;
        // printf("(%d,%d)\n", img->field_x, img->field_y);
        mouse->button &= ~(CENTER);
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
    draw_circle(&vars->img, &way.p0, radius, PLAYER_COLOR);

    int line_length = LINE_LENGTH;
    int i = 0;
    while (i < 7)
    {
        t_pos cross_point;
        int color = 0x0000ff00;
        double radian = (player->angle + (i - 3) * 10) * PI / 180.0;
        way.p1.x = player->x + (int)(line_length * cos(radian));
        way.p1.y = player->y + (int)(line_length * sin(radian));
        draw_line(&vars->img, &way, color);

        int j = 0;
        t_line *wall;
        while (wall = get_wall(j))
        {
            if (do_intersect(wall, &way) && get_intersection(wall, &way, &cross_point))
            {
                color = 0x00ff0000;
                draw_circle(&vars->img, &cross_point, 10, color);
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