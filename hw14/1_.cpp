#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <complex>
#include <vector>
#include <cmath>

# define M_PI		3.14159265358979323846	/* pi */

using complex_t = std::complex<double>;


class BigProd
{
public:
    BigProd()
    {
    }

    BigProd(const std::string& str)
        :   sign(str.size() > 0 && str[0] != '-')
    {
        digits.reserve(str.size() - !sign);
        for (size_t i = !sign; i < str.size(); ++i)
        {
            digits.push_back(str[i] - '0');
        }
        std::reverse(digits.begin(), digits.end());
    }

    friend std::ostream& operator<<(std::ostream& out, const BigProd& res)
    {
        const std::vector<int>& digits = res.digits;
        if ((digits.size() > 1 || (digits.size() && digits[0])) && !res.sign)
        {
            out << '-';
        }
        std::copy(digits.crbegin(), digits.crend(), std::ostream_iterator<int>(out));
        return out;
    }
    friend std::istream& operator>>(std::istream& in, BigProd& res)
    {
        std::string t;
        in >> t;
        res = BigProd(t);

        return in;
    }
    BigProd operator*(const BigProd& other)
    {
        BigProd res;
        res.sign = sign == other.sign;

        std::vector<complex_t> first(digits.begin(), digits.end());
        std::vector<complex_t> second(other.digits.begin(), other.digits.end());

        size_t power = 1;
        while(power <= std::max(digits.size(), other.digits.size()) * 2)
        {
            power <<= 1;
        }
        first.resize(power);
        second.resize(power);
        res.digits.resize(power);

        FFT(first, true);
        FFT(second, true);

        for (size_t i = 0; i < power; ++i)
        {
            first[i] *= second[i];
        }

        FFT(first, false, true);


        for (size_t i = 0; i < power; ++i)
        {
            res.digits[i] = static_cast<int>(first[i].real() * 0.5 + 0.5);
        }

        int shared = 0;
        for (size_t i = 0; i < power; ++i)
        {
            res.digits[i] += shared;
            shared = res.digits[i] / 10;
            res.digits[i] %= 10;
        }

        while (power > 1 && !res.digits[power - 1])
        {
            --power;
        }
        res.digits.resize(power);

        return res;
    }

private:
    bool FFT(std::vector<complex_t>& w, bool sign, bool reverse = false)
    {
        size_t size = w.size();
        if (size == 1)
        {
            return true;
        }

        std::vector<complex_t> left(size / 2);
        std::vector<complex_t> right(size / 2);

        for (size_t i = 0, j = 0; i < size; ++j, i += 2)
        {
            left[j] = w[i];
            right[j] = w[i + 1];
        }

        if (reverse)
        {
            if (!FFT(left, false, true))
            {
                for (auto& i : left)
                {
                    i /= 2;
                }
            }
            if (!FFT(right, false, true))
            {
                for (auto& i : right)
                {
                    i /= 2;
                }
            }
        }
        else
        {
            FFT(left, true);
            FFT(right, true);
        }

        double phi = (sign ? 1 : -1) * 2 * M_PI / size;
        complex_t dot = 1;
        complex_t nth(std::cos(phi), std::sin(phi));
        for (size_t i = 0; i < size / 2; ++i)
        {
            w[i] = left[i] + dot * right[i];
            w[i + (size >> 1)] = left[i] - dot * right[i];
            dot *= nth;
        }

        return false;
    }

    bool sign;
    std::vector<int> digits;
};

int main()
{
    BigProd a, b;
    std::cin >> a >>b;

    std::cout << a * b;

    return 0;
}