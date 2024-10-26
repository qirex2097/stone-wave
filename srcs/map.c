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

    int k = 0;
    while (k < 5)
    {
        i = 1 + rand() % (map->h - 2);
        j = 1 + rand() % (map->w - 2);
        map->data[i][j] = '1';
        k++;
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
                rect.x = j * 100;
                rect.y = i * 100;
                rect.w = rect.h = 100;
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
void ray_grid_intersection(t_map *map, t_pos ray_origin, t_vec ray_direction)
{
    int grid_size = map->grid_size;

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

    printf("(%d,%d),dir=(%d,%d)\n", current_pos_x, current_pos_y,
           step_x * ray_direction_x, step_y * ray_direction_y);

    while (1)
    {
        // 次にXまたはY方向でセルの境界に到達するまでの距離
        int t_max_x = (current_pos_x % grid_size == 0) ? grid_size : (step_x > 0) ? ((current_pos_x / grid_size + 1) * grid_size - current_pos_x)
                                                                                  : (current_pos_x - ((current_pos_x - 1) / grid_size) * grid_size);
        int t_max_y = (current_pos_y % grid_size == 0) ? grid_size : (step_y > 0) ? ((current_pos_y / grid_size + 1) * grid_size - current_pos_y)
                                                                                  : (current_pos_y - ((current_pos_y - 1) / grid_size) * grid_size);

        int dx, dy;
        if (t_max_x * ray_direction_y < t_max_y * ray_direction_x)
        {
            dx = step_x * t_max_x;
            dy = step_y * t_max_x * ray_direction_y / ray_direction_x;
        }
        else
        {
            dx = step_x * t_max_y * ray_direction_x / ray_direction_y;
            dy = step_y * t_max_y;
        }

        current_pos_x += dx;
        current_pos_y += dy;

        printf("(%d,%d)\n", current_pos_x, current_pos_y);

        if ((current_pos_x <= map->x || map->x + map->w * map->grid_size <= current_pos_x) ||
            (current_pos_y <= map->y || map->y + map->h * map->grid_size <= current_pos_y))
            break;
    }
    printf("----------------------------------------\n");
}
