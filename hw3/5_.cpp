#include <iostream>
#include <vector>

int64_t gcd_extended(int64_t a, int64_t b, int64_t& x, int64_t& y)
{
    if (a == 0)
    {
        x = 0;
        y = 1;
        return b;
    }
    int64_t x1 = 0, y1 = 0;
    int64_t d = gcd_extended(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

int64_t mod_inverse(int64_t a, int64_t m)
{
    int64_t x, y;
    int64_t res = gcd_extended(a, m, x, y);
    return (x + m) % m;
}

int64_t CRT(const std::vector<int64_t>& A, const std::vector<int64_t>& R)
{
    std::vector<std::vector<int64_t>> inversed(A.size(), std::vector<int64_t>(A.size()));
    std::vector<int64_t> x(A.size());

    int64_t result = 0;
    int64_t M = 1;

    for (int i = 0; i < inversed.size(); ++i)
    {
        for (int j = i + 1; j < R.size(); ++j)
        {
            inversed[i][j] = mod_inverse(R[i], R[j]);
        }
    }

    for (int i = 0; i < A.size(); ++i)
    {
        x[i] = A[i];

        for (int j = 0; j < i; ++j)
        {
            x[i] = (x[i] - x[j]) * inversed[j][i];
            x[i] = x[i] % R[i];

            if (x[i] < 0)
            {
                x[i] += R[i];
            }
        }
        result += M * x[i];

        M *= R[i];
    }

    return result;
}

int main()
{
    int N;
    std::cin >> N;

    for (int i = 0; i < N; ++i)
    {
        std::vector<int64_t> a(2);
        std::vector<int64_t> r(2);
        std::cin >> a[1] >> a[0] >> r[1] >> r[0];

        std::cout << CRT(a, r) << std::endl;
    }

    return 0;
}