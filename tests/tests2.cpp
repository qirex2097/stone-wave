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
    // void ray_grid_intersection(t_map *map, t_pos ray_origin, t_vec ray_direction)
    ray_grid_intersection(map, player_pos, dir);
    player_pos.x = 150;
    player_pos.y = 20;
    dir.x = -3;
    dir.y = 1;
    ray_grid_intersection(map, player_pos, dir);
    player_pos.x = 100;
    player_pos.y = 920;
    dir.x = 0;
    dir.y = -1;
    ray_grid_intersection(map, player_pos, dir);
    player_pos.x = 550;
    player_pos.y = 550;
    dir.x = -1;
    dir.y = -1;
    ray_grid_intersection(map, player_pos, dir);
    free(map);
}