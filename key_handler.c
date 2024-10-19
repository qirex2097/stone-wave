#include <stdlib.h>
#include "stone.h"

int key_press_handler(int keycode, void *param)
{
    t_vars *vars = param;
    t_mouse *mouse = &vars->mouse;

    // printf("press:%d\n", keycode);

    if (keycode == KEY_UP)
        mouse->button |= MOVE_FORWARD;
    if (keycode == KEY_DOWN)
        mouse->button |= MOVE_BACKWARD;
    if (keycode == KEY_LEFT)
        mouse->button |= TURN_LEFT;
    if (keycode == KEY_RIGHT)
        mouse->button |= TURN_RIGHT;
    if (keycode == KEY_W)
        mouse->button |= MOVE_UP;
    if (keycode == KEY_S)
        mouse->button |= MOVE_DOWN;
    if (keycode == KEY_A)
        mouse->button |= MOVE_LEFT;
    if (keycode == KEY_D)
        mouse->button |= MOVE_RIGHT;

    return 0;
}

int key_release_handler(int keycode, void *param)
{
    t_vars *vars = param;
    t_mouse *mouse = &vars->mouse;

    // printf("release:%d\n", keycode);
    if (keycode == KEY_ESC)
    {
        cleanup(vars);
        exit(0);
    }
    if (keycode == KEY_UP)
        mouse->button &= ~(MOVE_FORWARD);
    if (keycode == KEY_DOWN)
        mouse->button &= ~(MOVE_BACKWARD);
    if (keycode == KEY_LEFT)
        mouse->button &= ~(TURN_LEFT);
    if (keycode == KEY_RIGHT)
        mouse->button &= ~(TURN_RIGHT);
    if (keycode == KEY_W)
        mouse->button &= ~(MOVE_UP);
    if (keycode == KEY_S)
        mouse->button &= ~(MOVE_DOWN);
    if (keycode == KEY_A)
        mouse->button &= ~(MOVE_LEFT);
    if (keycode == KEY_D)
        mouse->button &= ~(MOVE_RIGHT);

    return 0;
}

int mouse_down_handler(int button, int x, int y, void *param)
{
    t_vars *vars = param;
    t_mouse *mouse = &vars->mouse;

    switch (button)
    {
    case BUTTON_RIGHT:
        if (mouse->button & SHOW_MINIWINDOW)
            mouse->button &= ~(SHOW_MINIWINDOW);
        else
        {
            mouse->button |= SHOW_MINIWINDOW;
            vars->mini_x = x;
            vars->mini_y = y;
        }
        break;
    }

    if (button == BUTTON_LEFT)
    {
        mouse->pos.x = x;
        mouse->pos.y = y;
        mouse->button |= CENTER;
    }
    if (button == BUTTON_UP)
    {
        mouse->button |= SCALE_UP;
    }
    if (button == BUTTON_DOWN)
    {
        mouse->button |= SCALE_DOWN;
    }

    return 0;
}
