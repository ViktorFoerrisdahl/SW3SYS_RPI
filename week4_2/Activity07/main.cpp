#include <vector>
#include <functional>
#include <iostream>



std::function<int(int)> compose(std::function<int(int,int)> f, std::function<int(int,int,int)> g)
{
    return [f,g] (int x) {int temp = g(x,x,x); return f(temp,temp);};

}

int main ()
{

    auto add2 = [] (int x, int y) {return x + y;};

    auto multiply3 = [] (int x, int y, int z) {return x * y * z;};

    auto new_funktion = compose(add2,multiply3);

    int temp = 5;

    std::cout << new_funktion(temp);


    return 0;
}