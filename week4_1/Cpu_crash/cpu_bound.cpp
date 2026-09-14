#include <iostream>
#include <math.h>

int main() {
    int counter = 0;
    for (auto i = 3; i < 1000000000; i++)
    {
        for (auto j = 0; j < 10000000000; j++)
        {
            std::cout << i << " " << j << std::endl;
            int o = i * j;
            int a = o / i;
            std::cout << a << std::endl;
            int s = sqrt(a);
            int p = s * o * a;
        }
    }
    return 0;
}