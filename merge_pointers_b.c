#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void assert_sorted_string(char* str, int len) {
    for (int i = 1; i < len; i ++) {
        assert(str[i] >= str[i-1]);
    }
}

char* merge_strings(char first_string[], char second_string[]) {
    int first_str_len = strlen(first_string);
    int second_str_len = strlen(second_string);

    assert(first_str_len > 0 || second_str_len > 0);
    assert_sorted_string(first_string, first_str_len);
    assert_sorted_string(second_string, second_str_len);

    char* combined_string = malloc(sizeof(char) * (first_str_len + second_str_len));
    char* combined_str_ptr = combined_string;

    while (*first_string != 0 && *second_string != 0) {
        if (*first_string < *second_string) {
            *combined_str_ptr++ = *first_string++;
        } else {
            *combined_str_ptr++ = *second_string++;
        }
    }

    strcpy(combined_str_ptr, first_string);
    combined_str_ptr += strlen(first_string);
    strcpy(combined_str_ptr, second_string);
    return combined_string;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <first string> <second string>\n", argv[0]);
        return 1;
    }

    char* merged_string = merge_strings(argv[1], argv[2]);

    printf("First string: %s\nSecond string: %s\nMerged string: %s\n", argv[1], argv[2], merged_string);
    free(merged_string);
    return 0;
}