#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "memmanager.h"

#define MAXIMUM_ALLOCATIONS 10

void* LIBRARY_NOT_INITIALIZED = (void*) 0;
void* NOT_ENOUGH_MEMORY = (void*) -1;
void* FRAGMENTED_MEMORY = (void*) -2;


void* main_block = NULL;
size_t main_block_size = 0;
struct AllocatedMemory **allocated_memory_blocks = NULL;

void initialize_manager(size_t block_size) {
    main_block = malloc(block_size);
    main_block_size = block_size;
    allocated_memory_blocks = malloc(sizeof(struct AllocatedMemory*) * MAXIMUM_ALLOCATIONS);

    memset(main_block, 0, block_size);
    for (size_t i = 0; i < MAXIMUM_ALLOCATIONS; i ++) {
        allocated_memory_blocks[i] = NULL;
    }
}

void* allocate_memory(size_t size) {
    if (main_block == NULL) {
        return LIBRARY_NOT_INITIALIZED;
    }

    size_t total_allocation = 0;
    size_t start_address = 0;

    for (size_t j = 0; j < MAXIMUM_ALLOCATIONS; j ++) {
        if (allocated_memory_blocks[j] != 0) {
            if (start_address + size >= allocated_memory_blocks[j]->address) {
                start_address = allocated_memory_blocks[j]->address + allocated_memory_blocks[j]->size;
                total_allocation += allocated_memory_blocks[j]->size;
            }
        }
    }

    if (main_block_size - start_address < size) {
        if (main_block_size - total_allocation < size) {
            return NOT_ENOUGH_MEMORY;
        }
        return FRAGMENTED_MEMORY;
    }

    struct AllocatedMemory* allocated_memory_block = malloc(sizeof(struct AllocatedMemory));
    allocated_memory_block->address=start_address;
    allocated_memory_block->size=size;

    for (size_t j = 0; j < MAXIMUM_ALLOCATIONS; j ++) {
        if (allocated_memory_blocks[j] == 0) {
            allocated_memory_blocks[j] = allocated_memory_block;
            break;
        }
    }

    return (void*) main_block + allocated_memory_block->address;
}

int allocated_memory_comp(const void* first, const void* second) {
    struct AllocatedMemory* first_allocated = (struct AllocatedMemory*) first;
    struct AllocatedMemory* second_allocated = (struct AllocatedMemory*) second;

    if (*(int*) first != 0 && *(int*)second == 0) {
        return -1;
    } else if (*(int*)first == 0 && *(int*)second != 0) {
        return 1;
    } else if (*(int*)first == 0 && *(int*)second == 0) {
        return 0;
    } else {
        return (int) (first_allocated->address - second_allocated->address);
    }
}

void defragment_memory() {
    size_t free_address = 0;
    qsort(allocated_memory_blocks, MAXIMUM_ALLOCATIONS, sizeof (struct AllocatedMemory*), allocated_memory_comp);
    for (size_t j = 0; j < MAXIMUM_ALLOCATIONS; j ++) {
        if (allocated_memory_blocks[j] != 0) {
            if (free_address != allocated_memory_blocks[j]->address) {
                memcpy(main_block + free_address, main_block + allocated_memory_blocks[j]->address, allocated_memory_blocks[j]->size);
                allocated_memory_blocks[j]->address = free_address;
            }
            free_address += allocated_memory_blocks[j]->size;
        }
    }
}

int free_memory(void* address) {
    if (main_block == NULL) {
        return 1;
    }

    for (size_t j = 0; j < MAXIMUM_ALLOCATIONS; j ++) {
        if (allocated_memory_blocks[j] != 0) {
            if ((uint64_t) (address - main_block) == allocated_memory_blocks[j]->address) {
                free(allocated_memory_blocks[j]);
                allocated_memory_blocks[j] = 0;
                return 0;
            }
        }
    }
    return 1;
}

void finalize_manager() {
    for (size_t j = 0; j < MAXIMUM_ALLOCATIONS; j ++) {
        if (allocated_memory_blocks[j] != 0) {
            free(allocated_memory_blocks[j]);
            allocated_memory_blocks[j] = 0;
        }
    }

    free(allocated_memory_blocks);
    free(main_block);

    allocated_memory_blocks = NULL;
    main_block = NULL;
    main_block_size = 0;
}
