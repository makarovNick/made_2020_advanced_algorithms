#include <iostream>
#include <string>
#include <algorithm>
#include <complex>
#include <vector>
#include <cmath>

#define M_PI 3.14159265358979323846 /* pi */

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
            std::reverse(res.begin(), res.end());
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

std::vector<int> DNA_similarity(const std::string& a, const std::string& b)
{
    int size = a.size();

    FFT fft(size * 2);
    std::string letters = "ACGT";
    std::vector<complex_t> d(fft.capacity());
    std::vector<complex_t> x(fft.capacity());

    for (char c : letters)
    {
        for (int i = 0; i < size; ++i)
        {
            x[i] = complex_t{ 1.0 * (a[i] == c), 1.0 * (b[size - i - 1] == c) };
        }
        auto fa = fft(x);
        for (int i = 0; i < fft.capacity(); ++i)
        {
            complex_t z1 = i ? fa[fft.capacity() - i] : fa[0];
            complex_t x(fa[i].real() + z1.real(), fa[i].imag() - z1.imag());
            complex_t y(fa[i].imag() + z1.imag(), z1.real() - fa[i].real());
            d[i] += x * y;
        }
    }
    auto z = fft(d, true);

    std::vector<int> res(2 * size);
    for (int i = 0; i < 2 * size; ++i)
    {
        res[i] = std::floor(z[i].real() / letters.size() + 0.5);
    }

    std::vector<int> sum(fft.capacity());
    for (int i = 0; i < size; ++i)
    {
        sum[i] = res[i + (size - 1)] + (i > 0 ? res[i - 1] : 0);
    }

    return sum;
}

int main()
{
    int n;
    std::string a, b;

    std::cin >> n;
    std::cin >> a >> b;

    auto res = DNA_similarity(a, b);

    int ind = std::max_element(res.begin(), res.end()) - res.begin();

    std::cout << res[ind] << ' ' << ind;

    return 0;
}