#include <math.h>

// ベクトルの内積を計算
long long dot_product(int x1, int y1, int x2, int y2)
{
    return (long long)x1 * x2 + (long long)y1 * y2;
}

// ベクトルの長さの平方を計算
long long vector_length_squared(int x, int y)
{
    return (long long)x * x + (long long)y * y;
}

// 三点 (x1, y1), (x2, y2), (x3, y3) を結ぶ直線の角度のコサインを計算
double cosine_angle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    // ベクトル v1 = (x2 - x1, y2 - y1), ベクトル v2 = (x3 - x2, y3 - y2)
    int v1x = x1 - x2, v1y = y1 - y2;
    int v2x = x3 - x2, v2y = y3 - y2;

    // 内積 v1・v2
    long long dot = dot_product(v1x, v1y, v2x, v2y);

    // v1 の長さの平方と v2 の長さの平方
    long long len_v1_sq = vector_length_squared(v1x, v1y);
    long long len_v2_sq = vector_length_squared(v2x, v2y);

    // ベクトルの大きさ (長さ) を平方根で求める
    double len_v1 = sqrt((double)len_v1_sq);
    double len_v2 = sqrt((double)len_v2_sq);

    if (len_v1 == 0.0 || len_v2 == 0.0)
        return 0.0;
    else
        return (double)dot / (len_v1 * len_v2); // cosθ = dot / (|v1| * |v2|)
}

// 2点間の距離の平方を計算する関数
long long distance_squared(int x1, int y1, int x2, int y2)
{
    long long dx = x2 - x1;
    long long dy = y2 - y1;
    return dx * dx + dy * dy;
}
