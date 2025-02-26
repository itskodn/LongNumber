#include "./hm/hm.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <chrono>

int main(int argc, char *argv[])
{

    auto start_time = std::chrono::steady_clock::now();

    int precision = 4 * std::stoi(argv[1]);

    LongNumber pi(0, precision);
    LongNumber n0{1, precision};
    LongNumber n(16, precision);

    LongNumber a0{4, precision};
    LongNumber a(1, precision);
    LongNumber b0{2, precision};
    LongNumber b(4, precision);
    LongNumber c0{1, precision};
    LongNumber d0{1, precision};

    LongNumber c(5, precision);
    LongNumber d(6, precision);
    LongNumber e(8, precision);

    for (int k = 0; k < precision; ++k)
    {
        pi = pi + n0 * (a0 / a - b0 / b - c0 / c - d0 / d);
        n0 = n0 / n;
        a = a + e;
        b = b + e;
        c = c + e;
        d = d + e;
    }

    std::cout << pi << '\n';


    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "time = " << duration.count() << "\n";

    return 0;
}

