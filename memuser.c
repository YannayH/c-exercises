#include <stdio.h>
#include <assert.h>
#include "memmanager.h"

int main() {
    void* arr = allocate_memory(20);  // Should not be allocated. Library not initialized.
    assert(arr == LIBRARY_NOT_INITIALIZED);

    initialize_manager(1024);
    void* small_arr = allocate_memory(20);  // Should be allocated properly.
    void* big_arr = allocate_memory(400);  // Should be allocated properly, 1024 - 20 = 1004 bytes left.
    void* big_arr2 = allocate_memory(400); // Should be allocated properly, 1024 - 420 = 604 bytes left.
    void* med_arr = allocate_memory(100); // Should be allocated properly, 1024 - 420 = 204 bytes left.
    void* unallocated_arr = allocate_memory(400); // Should not be allocated. Not enough memory. Only 104 bytes left

    assert(small_arr > 0);
    assert(big_arr > 0);
    assert(big_arr2 > 0);
    assert(med_arr > 0);
    assert(unallocated_arr == NOT_ENOUGH_MEMORY);

    free_memory(big_arr2);
    big_arr2 = allocate_memory(401); // Should NOT be allocated properly. There's enough memory. But it's fragmented.
    assert(big_arr2 == FRAGMENTED_MEMORY);

    defragment_memory();
    big_arr2 = allocate_memory(401); // Should be allocated properly. Since we de-fragmented the memory.
    assert(big_arr2 > 0);

    finalize_manager();
    return 0;
}