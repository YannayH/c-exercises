#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>

#include "memmanager.h"

#define MAXIMUM_ALLOCATIONS 1024 * 1024

static size_t BLOCK_SIZE = 2147483647;
static void* main_block = NULL;
static struct AllocatedMemory **allocated_memory_blocks = NULL;


__attribute__((constructor))
static void initialize_manager() {
    main_block = mmap(NULL, BLOCK_SIZE, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    allocated_memory_blocks = mmap(NULL, sizeof(struct AllocatedMemory*) * MAXIMUM_ALLOCATIONS, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

    memset(main_block, 0, BLOCK_SIZE);
    for (size_t i = 0; i < MAXIMUM_ALLOCATIONS; i ++) {
        allocated_memory_blocks[i] = NULL;
    }
}

void* __wrap_malloc(size_t size) {
    printf("mallocing %d\n", size);
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


    struct AllocatedMemory* allocated_memory_block = mmap(NULL, sizeof(struct AllocatedMemory), PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);;
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

int __wrap_free(void* address) {
    if (main_block == NULL) {
        return 1;
    }

    for (size_t j = 0; j < MAXIMUM_ALLOCATIONS; j ++) {
        if (allocated_memory_blocks[j] != 0) {
            if ((uint64_t) (address - main_block) == allocated_memory_blocks[j]->address) {
                munmap(allocated_memory_blocks[j], sizeof(struct AllocatedMemory));
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
            munmap(allocated_memory_blocks[j], sizeof(struct AllocatedMemory));
            allocated_memory_blocks[j] = 0;
        }
    }

    munmap(allocated_memory_blocks, sizeof(struct AllocatedMemory*) * MAXIMUM_ALLOCATIONS);
    munmap(main_block, BLOCK_SIZE);

    allocated_memory_blocks = NULL;
    main_block = NULL;
}
