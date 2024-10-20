#include <mlx.h>
#include <stdlib.h>
#include "stone.h"

typedef enum
{
    COLLINEAR = 0,
    CLOCKWISE,
    COUNTERCLOCKWISE,
} Orientation;

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

int orientation(t_pos *p1, t_pos *p2, t_pos *p3)
{
    long long val = (p2->y - p1->y) * (p3->x - p2->x) - (p2->x - p1->x) * (p3->y - p2->y);
    if (val == 0)
        return COLLINEAR;
    return (val > 0) ? CLOCKWISE : COUNTERCLOCKWISE;
}

int on_segment(t_pos *p1, t_pos *p2, t_pos *p3)
{
    return (p3->x <= ((p1->x > p2->x) ? p1->x : p2->x) &&
            p3->x >= ((p1->x < p2->x) ? p1->x : p2->x) &&
            p3->y <= ((p1->y > p2->y) ? p1->y : p2->y) &&
            p3->y >= ((p1->y < p2->y) ? p1->y : p2->y));
}

int do_intersect(t_line *line1, t_line *line2)
{
    int o1 = orientation(&line1->p0, &line1->p1, &line2->p0);
    int o2 = orientation(&line1->p0, &line1->p1, &line2->p1);
    int o3 = orientation(&line2->p0, &line2->p1, &line1->p0);
    int o4 = orientation(&line2->p0, &line2->p1, &line1->p1);

    if (o1 != o2 && o3 != o4)
        return 1;

    if (o1 == 0 && on_segment(&line1->p0, &line1->p1, &line2->p0))
        return 1;
    if (o2 == 0 && on_segment(&line1->p0, &line1->p1, &line2->p1))
        return 1;
    if (o3 == 0 && on_segment(&line2->p0, &line2->p1, &line1->p0))
        return 1;
    if (o4 == 0 && on_segment(&line2->p0, &line2->p1, &line1->p1))
        return 1;

    return 0;
}

int get_intersection(t_line *line0, t_line *line1, t_pos *cross_point)
{
    int x1 = line0->p0.x, y1 = line0->p0.y;
    int x2 = line0->p1.x, y2 = line0->p1.y;
    int x3 = line1->p0.x, y3 = line1->p0.y;
    int x4 = line1->p1.x, y4 = line1->p1.y;

    long long denom = (long long)(x1 - x2) * (y3 - y4) - (long long)(y1 - y2) * (x3 - x4);

    if (denom == 0)
    {
        return 0;
    }

    long long num_x = (long long)(x1 * y2 - y1 * x2) * (x3 - x4) - (long long)(x1 - x2) * (x3 * y4 - y3 * x4);
    long long num_y = (long long)(x1 * y2 - y1 * x2) * (y3 - y4) - (long long)(y1 - y2) * (x3 * y4 - y3 * x4);

    cross_point->x = num_x / denom;
    cross_point->y = num_y / denom;

    return 1;
}

void convert_to_screen(t_pos *field, t_pos_s *screen, t_img *img, t_camera *camera)
{
    if (img == NULL || camera == NULL || camera->w == 0 || camera->h == 0)
        return;
    screen->x = (field->x - camera->x) * img->w / camera->w;
    screen->y = (field->y - camera->y) * img->h / camera->h;
}

void convert_to_field(t_pos_s *screen, t_pos *field, t_img *img, t_camera *camera)
{
    if (camera == NULL || img == NULL || img->w == 0 || img->h == 0)
        return;
    field->x = screen->x * camera->w / img->w + camera->x;
    field->y = screen->y * camera->h / img->h + camera->y;
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

// 2点間の距離の平方を計算する関数
long long distance_squared(int x1, int y1, int x2, int y2)
{
    long long dx = x2 - x1;
    long long dy = y2 - y1;
    return dx * dx + dy * dy;
}
