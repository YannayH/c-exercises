#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "memmanager.h"

int main() {
    void* small_arr = malloc(20);  // Should be allocated properly.
    void* big_arr = malloc(400);  // Should be allocated properly, 1024 - 20 = 1004 bytes left.
    void* big_arr2 = malloc(400); // Should be allocated properly, 1024 - 420 = 604 bytes left.
    void* med_arr = malloc(100); // Should be allocated properly, 1024 - 420 = 204 bytes left.

    assert(small_arr > 0);
    assert(big_arr > 0);
    assert(big_arr2 > 0);
    assert(med_arr > 0);

    return 0;
}