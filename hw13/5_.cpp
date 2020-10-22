#include <iomanip>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

#define EPS 1e-7

struct Point
{
    double x;
    double y;

    Point subtract(const Point& p) const
    {
        return Point{ x - p.x, y - p.y };
    }

    double distance(const Point& p) const
    {
        return std::hypot(x - p.x, y - p.y);
    }

    double cross(const Point& p) const
    {
        return x * p.y - y * p.x;
    }
};

struct Circle
{
    Point O;
    double R;

    bool contains(const Point& p) const
    {
        return O.distance(p) <= R + EPS;
    }

    bool contains(const std::vector<Point>& ps) const
    {
        for (const Point& p : ps)
        {
            if (!contains(p))
            {
                return false;
            }
        }
        return true;
    }
};

const Circle DEG_CIRCLE = { Point{ 0, 0 }, -1 };

Circle CircleOnDiameter(const Point& a, const Point& b)
{
    Point O{ (a.x + b.x) / 2, (a.y + b.y) / 2 };

    return Circle{ O, std::max(O.distance(a), O.distance(b)) };
}


Circle CircumCircle(const Point& a, const Point& b, const Point& O)
{
    double ox = (std::min(std::min(a.x, b.x), O.x) + std::max(std::max(a.x, b.x), O.x)) / 2;
    double oy = (std::min(std::min(a.y, b.y), O.y) + std::max(std::max(a.y, b.y), O.y)) / 2;
    double ax = a.x - ox;
    double ay = a.y - oy;
    double bx = b.x - ox;
    double by = b.y - oy;
    double cx = O.x - ox;
    double cy = O.y - oy;
    double d = (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by)) * 2;
    if (d == 0)
    {
        return DEG_CIRCLE;
    }
    double x = ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay)
                   + (cx * cx + cy * cy) * (ay - by)) / d;
    double y = ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx)
                   + (cx * cx + cy * cy) * (bx - ax)) / d;

    Point p{ ox + x, oy + y };
    double R = std::max(std::max(p.distance(a), p.distance(b)), p.distance(O));

    return Circle{ p, R };
}

Circle EnclosingCircleTwoPoints(const std::vector<Point>& points, int end, const Point& p, const Point& q)
{
    Circle circ = CircleOnDiameter(p, q);
    Circle left = DEG_CIRCLE;
    Circle right = DEG_CIRCLE;

    Point pq = q.subtract(p);
    for (int i = 0; i < end; i++)
    {
        const Point& R = points[i];
        if (circ.contains(R))
        {
            continue;
        }

        double cross = pq.cross(R.subtract(p));
        Circle O = CircumCircle(p, q, R);
        if (O.R < 0)
        {
            continue;
        }
        else if (cross > 0 && (left.R < 0 || pq.cross(O.O.subtract(p)) > pq.cross(left.O.subtract(p))))
        {
            left = O;
        }
        else if (cross < 0 && (right.R < 0 || pq.cross(O.O.subtract(p)) < pq.cross(right.O.subtract(p))))
        {
            right = O;
        }
    }

    if (left.R < 0 && right.R < 0)
    {
        return circ;
    }
    else if (left.R < 0)
    {
        return right;
    }
    else if (right.R < 0)
    {
        return left;
    }

    return left.R <= right.R ? left : right;
}

Circle EnclosingCircleOnePoint(const std::vector<Point>& points, int end, const Point& p)
{
    Circle O{ p, 0 };
    for (int i = 0; i < end; i++)
    {
        const Point& q = points[i];
        if (!O.contains(q))
        {
            if (O.R == 0)
            {
                O = CircleOnDiameter(p, q);
            }
            else
            {
                O = EnclosingCircleTwoPoints(points, i + 1, p, q);
            }
        }
    }
    return O;
}

Circle SmallestEnclosingCircle(std::vector<Point> points)
{
    std::default_random_engine rand((std::random_device())());
    std::shuffle(points.begin(), points.end(), rand);

    Circle O = DEG_CIRCLE;
    for (int i = 0; i < points.size(); i++)
    {
        const Point& p = points[i];
        if (O.R < 0 || !O.contains(p))
        {
            O = EnclosingCircleOnePoint(points, i + 1, p);
        }
    }
    return O;
}

int main()
{
    int n;
    std::cin >> n;
    std::vector<Point> points(n);
    std::cout << std::setprecision(10);

    for (int i = 0; i < n; ++i)
    {
        std::cin >> points[i].x >> points[i].y;
    }

    Circle res = SmallestEnclosingCircle(points);

    std::cout << res.O.x << ' ' << res.O.y << '\n';
    std::cout << res.R;

    return 0;
}