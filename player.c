#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stone.h"

#define PLAYER_RADIUS 10
#define VIEW_LENGTH 300
#define PLAYER_COLOR 0x00ff0000
#define TEXT_COLOR 0x00ffffff

double cosine_angle(int x1, int y1, int x2, int y2, int x3, int y3);

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

void map_point_on_line(t_line *line, int w, int a, t_pos *point)
{
    if (line == NULL || point == NULL || w == 0)
        return;
    point->x = line->p0.x + (a * (line->p1.x - line->p0.x)) / w;
    point->y = line->p0.y + (a * (line->p1.y - line->p0.y)) / w;
}

int find_intersection_point(t_vars *vars, t_line *line, t_pos *cross_point, int *color)
{
    t_wall *wall;
    int j = 0;
    int flg = 0;
    while (wall = get_wall(j))
    {
        if (do_intersect(&wall->line, line) && get_intersection(&wall->line, line, cross_point))
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

    double radian = (player->angle * PI) / 180.0;
    int player_ray_x = player->x + (int)(VIEW_LENGTH * cos(radian));
    int player_ray_y = player->y + (int)(VIEW_LENGTH * sin(radian));

    int line_length = VIEW_LENGTH;
    int i = 0;
    int kazu = 29;
    int kakudo = 3;
    while (i < kazu)
    {
        int color = 0x0000ff00;
        double radian = (player->angle + (i - kazu / 2) * kakudo) * PI / 180.0;
        way.p0.x = player->x;
        way.p0.y = player->y;
        way.p1.x = player->x + (int)(line_length * cos(radian));
        way.p1.y = player->y + (int)(line_length * sin(radian));
        draw_line(&vars->img, &vars->camera, &way, color);

        int j = 0;
        t_wall *wall;
        while (wall = get_wall(j))
        {
            t_pos cross_point;

            if (do_intersect(&wall->line, &way) && get_intersection(&wall->line, &way, &cross_point))
            {
                color = wall->color;
                draw_circle(&vars->img, &vars->camera, &cross_point, 3, color);
                // ミニウィンドウにラインを描画
                double cos_theta = cosine_angle(player->x, player->y, player_ray_x, player_ray_y, cross_point.x, cross_point.y);
                double distance = sqrt(distance_squared(way.p0.x, way.p0.y, cross_point.x, cross_point.y)) * cos_theta;
                int line_length = (int)(2800 / distance);
                int x = vars->img2.w / 2 + (i - kazu / 2) * 10;
                my_mlx_draw_line(&vars->img2, x, 60 - line_length / 2, x, 60 + line_length / 2, color);
            }
            j++;
        }
        i++;
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

// ベクトルの内積を計算
long long dot_product(int x1, int y1, int x2, int y2)
{
    return (long long)x1 * x2 + (long long)y1 * y2;
}

// ベクトルの長さの平方を計算
long long vector_length_squared(int x, int y)
{
    return (long long)x * x + (long long)y * y;
}

// 三点 (x1, y1), (x2, y2), (x3, y3) を結ぶ直線の角度のコサインを計算
double cosine_angle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    // ベクトル v1 = (x2 - x1, y2 - y1), ベクトル v2 = (x3 - x2, y3 - y2)
    int v1x = x1 - x2, v1y = y1 - y2;
    int v2x = x3 - x2, v2y = y3 - y2;

    // 内積 v1・v2
    long long dot = dot_product(v1x, v1y, v2x, v2y);

    // v1 の長さの平方と v2 の長さの平方
    long long len_v1_sq = vector_length_squared(v1x, v1y);
    long long len_v2_sq = vector_length_squared(v2x, v2y);

    // ベクトルの大きさ (長さ) を平方根で求める
    double len_v1 = sqrt((double)len_v1_sq);
    double len_v2 = sqrt((double)len_v2_sq);

    // cosθ = dot / (|v1| * |v2|)
    double cos_theta = (double)dot / (len_v1 * len_v2);

    return cos_theta;
}
