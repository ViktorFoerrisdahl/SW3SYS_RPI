#include <iostream>

int add(int a, int b)
{
    return a + b;
}

// Write this function:
int apply(int (*op)(int, int), int x, int y)
{
    return op(x,y);
}

int main()
{
    int n1 = 1;
    int n2 = 2;
    int n3 = apply(add,n1,n2);

    std::cout << "nyt tal: " << apply(add,n3,n1) << std::endl; 

    return 0;
}