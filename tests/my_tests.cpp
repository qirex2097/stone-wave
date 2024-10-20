#include <gtest/gtest.h>
#include <math.h>
#include <string>
extern "C"
{
#include "../stone.h" // テスト対象のヘッダー
    extern double cosine_angle(int x1, int y1, int x2, int y2, int x3, int y3);
    extern void map_point_on_line(t_line *line, int w, int a, t_pos *point);
    extern int find_intersection_point(t_line *line, t_pos *cross_point, int *color);

    int mlx_string_put(void *xvar, void *win, int x, int y, int color, char *string) { return 0; }
}

TEST(MyLibraryTest, FunctionReturnsExpectedValue)
{
    EXPECT_EQ(distance_squared(0, 0, 1, 1), 2); // 例: my_functionは2倍する関数
    EXPECT_EQ(cosine_angle(0, 0, 0, 1, 1, 1), 0);
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
}