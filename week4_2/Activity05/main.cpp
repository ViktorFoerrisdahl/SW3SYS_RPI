#include <vector>
#include <functional>
#include <iostream>

std::vector<int> transform(const std::vector<int>& input, std::function<int(int)> f)
{
    std::vector<int> temp;
    for (auto i = 0; i < input.size(); i++)
    {
        temp.emplace_back(f(input[i]));
    }

    return temp;
}


int main ()
{
    std::vector<int> vector1 = {1,2,3,4,5,6,6};

    std::vector<int> vector1_transform = transform(vector1, [] (int x) {return x*23;});

    for (auto i : vector1_transform)
    {
        std::cout << i << ", ";
    }

    return 0;
}