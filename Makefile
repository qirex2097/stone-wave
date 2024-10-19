# コンパイラの指定
CC = gcc

# コンパイルオプションの指定
MLX_DIR = minilibx-linux
CFLAGS = -I$(MLX_DIR) -g
LDFLAGS = -L$(MLX_DIR) -lm -lmlx -lX11 -lXext

# 実行ファイル名
TARGET = stone

# ソースファイル
SRCS = $(wildcard *.c)

# オブジェクトファイルの指定（.c -> .o に変換）
OBJS = $(SRCS:.c=.o)

# ルール定義
all: $(TARGET)

# 実行ファイルのビルドルール
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

# 各ソースファイルのオブジェクトファイルを作成するルール
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# クリーンアップルール
clean:
	rm -f $(OBJS) $(TARGET)

# ディペンデンシの自動生成（オプション）
depend: $(SRCS)
	$(CC) -MM $(SRCS) > .depend

# ディペンデンシファイルのインクルード
-include .depend

CXX = g++
CXXFLAGS = -I./googletest -I./googletest/include -pthread
TEST_SRC = tests/my_tests.cpp
TEST_TARGET_SRCS = drawing.c
TEST_TARGET = run_tests
TEST_OBJ = $(TEST_TARGET_SRCS:.c=.o) $(TEST_SRC:.cpp=.o) googletest/src/gtest-all.o googletest/src/gtest_main.o

tests: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) $(OBJS)
	$(CXX) $(TEST_OBJ) -o $(TEST_TARGET)

.PHONY: tests
