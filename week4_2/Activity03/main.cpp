#include <algorithm>
#include <iostream>
#include <vector>

int main () 
{

    for(int i = 1; i < 6; i++) { std::cout << [] (int x) {return x*x;} (i) << ", ";}

    std::cout << std::endl;

    for (int i = 1; i < 6; i++)
    {
        std::cout << i*i << ", ";
    }

    return 0;
}