#include <gtest/gtest.h>

extern "C"
{
#include "../srcs/stone.h" // テスト対象のヘッダー
}

TEST(FindNextGridCrossingTest, FunctionReturnsExpectedValue)
{
    int grid_size = 100;
    t_pos player_pos = {10, 20};
    t_vec dir = {3, 1};
    t_pos cross_point;
    cross_point = find_next_grid_crossing(player_pos, dir, grid_size);
    EXPECT_EQ(cross_point.x, 100);
    player_pos.x = 150;
    player_pos.y = 20;
    dir.x = -3;
    dir.y = 1;
    cross_point = find_next_grid_crossing(player_pos, dir, grid_size);
    EXPECT_EQ(cross_point.x, 100);
    player_pos.x = 100;
    player_pos.y = 920;
    dir.x = 0;
    dir.y = -1;
    cross_point = find_next_grid_crossing(player_pos, dir, grid_size);
    EXPECT_EQ(cross_point.y, 900);
    player_pos.x = 550;
    player_pos.y = 550;
    dir.x = -1;
    dir.y = -1;
    cross_point = find_next_grid_crossing(player_pos, dir, grid_size);
    EXPECT_EQ(cross_point.x, 500);
    EXPECT_EQ(cross_point.y, 500);
}

TEST(BuffTest, Buff)
{
    const char *str[] = {"HELLO", "WORLD"};
    t_buff buff = {0, NULL};
    my_string_put(&buff, str[0]);
    my_string_put(&buff, str[1]);
    EXPECT_EQ(buff.row_kazu, 2);

    int i = 0;
    char *p = buff.rows[i];
    while (i < buff.row_kazu)
    {
        EXPECT_STREQ(buff.rows[i], str[i]);
        i++;
    }

    my_string_put(&buff, "");
    EXPECT_EQ(buff.row_kazu, 3);
    EXPECT_STREQ(buff.rows[2], "");

    my_string_put(&buff, NULL);
    EXPECT_EQ(buff.row_kazu, 3);

    cleanup_buff(&buff);
    EXPECT_EQ(buff.row_kazu, 0);
    EXPECT_TRUE(buff.rows == NULL);
}
