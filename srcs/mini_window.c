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

void draw_player_view(t_vars *vars, t_line *screen)
{
    t_player *player = &vars->player;

    static int sx = 0;
    // while (sx < vars->img2.w)
    {
        t_line ray;
        t_pos p1;
        int width = vars->img2.w;
        ray.x0 = player->x;
        ray.y0 = player->y;
        map_point_on_line(screen, width, sx, &p1);
        ray.x1 = p1.x;
        ray.y1 = p1.y;
        draw_line(&vars->img, &vars->camera, &ray, 0x00ffffff);

        int j = 0;
        t_wall *wall;
        wall = get_wall(j);
        while (wall)
        {
            draw_miniwindow(vars, wall, &ray, sx);
            wall = get_wall(++j);
        }

        sx++;

        if (vars->img2.w <= sx)
            sx = 0;
    }
}
