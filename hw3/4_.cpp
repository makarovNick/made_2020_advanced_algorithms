#include <iostream>

int64_t gcd_extended(int64_t a, int64_t b, int64_t &x, int64_t &y)  
{  
    if (a == 0)  
    {  
        x = 0;  
        y = 1;
 
        return b;  
    }  
  
    int64_t x1;   
    int64_t gcd = gcd_extended(b % a, a, x1, y);  
   
    x = y - (b / a) * x1;  
    y = x1;  
  
    return gcd;  
}  

int main()
{
    int64_t A, B, C;
    std::cin >> A >> B >> C;

    int64_t x, y;
    int64_t gcd = gcd_extended(A, B, x, y);

    if (C % gcd == 0)
    {
        std::cout << - x * C / gcd << ' ' << - y * C / gcd;
    }
    else
    {
        std::cout << -1;
    }

    return 0;
}