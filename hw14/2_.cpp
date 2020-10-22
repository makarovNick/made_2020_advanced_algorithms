#include <iostream>
#include <string>
#include <algorithm>
#include <complex>
#include <vector>
#include <cmath>

# define M_PI		3.14159265358979323846	/* pi */

using complex_t = std::complex<double>;

const int MAX_LOG = 18;
constexpr const int MAX_LENGTH = 1 << MAX_LOG;

class FFT
{
public:
    FFT(int n)
    {
        size = 1;
        size_log = 0;
        while (size < n)
        {
            size <<= 1;
            size_log++;
        }
        inv = 1.0 / size;

        for (int i = 0; i < size; ++i)
        {
            indexes[i] = (indexes[i >> 1] >> 1) + ((i & 1) << (size_log - 1));
        }
        for (int k = 1; k < size; k *= 2)
        {
            complex_t tmp(std::cos(M_PI / k), std::sin(M_PI / k));
            W[k] = { 1, 0 };
            for (int i = 1; i < k; i++)
            {
                W[k + i] = (i & 1) ? W[(k + i) >> 1] * tmp : W[(k + i) >> 1];
            }
        }
    }
    std::vector<complex_t> operator()(const std::vector<complex_t>& w, bool reverse = false)
    {
        std::vector<complex_t> res(size);
        for (int i = 0; i < size; ++i)
        {
            res[indexes[i]] = w[i];
        }
        for (int k = 1; k < size; k *= 2)
        {
            for (int i = 0; i < size; i += 2 * k)
            {
                for (int j = 0; j < k; ++j)
                {
                    complex_t tmp = W[k + j] * res[i + j + k];
                    res[i + j + k] = res[i + j] - tmp;
                    res[i + j] = res[i + j] + tmp;
                }
            }
        }
        if (reverse)
        {
            std::reverse(res.begin() + 1, res.end());
            for (int i = 0; i < size; ++i)
            {
                res[i] *= inv;
            }
        }

        return res;
    }
    int capacity() const
    {
        return size;
    }

private:
    int size;
    int size_log;
    double inv;

    complex_t W[MAX_LENGTH];
    int indexes[MAX_LENGTH];
};

std::vector<int> n_poitions(std::vector<int>& w)
{
    std::vector<int> c(2 * w.size());
    FFT fft(c.size());
    std::vector<complex_t> tmp(MAX_LENGTH);

    for (int i = 0; i < fft.capacity(); ++i)
    {
        tmp[i] = { (i < w.size() ? w[i] : 0.0), 0.0 };
    }
    auto fa = fft(tmp);
    for (int i = 0; i < fft.capacity(); ++i)
    {
        fa[i] *= fa[i];
    }
    tmp = fft(fa, true);

    for (int i = 0; i < c.size(); ++i)
    {
        c[i] = std::floor(tmp[i].real() + 0.5);
    }

    return c;
}

int main()
{
    std::string s;
    std::cin >> s;

    std::vector<int> w(s.size());
    for (int i = 0; i < w.size(); ++i)
    {
        w[i] = s[i] - '0';
    }

    auto a2 = n_poitions(w);
    int64_t ans = 0;
    for (int i = 0; i < a2.size(); ++i)
    {
        if (s[i] == '1')
        {
            ans += a2[2 * i] - 1;
        }
    }

    std::cout << ans / 2;

    return 0;
}