#include <iostream>

#include <iostream>
#include "sort.cpp"

int main() {
    int a[100];
    for (int i = 0; i < 100; ++i) a[i] = 100 - i;
    qs::sort(a, a + 100, [](const int& x, const int& y){ return x < y; });

    for (int i = 0; i < 10; ++i) std::cout << a[i] << ' '; // 0 1 2 3 4 5 6 7 8 9 ...
    std::cout << '\n';
}