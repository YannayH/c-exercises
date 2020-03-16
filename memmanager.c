#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "memmanager.h"

#define MAXIMUM_ALLOCATIONS 1024 * 1024

static size_t BLOCK_SIZE = 2147483647;
static void* main_block = NULL;
static struct AllocatedMemory **allocated_memory_blocks = NULL;

__attribute__((constructor))
static void initialize_manager() {
    main_block = malloc(BLOCK_SIZE);
    allocated_memory_blocks = malloc(sizeof(struct AllocatedMemory*) * MAXIMUM_ALLOCATIONS);

    memset(main_block, 0, BLOCK_SIZE);
    for (size_t i = 0; i < MAXIMUM_ALLOCATIONS; i ++) {
        allocated_memory_blocks[i] = NULL;
    }
}

void* allocate_memory(size_t size) {
    size_t start_address = 0;

    for (size_t j = 0; j < MAXIMUM_ALLOCATIONS; j ++) {
        if (allocated_memory_blocks[j] != 0) {
            if (start_address + size >= allocated_memory_blocks[j]->address) {
                start_address = allocated_memory_blocks[j]->address + allocated_memory_blocks[j]->size;
            }
        }
    }

    if (BLOCK_SIZE - start_address < size) {
        errno = ENOMEM;
        return NULL;
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


__attribute__((destructor))
static void finalize_manager() {
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
}
