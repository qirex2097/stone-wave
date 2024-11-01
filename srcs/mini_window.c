#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <mlx.h>
#include <stdio.h>
#include "stone.h"

#define MINIWINDOW_W 320
#define MINIWINDOW_H 240
#define VIEW_LENGTH 300
#define WALL_HEIGHT 10000

#define SEGMENT_SCALE_FACTOR 5

int init_mini_window(t_vars *vars)
{
    if (vars == NULL || vars->mlx == NULL)
        return -1;

    vars->img2.img = mlx_new_image(vars->mlx, MINIWINDOW_W, MINIWINDOW_H);
    if (vars->img2.img == NULL)
        return -1;
    vars->img2.w = MINIWINDOW_W;
    vars->img2.h = MINIWINDOW_H;
    vars->img2.addr = mlx_get_data_addr(vars->img2.img, &vars->img2.bits_per_pixel, &vars->img2.line_length, &vars->img2.endian);
    if (vars->img2.addr == NULL)
    {
        mlx_destroy_image(vars->mlx, vars->img2.img);
        return -1;
    }
    memset(vars->img2.addr, 0, MINIWINDOW_W * MINIWINDOW_H * (vars->img2.bits_per_pixel / 8));
    return 0;
}

int render_mini_window(t_vars *vars)
{
    if (vars == NULL || vars->mlx == NULL || vars->mlx_win == NULL)
        return -1;

    mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->img2.img, vars->mini_x, vars->mini_y);
    memset(vars->img2.addr, 0, MINIWINDOW_W * MINIWINDOW_H * (vars->img2.bits_per_pixel / 8));

    my_mlx_draw_line(&vars->img2, 0, 0, MINIWINDOW_W - 1, 0, 0x00ff00ff);
    my_mlx_draw_line(&vars->img2, MINIWINDOW_W - 1, 0, MINIWINDOW_W - 1, MINIWINDOW_H - 1, 0x00ff00ff);
    my_mlx_draw_line(&vars->img2, MINIWINDOW_W - 1, MINIWINDOW_H - 1, 0, MINIWINDOW_H - 1, 0x00ff00ff);
    my_mlx_draw_line(&vars->img2, 0, MINIWINDOW_H - 1, 0, 0, 0x00ff00ff);

    return 0;
}

t_pos map_point_on_line(t_line line, int w, int a)
{
    if (w == 0)
        return (t_pos){0, 0};

    t_pos point;
    point.x = line.x0 + (a * (line.x1 - line.x0)) / w;
    point.y = line.y0 + (a * (line.y1 - line.y0)) / w;
    return point;
}

int calculate_line_length(t_pos p, int angle, t_pos cross_point)
{
    double dx = cos(angle * PI / 180.0);
    double dy = sin(angle * PI / 180.0);
    double A = -dy;
    double B = dx;
    double C = -(A * p.x + B * p.y);
    double distance;

    distance = fabs(A * cross_point.x + B * cross_point.y + C) / sqrt(A * A + B * B);

    if (distance <= 0.0001)
        return -1;
    return (int)(WALL_HEIGHT / distance);
}

void draw_player_view(t_vars *vars, t_line screen_line)
{
    t_player *player = &vars->player;

    int sx = 0;
    while (sx < vars->img2.w)
    {
        t_line view_ray;
        t_pos p1;
        int width = vars->img2.w;
        p1 = map_point_on_line(screen_line, width, sx);
        view_ray.x0 = player->x;
        view_ray.y0 = player->y;
        view_ray.x1 = p1.x;
        view_ray.y1 = p1.y;
        // draw_line(&vars->img, &vars->camera, &view_ray, 0x00ffffff);

        t_pos origin, cross_point;
        t_vec direction;

        double radian = (player->angle * PI) / 180.0;
        origin.x = player->x;
        origin.y = player->y;
        direction.x = view_ray.x1 - view_ray.x0;
        direction.y = view_ray.y1 - view_ray.y0;
        cross_point = detect_ray_wall_intersection(vars->map, origin, direction);
        draw_circle(&vars->img, &vars->camera, &cross_point, 3, 0x00ffffff);

        int color;
        color = get_wall_color(vars->map, cross_point, direction);

        // draw a line at position sx
        int line_length = calculate_line_length(origin, (player->angle + 90) % 360, cross_point);
        if (line_length > 0)
            my_mlx_draw_line(&vars->img2,
                             sx, MINIWINDOW_H / 2 - line_length / 2,
                             sx, MINIWINDOW_H / 2 + line_length / 2, color);
        sx++;
    }
}
