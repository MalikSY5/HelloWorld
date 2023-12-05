#include <stdio.h>
#include <stdlib.h>

// Define block and node structures
typedef struct block {
    int pid;         // process id
    int start;       // start of the memory partition
    int end;         // end of the memory partition
} block_t;

typedef struct node {
    block_t *blk;     // pointer to the block
    struct node *next; // pointer to next node
} node_t;

// Function prototypes
void list_add_to_back(node_t **head, block_t *blk);
void list_add_ascending_by_address(node_t **head, block_t *blk);
void list_add_ascending_by_blocksize(node_t **head, block_t *blk);
void list_add_descending_by_blocksize(node_t **head, block_t *blk);
int list_is_in_by_pid(node_t *head, int pid);
void list_coalesce_nodes(node_t **head);
void allocate_memory(node_t **allocated, node_t **free_list, int pid, int request_size);
void deallocate_memory(node_t **allocated, node_t **free_list, int pid);
void coalesce_memory(node_t **free_list);

int main() {
    // Read initial partition size
    int initial_size;
    scanf("%d", &initial_size);

    // Create a linked list (FREE_LIST) with the initial partition size
    block_t *initial_block = malloc(sizeof(block_t));
    initial_block->pid = 0;
    initial_block->start = 0;
    initial_block->end = initial_size;

    node_t *free_list = malloc(sizeof(node_t));
    free_list->blk = initial_block;
    free_list->next = NULL;

    // Other variables
    node_t *allocated = NULL;
    int pid, request_size;

    // Read and process input
    while (1) {
        scanf("%d", &pid);
        if (pid == -99999) {
            int dummy;
            scanf("%d", &dummy);
            coalesce_memory(&free_list);
        } else if (pid < 0) {
            scanf("%d", &request_size);
            if (request_size == 0) {
                deallocate_memory(&allocated, &free_list, -pid);
            }
        } else {
            scanf("%d", &request_size);
            allocate_memory(&allocated, &free_list, pid, request_size);
        }
    }

    return 0;
}

// Function to allocate memory based on allocation policy
void allocate_memory(node_t **allocated, node_t **free_list, int pid, int request_size) {
    // Iterate through free list to find a suitable block
    node_t *current = *free_list;
    node_t *prev = NULL;
    while (current != NULL) {
        int block_size = current->blk->end - current->blk->start;
        if (block_size >= request_size) {
            // Allocate memory
            block_t *allocated_block = malloc(sizeof(block_t));
            allocated_block->pid = pid;
            allocated_block->start = current->blk->start;
            allocated_block->end = current->blk->start + request_size;

            // Update free list
            current->blk->start += request_size;
            if (current->blk->start >= current->blk->end) {
                // Remove empty block
                if (prev != NULL) {
                    prev->next = current->next;
                } else {
                    *free_list = current->next;
                }
                free(current->blk);
                free(current);
            }

            // Insert into allocated list
            list_add_ascending_by_address(allocated, allocated_block);
            return;
        }
        prev = current;
        current = current->next;
    }

    // Not enough memory
    printf("Error: Not Enough Memory\n");
}

// Function to deallocate memory based on allocation policy
void deallocate_memory(node_t **allocated, node_t **free_list, int pid) {
    // Search for the block in the allocated list
    node_t *current = *allocated;
    node_t *prev = NULL;
    while (current != NULL) {
        if (current->blk->pid == pid) {
            // Deallocate memory
            block_t *deallocated_block = current->blk;

            // Remove from allocated list
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                *allocated = current->next;
            }
            free(current);

            // Insert into free list based on allocation policy
            list_add_ascending_by_address(free_list, deallocated_block);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Function to coalesce memory blocks in the free list
void coalesce_memory(node_t **free_list) {
    list_coalesce_nodes(free_list);
}

