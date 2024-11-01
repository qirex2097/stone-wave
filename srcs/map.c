#include <stdlib.h>
#include <stdio.h>
#include "stone.h"

#define GRID_SIZE 100

t_map *init_map(int w, int h)
{
    int i, j;
    t_map *map;
    map = (t_map *)malloc(sizeof(t_map));
    if (map == NULL)
    {
        perror("Unable to allocate memory for map");
        return NULL;
    }
    map->data = malloc((h + 1) * sizeof(char *));
    if (map->data == NULL)
    {
        perror("Unable to allocate memory for map");
        free_map(map);
        return NULL;
    }
    i = 0;
    while (i < h)
    {
        map->data[i] = malloc((w + 1) * sizeof(char));
        if (map->data[i] == NULL)
        {
            perror("Unable to allocate memory for map");
            free_map(map);
            return NULL;
        }
        map->data[i][w] = '\0';
        i++;
    }
    map->data[h] = NULL;
    map->x = 0;
    map->y = 0;
    map->w = w;
    map->h = h;
    map->grid_size = GRID_SIZE;

    // サンプルマップ
    i = 0;
    while (i < map->h)
    {
        j = 0;
        while (j < map->w)
        {
            if (i == 0 || i == map->h - 1 || j == 0 || j == map->w - 1)
                map->data[i][j] = '1';
            else
                map->data[i][j] = '0';
            j++;
        }
        i++;
    }

    map->data[1][1] = '1';
    if (map->w > 9 && map->h > 9)
    {
        map->data[1][8] = '1';
        map->data[8][1] = '1';
        map->data[8][8] = '1';
    }

    return map;
}

void free_map(t_map *map)
{
    if (map == NULL)
        return;
    int i = 0;
    if (map->data)
    {
        while (map->data[i])
        {
            free(map->data[i]);
            i++;
        }
        free(map->data);
    }
    free(map);
    return;
}

int draw_map(t_vars *vars)
{
    if (vars == NULL || vars->map == NULL)
        return 0;
    t_map *map = vars->map;
    int i, j;
    i = 0;
    while (i < map->h)
    {
        j = 0;
        while (j < map->w)
        {
            if (map->data[i][j] == '1')
            {
                t_rect rect;
                rect.x = j * map->grid_size;
                rect.y = i * map->grid_size;
                rect.w = rect.h = map->grid_size;
                draw_rect(&vars->img, &vars->camera, &rect, 0x00ff4080);
            }
            j++;
        }
        i++;
    }
    return 0;
}

// 最大公約数を求める関数（整数比を簡約化するため）
int gcd(int a, int b)
{
    while (b != 0)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// 光線と格子の交点を求めるDDAアルゴリズム
t_pos find_next_grid_crossing(t_pos ray_origin, t_vec ray_direction, int grid_size)
{
    // 方向ベクトルの簡約化
    int gcd_val = gcd(abs(ray_direction.x), abs(ray_direction.y));
    int ray_direction_x = abs(ray_direction.x / gcd_val);
    int ray_direction_y = abs(ray_direction.y / gcd_val);

    // 現在位置
    int current_pos_x, current_pos_y;
    current_pos_x = ray_origin.x;
    current_pos_y = ray_origin.y;

    // どの方向に進むかを決定
    int step_x = (ray_direction.x > 0) ? 1 : -1;
    int step_y = (ray_direction.y > 0) ? 1 : -1;

    // 次にXまたはY方向でセルの境界に到達するまでの距離
    int t_max_x = (current_pos_x % grid_size == 0) ? grid_size : (step_x > 0) ? ((current_pos_x / grid_size + 1) * grid_size - current_pos_x)
                                                                              : (current_pos_x - ((current_pos_x - 1) / grid_size) * grid_size);
    int t_max_y = (current_pos_y % grid_size == 0) ? grid_size : (step_y > 0) ? ((current_pos_y / grid_size + 1) * grid_size - current_pos_y)
                                                                              : (current_pos_y - ((current_pos_y - 1) / grid_size) * grid_size);

    int dx, dy;
    if (t_max_x * ray_direction_y < t_max_y * ray_direction_x)
    {
        if (ray_direction_x == 0)
        {
            dx = 0;
            dy = step_y * grid_size;
        }
        else
        {

            dx = step_x * t_max_x;
            dy = step_y * t_max_x * ray_direction_y / ray_direction_x;
        }
    }
    else
    {
        if (ray_direction_y == 0)
        {
            dx = step_x * grid_size;
            dy = 0;
        }
        else
        {

            dx = step_x * t_max_y * ray_direction_x / ray_direction_y;
            dy = step_y * t_max_y;
        }
    }

    current_pos_x += dx;
    current_pos_y += dy;

    return (t_pos){current_pos_x, current_pos_y};
}

int is_not_space(t_map *map, int map_x, int map_y)
{
    return map_x < 0 || map_y < 0 || map_x >= map->w || map_y >= map->h || map->data[map_y][map_x] != '0';
}

t_pair get_grid_at_position(t_map *map, t_pos cross_point, t_vec direction)
{
    int map_x1, map_y1, map_x2, map_y2;
    int grid_size = map->grid_size;
    map_x1 = cross_point.x / grid_size;
    map_y1 = cross_point.y / grid_size;
    if (cross_point.x % grid_size == 0 && cross_point.y % grid_size == 0)
    {
        if (direction.x * direction.y > 0)
        {
            map_x2 = map_x1 - 1;
            map_y2 = map_y1 - 1;
        }
        else
        {
            map_x1 -= 1;
            map_x2 = map_x1 + 1;
            map_y2 = map_y1 - 1;
        }
    }
    else if (cross_point.x % grid_size == 0)
    {
        map_x2 = map_x1 - 1;
        map_y2 = map_y1;
        if (is_not_space(map, map_x2, map_y2))
            return (t_pair){map_x1, map_y1, map_x2, map_y2};
        else
            return (t_pair){map_x2, map_y2, map_x1, map_y1};
    }
    else
    {
        map_x2 = map_x1;
        map_y2 = map_y1 - 1;
        if (is_not_space(map, map_x2, map_y2))
            return (t_pair){map_x1, map_y1, map_x2, map_y2};
        else
            return (t_pair){map_x2, map_y2, map_x1, map_y1};
    }

    return (t_pair){map_x1, map_y1, map_x2, map_y2};
}

int is_ray_hit_wall(t_map *map, t_pos cross_point)
{
    const int grid_x = cross_point.x % map->grid_size;
    const int grid_y = cross_point.y % map->grid_size;
    if (grid_x != 0 && grid_y != 0)
        return 0;
    if ((cross_point.x <= map->x || map->x + map->w * map->grid_size <= cross_point.x) ||
        (cross_point.y <= map->y || map->y + map->h * map->grid_size <= cross_point.y))
        return 1;

    int map_x, map_y;
    map_x = (cross_point.x - map->x) / map->grid_size;
    map_y = (cross_point.y - map->y) / map->grid_size;
    if (grid_x == 0 && grid_y == 0)
    {
        if (is_not_space(map, map_x, map_y) || is_not_space(map, map_x - 1, map_y) ||
            is_not_space(map, map_x, map_y - 1) || is_not_space(map, map_x - 1, map_y - 1))
            return 1;
    }
    else if (grid_x == 0)
    {
        if (is_not_space(map, map_x, map_y) || is_not_space(map, map_x - 1, map_y))
            return 1;
    }
    else if (grid_y == 0)
    {
        if (is_not_space(map, map_x, map_y) || is_not_space(map, map_x, map_y - 1))
            return 1;
    }

    return 0;
}

t_pos detect_ray_wall_intersection(t_map *map, t_pos origin, t_vec direction)
{
    t_pos cross_point = {0, 0};
    while (1)
    {
        cross_point = find_next_grid_crossing(origin, direction, map->grid_size);
        origin.x = cross_point.x;
        origin.y = cross_point.y;

        if (is_ray_hit_wall(map, cross_point))
            break;
    }
    return cross_point;
}

int is_corner_wall(t_map *map, int map_x, int map_y)
{
    return (is_not_space(map, map_x, map_y) && is_not_space(map, map_x, map_y - 1)) ||
           (is_not_space(map, map_x - 1, map_y) && is_not_space(map, map_x - 1, map_y - 1));
}

int get_wall_color(t_map *map, t_pos cross_point, t_vec direction)
{
    const int VERTICAL_WALL_COLOR = 0x00ff0000;
    const int HORIZONTAL_WALL_COLOR = 0x0000ff00;
    int map_x, map_y;
    map_x = (cross_point.x - map->x) / map->grid_size;
    map_y = (cross_point.y - map->y) / map->grid_size;

    if (cross_point.x % map->grid_size == 0 && cross_point.y % map->grid_size == 0)
        return is_corner_wall(map, map_x, map_y) ? VERTICAL_WALL_COLOR : HORIZONTAL_WALL_COLOR;

    return (cross_point.x % map->grid_size == 0) ? VERTICAL_WALL_COLOR : HORIZONTAL_WALL_COLOR;
}
