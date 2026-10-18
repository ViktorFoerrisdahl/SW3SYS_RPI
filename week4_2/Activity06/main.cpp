#include <vector>
#include <functional>
#include <iostream>

std::vector<int> filter(const std::vector<int>& input, std::function<bool(int)> pred)
{
    std::vector<int> temp;
    for (auto i = 0; i < input.size(); i++)
    {
        if (pred(input[i]))
        {
            temp.emplace_back(input[i]);
        }
    }

    return temp;
}

int main ()
{

    std::vector<int> vector1 = {1,2,3,4,5,6,6,9,9,10,10,12,1000};

    std::vector<int> vector1_transform = filter(vector1, [] (int x) {return x > 10;});

    for (auto i : vector1_transform)
    {
        std::cout << i << ", ";
    }

    return 0;
}