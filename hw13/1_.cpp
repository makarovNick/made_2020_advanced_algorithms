#include <iomanip>
#include <iostream>
#include <cmath>
#include <vector>

#define PI 3.14159265359
#define EPS 1e-10
#define EQ(a, b) (std::abs((a) - (b)) < EPS)

using float64_t = long double;

template<typename T>
struct Point
{
    T X;
    T Y;
    template<typename T2>
    float64_t distance(const Point<T2>& p) const 
    {
        return std::hypot(X - p.X, Y - p.Y);
    }
    T squared_distance(Point<T> p) const
    {
        return (X - p.X) * (X - p.X) + (Y - p.Y) * (Y - p.Y);
    }
};

struct Circle
{
    Point<int> O;
    int R;
};

std::vector<Point<float64_t>> CircleIntersection(const Circle& a, const Circle& b)
{
    std::vector<Point<float64_t>> res;
    if (a.O.X == b.O.X && a.O.Y == b.O.Y)
    {
        if (a.R == b.R)
        {
            res.resize(3);
            return res;
        }
    }
    if (a.O.squared_distance(b.O) > (a.R + b.R) * (a.R + b.R) || a.O.squared_distance(b.O) < (a.R - b.R) * (a.R - b.R))
    {
        return res;
    }

    int A = (b.O.X - a.O.X);
    int B = (b.O.Y - a.O.Y);
    int C = A * A + B * B + a.R * a.R - b.R * b.R;
    A *= -2;
    B *= -2;

    int square = A * A + B * B;
 
    float64_t x0 = -A * C * 1.0 / square;
    float64_t y0 = -B * C * 1.0 / square;
    if (C * C <= a.R * a.R * square + EPS)    
    {
        if (std::abs(C * C - a.R * a.R * square) <= EPS)
        {
            res.push_back({x0 + a.O.X, y0 + a.O.Y});
        }
        else
        {
            float64_t mult = std::sqrt((a.R * a.R - C * 1.0 * C / square) / square);
            if (!std::isnan(mult))
            {
                res.push_back({x0 + B * mult + a.O.X, y0 - A * mult + a.O.Y});
                res.push_back({x0 - B * mult + a.O.X, y0 + A * mult + a.O.Y});

                if (std::abs(res[1].X - res[0].X) < EPS && std::abs(res[1].Y - res[0].Y) < EPS)
                {
                    res.pop_back();
                }
            }
        }
    }

    return res;
}

template<typename T1, typename T2>
auto FindH(const Point<T1>& a, const Point<T2>& b) -> Point<decltype(a.X * b.X)>
{
    return { (a.X + b.X) / 2, (a.Y + b.Y) / 2 };
}

int main()
{
    std::cout << std::setprecision(15);
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i)
    {
        Circle a, b;
        std::cin >> a.O.X >> a.O.Y >> a.R;
        std::cin >> b.O.X >> b.O.Y >> b.R;
        auto res = CircleIntersection(a, b);
        std::cout << res.size() << '\n';
        if (res.size() == 1)
        {
            std::cout << res[0].X << ' ' << res[0].Y << '\n';
        }
        else if (res.size() == 2)
        {
            auto H = FindH(res[0], res[1]);
            std::cout << H.X << ' ' << H.Y << '\n';
            std::cout << H.distance(a.O) << ' ' << H.distance(res[0]) << '\n';
            std::cout << res[0].X << ' ' << res[0].Y << '\n';
            std::cout << res[1].X << ' ' << res[1].Y << '\n';
        }   
    }

    return 0;
}