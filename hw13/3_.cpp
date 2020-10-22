#include <iomanip>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <algorithm>

#define PI 3.14159265359
#define EPS 1e-7
#define EQ(a, b) (std::abs((a) - (b)) < EPS)

using float64_t = long double;

struct Point
{
    bool operator<(const Point& other) const
    {
        return X < other.X || X == other.X && Y < other.Y;
    }

    float64_t angle(const Point& b)
    {
        if (EQ(X, b.X))
        {
            if (b.Y > Y)
            {
                return 180;
            }
            else
            {
                return 0;
            }
        }
        float64_t s = 0.0;
        if (X < b.X)
        {
            s = 360.0;
        }

        return atan2(X - b.X, Y - b.Y) * 180 / PI + s;
    }

    float64_t X;
    float64_t Y;
};

struct Line
{
    Line(){};
    Line(const Point& first, const Point& second)
    {
        A = first.Y - second.Y;
        B = second.X - first.X;
        C = first.X * second.Y - first.Y * second.X;
    }

    float64_t A;
    float64_t B;
    float64_t C;
};

Point intersection(const Line& a, const Line& b)
{
    float64_t denominator = a.A * b.B - a.B * b.A;
    if (EQ(denominator, 0))
    {
        return {NAN, NAN};
    }

    float64_t X = (b.C * a.B - b.B * a.C) / denominator;
    float64_t Y = (b.A * a.C - b.C * a.A) / denominator;

    return {X, Y};
}

float64_t area(const std::vector<Point>& points)
{
    float64_t res = 0;
    for (int i = 0; i < points.size(); ++i)
    {
        int prev = ((i == 0) ? points.size() - 1 : i - 1);
        res += (points[prev].X + points[i].X) * (points[prev].Y - points[i].Y);
    }

    return std::abs(res) / 2;
}

int main()
{
    int n;
    std::cin >> n;

    std::cout << std::setprecision(15);

    std::vector<std::set<Point>> intersections(n);
    std::vector<std::vector<int>> polygons;

    std::map<Point, int> point_to_index;
    std::vector<Point> index_to_point;

    std::vector<Line> lines(n);

    for (int i = 0; i < n; ++i)
    {
        Point first, second;
        std::cin >> first.X >> first.Y >> second.X >> second.Y;
        lines[i] = Line(first, second);

        for (int j = 0; j < i; ++j)
        {
            Point p = intersection(lines[i], lines[j]);
            if (!std::isnan(p.X) && !std::isnan(p.Y))
            {
                intersections[i].insert(p);
                intersections[j].insert(p);
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        if (intersections[i].size() >= 2)
        {
            for (auto j = intersections[i].begin(); j != --intersections[i].end();)
            {
                Point current = *j;
                Point next = *(++j);
                if (point_to_index.find(current) == point_to_index.end())
                {
                    point_to_index[current] = polygons.size();
                    index_to_point.push_back(current);
                    polygons.push_back(std::vector<int>());
                }
                if (point_to_index.find(next) == point_to_index.end())
                {
                    point_to_index[next] = polygons.size();
                    index_to_point.push_back(next);
                    polygons.push_back(std::vector<int>());
                }

                polygons[point_to_index[current]].push_back(point_to_index[next]);
                polygons[point_to_index[next]].push_back(point_to_index[current]);
            }
        }
    }

    for (int i = 0; i < polygons.size(); ++i)
    {
        std::sort(polygons[i].begin(), polygons[i].end(), [&index_to_point, i](int A, int B)
        {
            auto o = index_to_point[i];

            return o.angle(index_to_point[A]) > o.angle(index_to_point[B]); 
        });
    }

    std::vector<std::vector<bool>> visited(polygons.size());
    for (int i = 0; i < polygons.size(); ++i)
    {
        visited[i].resize(polygons[i].size(), false);
    }

    std::vector<float64_t> areas;
    for (int i = 0; i < polygons.size(); ++i)
    {
        for (int j = 0; j < polygons[i].size(); ++j)
        {
            if (!visited[i][j])
            {
                visited[i][j] = true;

                int current = polygons[i][j];
                int p = i;

                std::vector<int> current_points;
                while (true)
                {
                    current_points.push_back(current);
                    auto next = std::lower_bound(polygons[current].begin(), polygons[current].end(), p, [&index_to_point, current](int A, int B)
                    {
                        auto o = index_to_point[current];

                        return o.angle(index_to_point[A]) > o.angle(index_to_point[B]);            
                    });
                    if (next != polygons[current].end())
                    {
                        if (++next == polygons[current].end())
                        {
                            next = polygons[current].begin();
                        }
                        if (visited[current][next - polygons[current].begin()])
                        {
                            break;
                        }
                        visited[current][next - polygons[current].begin()] = true;
                        p = current;
                        current = *next;
                    }
                }

                if (current_points.size() < 2)
                {
                    continue;
                }

                std::vector<Point> polygon(current_points.size());
                for (int k = 0; k < current_points.size(); ++k)
                {
                    polygon[k] = index_to_point[current_points[k]];
                }

                float64_t S = area(polygon);

                if (S >= 1e-8)
                {
                    areas.push_back(S);
                }
            }
        }
    }

    std::sort(areas.begin(), areas.end());
    if (!areas.empty())
    {
        areas.pop_back();
    }

    std::cout << areas.size() << '\n';
    for (auto i : areas)
    {
        std::cout << i << '\n';
    }

    return 0;
}