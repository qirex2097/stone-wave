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

    i = 0;
    while (i < h)
    {
        j = 0;
        while (j < w)
        {
            if (i == 0 || i == h - 1 || j == 0 || j == w - 1)
                map->data[i][j] = '1';
            else
                map->data[i][j] = '0';
            j++;
        }
        i++;
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
