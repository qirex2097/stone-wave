#include <mlx.h>
#include <stdlib.h>
#include "stone.h"

int is_out_of_bounds(int x, int y, int w, int h)
{
    return (x < 0 || w <= x || y < 0 || h <= y);
}

void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    if (is_out_of_bounds(x, y, img->w, img->h))
        return;
    char *dst;
    dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}

void my_mlx_draw_line(t_img *data, int x0, int y0, int x1, int y1, int color)
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

    while (1)
    {
        int e2;

        my_mlx_pixel_put(data, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
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

void draw_circle_s(t_img *img, t_pos *center, int radius, int color)
{
    int x_center = center->x;
    int y_center = center->y;
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

void draw_line_s(t_img *data, t_line *line, int color)
{
    my_mlx_draw_line(data, line->p0.x, line->p0.y, line->p1.x, line->p1.y, color);
}

void put_pixel(t_img *img, t_camera *field, int x, int y, int color)
{
    t_pos pos;
    t_pos_s screen;
    pos.x = x;
    pos.y = y;
    convert_to_screen(&pos, &screen, img, field);
    my_mlx_pixel_put(img, screen.x, screen.y, color);
}

void draw_line(t_img *img, t_camera *field, t_line *line, int color)
{
    t_pos_s p0, p1;
    convert_to_screen(&line->p0, &p0, img, field);
    convert_to_screen(&line->p1, &p1, img, field);
    my_mlx_draw_line(img, p0.x, p0.y, p1.x, p1.y, color);
}

void draw_circle(t_img *img, t_camera *camera, t_pos *center, int radius, int color)
{
    int x_center = center->x;
    int y_center = center->y;
    int x = radius;
    int y = 0;
    int decision = 1 - radius;

    while (x >= y)
    {
        put_pixel(img, camera, x_center + x, y_center + y, color);
        put_pixel(img, camera, x_center - x, y_center + y, color);
        put_pixel(img, camera, x_center + x, y_center - y, color);
        put_pixel(img, camera, x_center - x, y_center - y, color);
        put_pixel(img, camera, x_center + y, y_center + x, color);
        put_pixel(img, camera, x_center - y, y_center + x, color);
        put_pixel(img, camera, x_center + y, y_center - x, color);
        put_pixel(img, camera, x_center - y, y_center - x, color);

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
