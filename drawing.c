#include <mlx.h>
#include "stone.h"

void my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
    if (x < 0 || data->w <= x || y < 0 || data->h <= y)
        return;
    char *dst;
    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}

void draw_line(t_img *data, int x0, int y0, int x1, int y1, int color)
{
    int dx, dy;
    int sx, sy;
    int err;

    if (x0 < x1)
    {
        dx = x1 - x0;
        sx = 1;
    }
    else
    {
        dx = x0 - x1;
        sx = -1;
    }
    if (y0 < y1)
    {
        dy = y1 - y0;
        sy = 1;
    }
    else
    {
        dy = y0 - y1;
        sy = -1;
    }
    err = dx - dy;

    while (x0 != x1 && y0 != y1)
    {
        int e2;

        my_mlx_pixel_put(data, x0, y0, color);
        e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void draw_circle(t_img *img, int x_center, int y_center, int radius, int color)
{
    int x = radius;
    int y = 0;
    int decision = 1 - radius;

    while (x >= y)
    {
        my_mlx_pixel_put(img, x_center + x, y_center + y, color);
        my_mlx_pixel_put(img, x_center - x, y_center + y, color);
        my_mlx_pixel_put(img, x_center + x, y_center - y, color);
        my_mlx_pixel_put(img, x_center - x, y_center - y, color);
        my_mlx_pixel_put(img, x_center + y, y_center + x, color);
        my_mlx_pixel_put(img, x_center - y, y_center + x, color);
        my_mlx_pixel_put(img, x_center + y, y_center - x, color);
        my_mlx_pixel_put(img, x_center - y, y_center - x, color);

        y++;

        if (decision <= 0)
        {
            decision += 2 * y + 1;
        }
        else
        {
            x--;
            decision += 2 * (y - x) + 1;
        }
    }
}