#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mlx.h>
#include "stone.h"

t_buff buff;

t_buff *init_buff()
{
    int i;
    buff.row_kazu = 0;
    i = 0;
    while (i < ROW_MAX + 1)
    {
        buff.rows[i] = NULL;
        i++;
    }
    return &buff;
}

void cleanup_buff()
{
    int i = 0;
    while (i < buff.row_kazu)
    {
        if (buff.rows[i])
            free(buff.rows[i]);
        buff.rows[i] = NULL;
        i++;
    }
    buff.row_kazu = 0;
}

void my_string_put(const char *str)
{
    if (str == NULL)
        return;
    if (buff.row_kazu >= ROW_MAX)
        return;

    buff.rows[buff.row_kazu] = strdup(str);
    if (buff.rows[buff.row_kazu] == NULL)
    {
        perror("Unable to allocate memory for string duplication");
        exit(1);
    }

    buff.row_kazu++;
}
