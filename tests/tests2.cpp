#include <gtest/gtest.h>

extern "C"
{
#include "../srcs/stone.h" // テスト対象のヘッダー
}

TEST(MyLibraryTest2, FunctionReturnsExpectedValue)
{
    t_map *map = init_map(10, 10);
    t_pos player_pos = {10, 20};
    t_vec dir = {3, 1};
    t_pos cross_point;
    // void ray_grid_intersection(t_map *map, t_pos ray_origin, t_vec ray_direction)
    find_next_grid_crossing(map, player_pos, dir, &cross_point);
    player_pos.x = 150;
    player_pos.y = 20;
    dir.x = -3;
    dir.y = 1;
    find_next_grid_crossing(map, player_pos, dir, &cross_point);
    player_pos.x = 100;
    player_pos.y = 920;
    dir.x = 0;
    dir.y = -1;
    find_next_grid_crossing(map, player_pos, dir, &cross_point);
    player_pos.x = 550;
    player_pos.y = 550;
    dir.x = -1;
    dir.y = -1;
    find_next_grid_crossing(map, player_pos, dir, &cross_point);
    free(map);
}

TEST(BuffTest, Buff)
{
    const char *str[] = {"HELLO", "WORLD"};
    t_buff buff;
    buff.row_kazu = 0;
    buff.rows = NULL;
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

    cleanup_buff(&buff);
    EXPECT_EQ(buff.row_kazu, 0);
    EXPECT_TRUE(buff.rows == NULL);
}
