#ifndef C_EXERCISE_MEMMANAGER_H
#define C_EXERCISE_MEMMANAGER_H

#include <inttypes.h>

struct AllocatedMemory {
    uint64_t address;
    uint64_t size;
};

extern void* LIBRARY_NOT_INITIALIZED;
extern void* NOT_ENOUGH_MEMORY;
extern void* FRAGMENTED_MEMORY;

void initialize_manager(size_t block_size);
void* allocate_memory(size_t size);
void defragment_memory();
int free_memory(void* address);
void finalize_manager();


#endif //C_EXERCISE_MEMMANAGER_H
