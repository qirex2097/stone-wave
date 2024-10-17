#include <stdlib.h>
#include "stone.h"

static t_line wall[] = {
    {{-50, -50}, {-50, 50}},
    {{-50, 50}, {50, 50}},
    {{50, 50}, {50, -50}},
    {{50, -50}, {-50, -50}},
};

int init_wall()
{
    return 0;
}

int draw_wall(t_vars *vars)
{
    for (int i = 0; i < 4; i++)
    {
        draw_line(&vars->img, &vars->camera, &wall[i], 0x00ff4080);
    }
    return 0;
}

int update_wall(t_vars *vars)
{
    (void)vars;
    return 0;
}

t_line *get_wall(int idx)
{
    if (idx < 0 || sizeof(wall) / sizeof(t_line) <= idx)
        return NULL;
    return &wall[idx];
}
