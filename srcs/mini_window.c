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
    memset(vars->img2.addr, 0, MINIWINDOW_W * MINIWINDOW_H * (vars->img2.bits_per_pixel / 8));
    return 0;
}

int render_mini_window(t_vars *vars)
{
    mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->img2.img, vars->mini_x, vars->mini_y);
    memset(vars->img2.addr, 0, MINIWINDOW_W * MINIWINDOW_H * (vars->img2.bits_per_pixel / 8));

    my_mlx_draw_line(&vars->img2, 0, 0, MINIWINDOW_W - 1, 0, 0x00ff00ff);
    my_mlx_draw_line(&vars->img2, MINIWINDOW_W - 1, 0, MINIWINDOW_W - 1, MINIWINDOW_H - 1, 0x00ff00ff);
    my_mlx_draw_line(&vars->img2, MINIWINDOW_W - 1, MINIWINDOW_H - 1, 0, MINIWINDOW_H - 1, 0x00ff00ff);
    my_mlx_draw_line(&vars->img2, 0, MINIWINDOW_H - 1, 0, 0, 0x00ff00ff);

    return 0;
}

void map_point_on_line(t_line *line, int w, int a, t_pos *point)
{
    if (line == NULL || point == NULL || w == 0)
        return;
    point->x = line->x0 + (a * (line->x1 - line->x0)) / w;
    point->y = line->y0 + (a * (line->y1 - line->y0)) / w;
}

void draw_miniwindow(t_vars *vars, t_wall *wall, t_line *way, int sx)
{
    t_player *player = &vars->player;
    t_pos cross_point;
    t_line way3;

    double radian = (player->angle * PI) / 180.0;
    int player_ray_x = player->x + (int)(VIEW_LENGTH * cos(radian));
    int player_ray_y = player->y + (int)(VIEW_LENGTH * sin(radian));

    scale_segment(way, 5, &way3);

    if (get_intersection(&wall->line, &way3, &cross_point))
    {
        int color = wall->color;
        draw_circle(&vars->img, &vars->camera, &cross_point, 3, color);
        // ミニウィンドウにラインを描画
        double cos_theta = cosine_angle(player_ray_x, player_ray_y, player->x, player->y, cross_point.x, cross_point.y);
        double distance = sqrt(distance_squared(way->x0, way->y0, cross_point.x, cross_point.y)) * cos_theta;
        if (distance <= 0)
            return;
        int line_length = (int)(WALL_HEIGHT / distance);
        my_mlx_draw_line(&vars->img2, sx, MINIWINDOW_H / 2 - line_length / 2, sx, MINIWINDOW_H / 2 + line_length / 2, color);
    }
}

t_pos ray_wall_intersection(t_vars *vars, t_pos origin, t_vec direction);

void draw_player_view(t_vars *vars, t_line *screen)
{
    t_player *player = &vars->player;

    static int sx = 0;
    sx = 0;
    while (sx < vars->img2.w)
    {
        t_line ray;
        t_pos p1;
        int width = vars->img2.w;
        ray.x0 = player->x;
        ray.y0 = player->y;
        map_point_on_line(screen, width, sx, &p1);
        ray.x1 = p1.x;
        ray.y1 = p1.y;
        // draw_line(&vars->img, &vars->camera, &ray, 0x00ffffff);

        t_pos origin, cross_point;
        t_vec direction;
        int color = 0x00ffffff;

        double radian = (player->angle * PI) / 180.0;
        origin.x = player->x;
        origin.y = player->y;
        direction.x = ray.x1 - ray.x0;
        direction.y = ray.y1 - ray.y0;
        cross_point = ray_wall_intersection(vars, origin, direction);
        // draw_circle(&vars->img, &vars->camera, &cross_point, 3, 0x00ffffff);

        if (cross_point.x % vars->map->grid_size == 0)
        {
            if (cross_point.y % vars->map->grid_size == 0)
            {
                char str[100];
                snprintf(str, sizeof(str), "(%3d,%3d)", cross_point.x, cross_point.y);
                my_string_put(&vars->buff, str);
            }
            color = 0x00800000;
        }

        // ミニウィンドウにラインを描画
        double cos_theta = cosine_angle(ray.x1, ray.y1, ray.x0, ray.y0, cross_point.x, cross_point.y);
        double distance = sqrt(distance_squared(ray.x0, ray.y0, cross_point.x, cross_point.y)) * cos_theta;
        if (distance <= 0)
            return;
        int line_length = (int)(WALL_HEIGHT / distance);
        my_mlx_draw_line(&vars->img2, sx, MINIWINDOW_H / 2 - line_length / 2, sx, MINIWINDOW_H / 2 + line_length / 2, color);

        sx++;
    }
}

t_pos ray_wall_intersection(t_vars *vars, t_pos origin, t_vec direction)
{
    t_pos cross_point;
    int map_x1, map_y1, map_x2, map_y2;
    int i = 1;
    while (ray_grid_intersection(vars->map, origin, direction, &cross_point))
    {
        int color;

        map_x1 = cross_point.x / vars->map->grid_size;
        map_y1 = cross_point.y / vars->map->grid_size;
        if (cross_point.x % vars->map->grid_size == 0 && cross_point.y % vars->map->grid_size == 0)
        {
            map_x2 = map_x1 - 1;
            map_y2 = map_y1 - 1;
        }
        else if (cross_point.x % vars->map->grid_size == 0)
        {
            map_x2 = map_x1 - 1;
            map_y2 = map_y1;
        }
        else
        {
            map_x2 = map_x1;
            map_y2 = map_y1 - 1;
        }
        // draw_circle(&vars->img, &vars->camera, &cross_point, 3, color);
        origin.x = cross_point.x;
        origin.y = cross_point.y;

        char str[100];
        snprintf(str, sizeof(str), "%2d:(%4d,%4d),map=(%d,%d)=%c,(%d,%d)=%c",
                 i, cross_point.x, cross_point.y,
                 map_x2, map_y2, vars->map->data[map_y2][map_x2],
                 map_x1, map_y1, vars->map->data[map_y1][map_x1]);
        // my_string_put(&vars->buff, str);

        if (vars->map->data[map_y2][map_x2] == '1' || vars->map->data[map_y1][map_x1] == '1')
            break;
        i++;
    }
    return cross_point;
}
