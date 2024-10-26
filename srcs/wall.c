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
    for (int i = 0; i < wall_kazu; i++)
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

int find_intersection_point(t_vars *vars, t_line *line, t_pos *cross_point, t_wall *wall_)
{
    t_wall *wall;
    int j = 0;
    int flg = 0;
    while (wall = get_wall(j))
    {
        if (get_intersection(&wall->line, line, cross_point))
        {
            wall_ = wall;
            flg = 1;
        }
        j++;
    }
    return flg;
}
