#include <stdlib.h>
#include <stdio.h>
#include "stone.h"

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
    map->w = w;
    map->h = h;

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