#include <iomanip>
#include <deque>
#include <algorithm>
#include <iostream>
#include <set>
#include <cmath>

#define EPS 1e-7

const double MAX_COORD = 10'000 + 1;

struct Point
{
    double X;
    double Y;
};

struct Plane
{
    Plane()
    {
        A = B = C = 0;
    }

    Plane(const Point& a, const Point& b)
    {
        A = b.Y - a.Y;
        B = a.X - b.X;
        C = b.X * a.Y - a.X * b.Y;
    }

    bool contains(const Point& a) const
    {
        return (A * a.X + B * a.Y + C) + EPS >= 0;
    }

    double A;
    double B;
    double C;
};

Point intersection(const Plane& A, const Plane& B)
{
    double determinant = A.A * B.B - B.A * A.B;

    if (std::abs(determinant) <= EPS)
    {
        return { NAN, NAN };
    }
    double x = -(B.B * A.C - A.B * B.C) / determinant;
    double y = -(A.A * B.C - B.A * A.C) / determinant;
    return { x, y };
}

double area(const std::deque<Point>& points)
{
    double res = 0;
    for (int i = 0; i < points.size(); ++i)
    {
        int prev = ((i == 0) ? points.size() - 1 : i - 1);
        if (std::abs(std::abs(points[i].X) - MAX_COORD) < EPS
            || std::abs(std::abs(points[i].Y) - MAX_COORD) < EPS)
        {
            return 0;
        }
        res += (points[prev].X + points[i].X) * (points[prev].Y - points[i].Y);
    }

    return std::abs(res) / 2;
}

void cut(std::deque<Point>& points, const Plane& plane)
{
    for (auto i = points.begin(); i != points.end(); ++i)
    {
        if (!plane.contains(*i))
        {
            Point temp;
            auto first_intersection = points.end();
            for (int j = 0; j < points.size(); ++j)
            {
                auto next = ((i + 1) == points.end()) ? points.begin() : (i + 1);
                if (!plane.contains(*i) && plane.contains(*next))
                {
                    temp = intersection(Plane(*i, *next), plane);
                    first_intersection = i;
                }
                if (plane.contains(*i) && !plane.contains(*next))
                {
                    *next = intersection(Plane(*i, *next), plane);
                    if (next == first_intersection)
                    {
                        first_intersection = points.insert(next + 1, temp);
                        return;
                    }
                    else
                    {
                        *first_intersection = temp;
                    }
                    if (std::distance(first_intersection, next) < 0)
                    {
                        points.erase(next + 1, first_intersection);
                    }
                    else
                    {
                        points.erase(next + 1, points.end());
                        points.erase(points.begin(), first_intersection);
                    }
                    return;
                }
                i++;
                if (i == points.end())
                {
                    i = points.begin();
                }
            }
        }
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int n;
    std::cin >> n;
    std::deque<Plane> planes(n);

    std::cout << std::setprecision(10);

    for (int i = 0; i < n; ++i)
    {
        std::cin >> planes[i].A >> planes[i].B >> planes[i].C;
    }

    std::deque<Point> points(4);
    points[0] = { -MAX_COORD, -MAX_COORD };
    points[1] = { -MAX_COORD, MAX_COORD };
    points[2] = { MAX_COORD, MAX_COORD };
    points[3] = { MAX_COORD, -MAX_COORD };

    for (int i = 0; i < n; ++i)
    {
        cut(points, planes[i]);
    }

    std::cout << area(points);

    return 0;
}