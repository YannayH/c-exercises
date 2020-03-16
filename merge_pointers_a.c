#include <stdio.h>
#include <string.h>
#include <assert.h>

void merge_strings(char first_string[], char second_string[], char combined_string[]) {
    int first_string_length = strlen(first_string);
    for (int i = 1; i < first_string_length; i ++) {
        assert(first_string[i] >= first_string[i-1]);
    }

    int second_string_length = strlen(second_string);
    for (int i = 1; i < second_string_length; i ++) {
        assert(second_string[i] >= second_string[i-1]);
    }

    assert(first_string_length > 0 || second_string_length > 0);

    int first_string_index = 0, second_string_index = 0;
    while (first_string_index < first_string_length && second_string_index < second_string_length) {
        if (first_string[first_string_index] < second_string[second_string_index]) {
            combined_string[first_string_index + second_string_index] = first_string[first_string_index];
            first_string_index ++;
        } else {
            combined_string[first_string_index + second_string_index] = second_string[second_string_index];
            second_string_index ++;
        }
    }

    strcpy(combined_string + first_string_index + second_string_index, first_string + first_string_index);
    strcpy(combined_string + first_string_length + second_string_index, second_string + second_string_index);
}

int main() {
    char a[] = "abc";
    char b[] = "av";
    char c[10] = "";

    merge_strings(a,b,c);

    printf("First string: %s\nSecond string: %s\nMerged string: %s\n", a, b, c);

    return 0;
}