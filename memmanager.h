#ifndef C_EXERCISE_MEMMANAGER_H
#define C_EXERCISE_MEMMANAGER_H

#include <inttypes.h>

struct AllocatedMemory {
    uint64_t address;
    uint64_t size;
};

void* allocate_memory(size_t size);
int free_memory(void* address);


#endif //C_EXERCISE_MEMMANAGER_H
