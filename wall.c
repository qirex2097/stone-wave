#include <stdlib.h>
#include <stdio.h>
#include "stone.h"

static t_wall *wall;
static int wall_kazu;

int init_wall(t_wall *param, int kazu)
{
    wall = param;
    wall_kazu = kazu;
    return 0;
}

int draw_wall(t_vars *vars)
{
    for (int i = 0; i < 4; i++)
    {
        draw_line(&vars->img, &vars->camera, &wall[i].line, 0x00ff4080);
    }
    return 0;
}

int update_wall(t_vars *vars)
{
    (void)vars;
    return 0;
}

t_wall *get_wall(int idx)
{
    if (idx < 0 || wall_kazu <= idx)
        return NULL;
    return &wall[idx];
}
