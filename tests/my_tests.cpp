#include <gtest/gtest.h>
#include <math.h>
#include <string>
extern "C"
{
#include "../srcs/stone.h" // テスト対象のヘッダー
    extern double cosine_angle(int x1, int y1, int x2, int y2, int x3, int y3);
    extern t_pos map_point_on_line(t_line *line, int w, int a);
    extern int do_intersect(t_line *line1, t_line *line2);

    int mlx_string_put(void *xvar, void *win, int x, int y, int color, char *string) { return 0; }
    void draw_miniwindow(t_vars *vars, t_wall *wall, t_line *way, int sx) {}
    void draw_player_view(t_vars *vars, t_line *screen) {}

    /* map.c */
    int is_not_space(t_map *map, int map_x, int map_y);
}

TEST(MyLibraryTest, FunctionReturnsExpectedValue)
{
    EXPECT_EQ(distance_squared(0, 0, 1, 1), 2); // 例: my_functionは2倍する関数
    EXPECT_EQ(cosine_angle(0, 0, 0, 1, 1, 1), 0);

    t_line line, line_new;
    line.x0 = 0;
    line.y0 = 0;
    line.x1 = 100;
    line.y1 = 200;
    scale_segment(&line, 3, &line_new);
    EXPECT_EQ(line_new.x0, 0);
    EXPECT_EQ(line_new.y0, 0);
    EXPECT_EQ(line_new.x1, 300);
    EXPECT_EQ(line_new.y1, 600);
}

/*----------------------------------------*/

static t_wall wall[] = {
    {{-50, -50, -50, 50}, 0x00ff0000},
    {{-50, 50, 50, 50}, 0x0080ff00},
    {{50, 50, 50, -50}, 0x008060ff},
    {{50, -50, -50, -50}, 0x00ff8000},
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
    EXPECT_EQ(wall->line.x0, -50);
    line.x0 = 0;
    line.y0 = 0;
    line.x1 = -100;
    line.y1 = 0;
    EXPECT_TRUE(get_intersection(&wall->line, &line, &cross_point));
    EXPECT_EQ(cross_point.x, -50);
    EXPECT_EQ(cross_point.y, 0);
    line.x0 = 0;
    line.y0 = 0;
    line.x1 = -50;
    line.y1 = -50;
    EXPECT_TRUE(get_intersection(&wall->line, &line, &cross_point));
    EXPECT_EQ(cross_point.x, -50);
    EXPECT_EQ(cross_point.y, -50);
    line.x0 = -50;
    line.y0 = -100;
    line.x1 = -50;
    line.y1 = -50;
    EXPECT_TRUE(do_intersect(&wall->line, &line));
    EXPECT_TRUE(get_intersection(&wall->line, &line, &cross_point));
    EXPECT_EQ(cross_point.x, -50);
    EXPECT_EQ(cross_point.y, -50);
}

class MapTest : public ::testing::Test
{
public:
    t_map *map;

protected:
    void SetUp() override
    {
        map = init_map(10, 10);
    }

    void TearDown() override
    {
        free_map(map);
    }
};

TEST_F(MapTest, Test)
{
    EXPECT_TRUE(map);
    EXPECT_STREQ(map->data[0], "1111111111");
    EXPECT_STREQ(map->data[2], "1000000001");
    EXPECT_STREQ(map->data[10 - 1], "1111111111");
}

TEST_F(MapTest, Wall)
{
    EXPECT_TRUE(is_not_space(map, 0, 0));
    EXPECT_TRUE(is_not_space(map, 0, 1));
    EXPECT_TRUE(is_not_space(map, 1, 1));
    EXPECT_TRUE(is_not_space(map, -1, 4));
    EXPECT_TRUE(is_not_space(map, 5, -1));
}