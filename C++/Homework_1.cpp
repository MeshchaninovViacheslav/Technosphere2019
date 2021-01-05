#include <iostream>
#include <chrono>
#include <cstdlib>
#include <vector>
#include "numbers.dat"
#include <math.h>

using namespace std;

bool IsPrime(int a) {
    if (a <= 1)
        return 0;
    int i = 2;
    int r = (int) sqrt(a);
    while (i <= r && a % i)
        i++;
    if (i <= r)
        return 0;
    return 1;
}


int main(int argc, char *argv[]) {
    if (!(argc & 1) || argc == 1)
        return -1;
    int n = Size;

    for (int k = 1; k < argc; k += 2) {
        int x = atoi(argv[k]);
        int y = atoi(argv[k + 1]);
        int i = 0, j = n - 1;
        while (Data[i] != x)
            i++;
        while (Data[j] != y)
            j--;
        int score = 0;
        while (i <= j) {
            if (IsPrime(Data[i]))
                score++;
            i++;
        }
        cout << score << '\n';
    }


    return 0;
}
