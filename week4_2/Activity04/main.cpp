#include <vector>
#include <iostream>

int main () {

    const double multiplier{10.0};
    const std::vector<double> numbers{1, 2, 3, 4, 5, 6, 7};

    // Lavet hvor den griber begge varibler
    for(int i = 0; i < numbers.size(); i++) { std::cout << [x = multiplier, i = numbers[i]] () {return x*i;} () << ", ";}

    std::cout << std::endl;

    // Lavet hvor den griber kun multiplier variblen
    for(int i = 0; i < numbers.size(); i++) { std::cout << [multiplier] (double y) {return multiplier*y;} (numbers[i]) << ", ";}


    return 0;
}