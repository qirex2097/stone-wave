#include <mlx.h>

#define WINDOW_W 800
#define WINDOW_H 800

int main(void)
{
    void *mlx;
    void *mlx_win;

    mlx = mlx_init();
    mlx_win = mlx_new_window(mlx, WINDOW_W, WINDOW_H, "HELLO");
    mlx_loop(mlx);

    return 0;
}