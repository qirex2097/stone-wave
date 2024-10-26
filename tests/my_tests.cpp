#include <gtest/gtest.h>
#include <math.h>
#include <string>
extern "C"
{
#include "../srcs/stone.h" // テスト対象のヘッダー
    extern double cosine_angle(int x1, int y1, int x2, int y2, int x3, int y3);
    extern void map_point_on_line(t_line *line, int w, int a, t_pos *point);
    extern int do_intersect(t_line *line1, t_line *line2);

    int mlx_string_put(void *xvar, void *win, int x, int y, int color, char *string) { return 0; }
    void draw_miniwindow(t_vars *vars, t_wall *wall, t_line *way, int sx) {}
    void draw_player_view(t_vars *vars, t_line *screen) {}
}

TEST(MyLibraryTest, FunctionReturnsExpectedValue)
{
    EXPECT_EQ(distance_squared(0, 0, 1, 1), 2); // 例: my_functionは2倍する関数
    EXPECT_EQ(cosine_angle(0, 0, 0, 1, 1, 1), 0);

    t_line line, line_new;
    line.p0.x = 0;
    line.p0.y = 0;
    line.p1.x = 100;
    line.p1.y = 200;
    scale_segment(&line, 3, &line_new);
    EXPECT_EQ(line_new.p0.x, 0);
    EXPECT_EQ(line_new.p0.y, 0);
    EXPECT_EQ(line_new.p1.x, 300);
    EXPECT_EQ(line_new.p1.y, 600);
}

/*----------------------------------------*/

static t_wall wall[] = {
    {{{-50, -50}, {-50, 50}}, 0x00ff0000},
    {{{-50, 50}, {50, 50}}, 0x0080ff00},
    {{{50, 50}, {50, -50}}, 0x008060ff},
    {{{50, -50}, {-50, -50}}, 0x00ff8000},
};

class WallTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        init_wall(wall, sizeof(wall) / sizeof(t_wall));
    }

    void TearDown() override
    {
    }
};

TEST_F(WallTest, Test)
{
    t_line line, screen_line;
    t_wall *wall;
    t_pos cross_point;
    int color;
    wall = get_wall(0);
    EXPECT_EQ(wall->line.p0.x, -50);
    line.p0.x = 0;
    line.p0.y = 0;
    line.p1.x = -100;
    line.p1.y = 0;
    EXPECT_TRUE(get_intersection(&wall->line, &line, &cross_point));
    EXPECT_EQ(cross_point.x, -50);
    EXPECT_EQ(cross_point.y, 0);
    line.p0.x = 0;
    line.p0.y = 0;
    line.p1.x = -50;
    line.p1.y = -50;
    EXPECT_TRUE(get_intersection(&wall->line, &line, &cross_point));
    EXPECT_EQ(cross_point.x, -50);
    EXPECT_EQ(cross_point.y, -50);
    line.p0.x = -50;
    line.p0.y = -100;
    line.p1.x = -50;
    line.p1.y = -50;
    EXPECT_TRUE(do_intersect(&wall->line, &line));
    EXPECT_TRUE(get_intersection(&wall->line, &line, &cross_point));
    EXPECT_EQ(cross_point.x, -50);
    EXPECT_EQ(cross_point.y, -50);
}

class MapTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(MapTest, Test)
{
    t_map *map;
    map = init_map(10, 10);
    EXPECT_TRUE(map);
    EXPECT_STREQ(map->data[0], "1111111111");
    EXPECT_STREQ(map->data[1], "1000000001");
    EXPECT_STREQ(map->data[10 - 1], "1111111111");
    free_map(map);
}