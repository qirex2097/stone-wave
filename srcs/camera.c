#include "stone.h"

#define SCALE_STEP 50

void init_camera(t_camera *camera, int x, int y, int w, int h)
{
    camera->x = x;
    camera->y = y;
    camera->w = w;
    camera->h = h;

    return;
}

int update_camera(t_vars *vars)
{
    t_mouse *mouse = &vars->mouse;
    t_img *img = &vars->img;
    t_camera *camera = &vars->camera;

    if (mouse->button & SCALE_UP)
    {
        camera->h -= SCALE_STEP;
        camera->w -= SCALE_STEP;
        camera->x += SCALE_STEP / 2;
        camera->y += SCALE_STEP / 2;
        mouse->button &= ~(SCALE_UP);
    }
    if (mouse->button & SCALE_DOWN)
    {
        camera->h += SCALE_STEP;
        camera->w += SCALE_STEP;
        camera->x -= SCALE_STEP / 2;
        camera->y -= SCALE_STEP / 2;
        mouse->button &= ~(SCALE_DOWN);
    }
    if (mouse->button & CENTER)
    {
        t_pos pos;
        convert_to_field(&mouse->pos, &pos, img, camera);
        camera->x = pos.x - camera->w / 2;
        camera->y = pos.y - camera->h / 2;
        // printf("(%d,%d)\n", field->x, field->y);
        mouse->button &= ~(CENTER);
    }

    return 0;
}
