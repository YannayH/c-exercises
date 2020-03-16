#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int is_prime(int number) {
    if (number == 2) {
        return 1;
    } else if (number == 1 || number % 2 == 0) {
        return 0;
    }

    double number_sqrt = sqrt(number);

    for (int i = 3; i <= ceil(number_sqrt); i += 2) {
        if (number % i == 0) {
            return 0;
        }
    }

    return 1;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <number>", argv[0]);
        return 1;
    }

    int number = atoi(argv[1]);

    printf("The number %d is %sprime!", number, is_prime(number) ? "" : "not ");
    return 0;
}
