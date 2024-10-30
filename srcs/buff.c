#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mlx.h>
#include "stone.h"

typedef struct
{
    int row_kazu;
    char **rows;
} t_buff;

t_buff buff;

void init_buff()
{
    buff.row_kazu = 0;
    buff.rows = NULL;
}

void cleanup_buff()
{
    if (buff.rows == NULL)
        return;

    int i = 0;
    while (i < buff.row_kazu)
    {
        if (buff.rows[i])
            free(buff.rows[i]);
        buff.rows[i] = NULL;
        i++;
    }
    free(buff.rows);
    buff.row_kazu = 0;
    buff.rows = NULL;
}

void draw_buff(t_vars *vars)
{
    int offset_x = 10;
    int offset_y = 10;
    int color = 0x00ffffff;
    char *str;

    if (buff.rows == NULL)
        return;

    int i = 0;
    while (i < buff.row_kazu)
    {
        str = buff.rows[i];
        mlx_string_put(vars->mlx, vars->mlx_win, offset_x, offset_y + i * 10, color, str);
        i++;
    }
}

void my_string_put(const char *str)
{
    if (str == NULL)
        return;
    if (buff.rows == NULL)
    {
        buff.rows = malloc(sizeof(char *));
        if (buff.rows == NULL)
        {
            perror("Unable to allocate memory for rows");
            exit(1);
        }
        buff.row_kazu = 0;
    }

    char **new_rows = realloc(buff.rows, sizeof(char *) * (buff.row_kazu + 2));
    if (new_rows == NULL)
    {
        perror("Unable to allocate memory for rows");
        exit(1);
    }
    buff.rows = new_rows;

    buff.rows[buff.row_kazu] = strdup(str);
    if (buff.rows[buff.row_kazu] == NULL)
    {
        perror("Unable to allocate memory for string duplication");
        exit(1);
    }

    buff.row_kazu++;
    buff.rows[buff.row_kazu] = NULL;
}
