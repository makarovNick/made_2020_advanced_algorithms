#include <iostream>
#include <string>
#include <algorithm>
#include <complex>
#include <vector>
#include <cmath>

#define M_PI 3.14159265358979323846 /* pi */
using complex_t = std::complex<double>;

const int MAX_LOG = 19;
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

int fuzzy_search(const std::string& haystack, std::string needle, int k)
{
    int size = haystack.size();
    std::reverse(needle.begin(), needle.end());
    FFT fft(haystack.size() + needle.size());
    std::string letters = "ACGT";

    // std::vector<bool> mask(size);
    std::vector<int> ans(fft.capacity(), 0);
    for (int p = 0; p < 4; p++)
    {
        std::vector<complex_t> a(fft.capacity(), 0);
        std::vector<complex_t> b(fft.capacity(), 0);
        int pos = -2e9;
        for (int i = 0; i < haystack.size(); i++)
        {
            if (haystack[i] == letters[p])
            {
                pos = i;
            }
            if (i - pos <= k)
            {
                a[i] = 1;
            }
        }
        pos = 2e9;
        for (int i = haystack.size() - 1; i >= 0; i--)
        {
            if (haystack[i] == letters[p])
            {
                pos = i;
            }
            if (pos - i <= k)
            {
                a[i] = 1;
            }
        }
        for (int i = 0; i < needle.size(); i++)
        {
            if (needle[i] == letters[p])
            {
                b[i] = 1;
            }
        }
        a = fft(a);
        b = fft(b);
        for (int i = 0; i < fft.capacity(); i++)
        {
            a[i] = a[i] * b[i];
        }
        auto r = fft(a, true);

        for (int i = 0; i < fft.capacity(); i++)
        {
            ans[i] += std::floor(r[i].real() + 0.5);
        }
    }
    int res = 0;
    for (int i = 0; i < fft.capacity(); i++)
    {
        if (ans[i] == needle.size())
        {
            res++;
        }
    }
    return res;
}

int main()
{
    int S, T, k;
    std::cin >> S >> T >> k;
    std::string haystack, needle;
    std::cin >> haystack >> needle;

    auto res = fuzzy_search(haystack, needle, k);

    std::cout << res;

    return 0;
}