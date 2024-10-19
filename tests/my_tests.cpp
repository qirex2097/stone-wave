#include <gtest/gtest.h>
extern "C"
{
#include "../stone.h" // テスト対象のヘッダー
}

TEST(MyLibraryTest, FunctionReturnsExpectedValue)
{
    EXPECT_EQ(distance_squared(0, 0, 1, 1), 2); // 例: my_functionは2倍する関数
}
