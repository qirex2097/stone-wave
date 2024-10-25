# コンパイラの指定
CC = gcc

# コンパイルオプションの指定
MLX_DIR = minilibx-linux
CFLAGS = -I$(MLX_DIR) -g
LDFLAGS = -L$(MLX_DIR) -lm -lmlx -lX11 -lXext

# 実行ファイル名
TARGET = stone

# ソースファイル
SRC_DIR = srcs
SRCS = $(wildcard $(SRC_DIR)/*.c)

# オブジェクトファイルの指定（.c -> .o に変換）
OBJ_DIR = objs
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# ルール定義
all: $(TARGET) tests

# 実行ファイルのビルドルール
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

# 各ソースファイルのオブジェクトファイルを作成するルール
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# クリーンアップルール
clean:
	rm -f $(OBJS) $(TARGET) $(TEST_OBJS) $(TEST_TARGET)

fclean: clean
	rm -f $(GOOGLE_TEST_OBJS)

# ディペンデンシの自動生成（オプション）
depend: $(SRCS)
	$(CC) -MM $(SRCS) > .depend

# ディペンデンシファイルのインクルード
-include .depend

# テスト用
CXX = g++
CXXFLAGS = -I$(GOOGLE_TEST_DIR) -I$(GOOGLE_TEST_DIR)/include -pthread
GOOGLE_TEST_DIR = googletest
GOOGLE_TEST_OBJS = $(GOOGLE_TEST_DIR)/src/gtest-all.o $(GOOGLE_TEST_DIR)/src/gtest_main.o
TEST_DIR = tests
TEST_SRCS = $(TEST_DIR)/my_tests.cpp
TEST_TARGET_SRCS = $(filter-out $(SRC_DIR)/main.c $(SRC_DIR)/mini_window.c $(SRC_DIR)/key_handler.c, $(wildcard $(SRC_DIR)/*.c))
TEST_TARGET = run_tests
TEST_OBJS = $(patsubst srcs/%.c, $(OBJ_DIR)/%.o, $(TEST_TARGET_SRCS)) $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TEST_SRCS))

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS) $(OBJS) $(GOOGLE_TEST_OBJS)
	$(CXX) $(TEST_OBJS) $(GOOGLE_TEST_OBJS) -o $(TEST_TARGET)

.PHONY: tests
