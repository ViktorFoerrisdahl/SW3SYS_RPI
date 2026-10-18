#include <iostream>

void forEach(int *arr, int size, int (*func)(int))
{
    for (auto i = 0; i < size; i++)
    {
        arr[i] = func(arr[i]);
    }
}

int sqare(int a)
{
    return a * a;
}

int main()
{
    int array[5];

    for (auto i = 0; i < 5; i++)
    {
        array[i] = i;
    }

    std::cout << "vores array: ";

    for (auto i : array)
    {
        std::cout << i << ", ";
    }

    std::cout << std::endl;

    forEach(array, sizeof(array), sqare);

    std::cout << "ny array: ";

    for (auto i : array)
    {
        std::cout << i << ", ";
    }

    return 0;
}