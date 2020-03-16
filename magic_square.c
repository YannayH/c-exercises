#include <stdio.h>
#include <stdlib.h>

struct Point {
    int row;
    int column;
};

void increment_point(struct Point* point, int size, int** square) {
    int next_row = point->row == 0 ? size - 1 : point->row - 1;±
    int next_column = (point->column + 1) % size;

    if (square[next_row][next_column] != 0) {
        next_row = point->row + 1;
        next_column = point->column;
    }

    point->row = next_row;
    point->column = next_column;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <size>", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);

    if (size % 2 != 1) {
        printf("The algorithm is currently only implemented for odd numbers.");
        return 1;
    }

    int** square = (int**) malloc(size * sizeof(int*));

    for (int i = 0; i < size; i ++) {
        square[i] = malloc(size * sizeof(int));
    }

    struct Point point = {0, (size - 1) / 2};

    for (int i = 0; i < size * size; i ++) {
        square[point.row][point.column] = i+1;
        increment_point(&point, size, square);
    }

    for (int i = 0; i < size; i ++) {
        for (int y = 0; y < size; y ++) {
            printf("%02d ", square[i][y]);
        }
        printf("\n");
        free(square[i]);
    }

    free(square);
    return 0;
}
