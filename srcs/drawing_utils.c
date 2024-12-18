#include <stdlib.h>
#include "stone.h"

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
    t_pos p1, p2, p3, p4;

    p1.x = line1->x0;
    p1.y = line1->y0;
    p2.x = line1->x1;
    p2.y = line1->y1;
    p3.x = line2->x0;
    p3.y = line2->y0;
    p4.x = line2->x1;
    p4.y = line2->y1;
    int o1 = orientation(&p1, &p2, &p3);
    int o2 = orientation(&p1, &p2, &p4);
    int o3 = orientation(&p3, &p4, &p1);
    int o4 = orientation(&p3, &p4, &p2);

    if (o1 != o2 && o3 != o4)
        return 1;

    if (o1 == 0 && on_segment(&p1, &p2, &p3))
        return 1;
    if (o2 == 0 && on_segment(&p1, &p2, &p4))
        return 1;
    if (o3 == 0 && on_segment(&p3, &p4, &p1))
        return 1;
    if (o4 == 0 && on_segment(&p3, &p4, &p2))
        return 1;

    return 0;
}

int get_intersection(t_line *line0, t_line *line1, t_pos *cross_point)
{
    if (!do_intersect(line0, line1))
        return 0;

    t_pos p1, p2, p3, p4;

    p1.x = line0->x0;
    p1.y = line0->y0;
    p2.x = line0->x1;
    p2.y = line0->y1;
    p3.x = line1->x0;
    p3.y = line1->y0;
    p4.x = line1->x1;
    p4.y = line1->y1;

    int x1 = p1.x, y1 = p1.y;
    int x2 = p2.x, y2 = p2.y;
    int x3 = p3.x, y3 = p3.y;
    int x4 = p4.x, y4 = p4.y;

    long long denom = (long long)(x1 - x2) * (y3 - y4) - (long long)(y1 - y2) * (x3 - x4);

    if (denom == 0)
    {
        // 同一直線上に存在するか
        if (orientation(&p1, &p2, &p3) == COLLINEAR)
        {
            // line0上にline1の端点が乗っているか
            if (on_segment(&p1, &p2, &p3))
            {
                cross_point->x = p3.x;
                cross_point->y = p3.y;
                return 1;
            }
            else if (on_segment(&p1, &p2, &p4))
            {
                cross_point->x = p4.x;
                cross_point->y = p4.y;
                return 1;
            }
        }
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

void scale_segment(t_line *line, int a, t_line *line_new)
{
    line_new->x0 = line->x0;
    line_new->y0 = line->y0;
    line_new->x1 = line->x0 + a * (line->x1 - line->x0);
    line_new->y1 = line->y0 + a * (line->y1 - line->y0);
}
