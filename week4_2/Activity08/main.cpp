#include <vector>
#include <functional>
#include <iostream>
#include <algorithm>
#include <numeric>

int main () 
{

    std::vector<int> tal = {1,2,3,5,6,7,13,20};
    std::vector<int> tal_even;

    std::transform(tal.begin(), tal.end(), std::back_inserter(tal_even), [] (int x) {if (x % 2 == 0) {return x * 2;} else {return x;}});

    for (auto i : tal_even)
    {
        std::cout << i << ", ";
    }

    std::cout << std::endl;

    int res = std::accumulate(tal.begin(),tal.end(),0);

    std::cout << res;

    return 0;
}