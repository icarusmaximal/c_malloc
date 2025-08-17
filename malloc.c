#include "malloc.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#define CHUNK_SIZE 4096

static void *chunk_start = NULL;
static void *chunk_brk = NULL;
static void *current_brk = NULL;

static block_header_t *free_list = NULL;

void *c_malloc(size_t size) {

  size_t total_size = sizeof(block_header_t) + size;

  block_header_t *curr = free_list;
  block_header_t *prev = NULL;
  while (curr) {
    if (total_size <= curr->size + sizeof(block_header_t)) {

      size_t remaining_size = curr->size - size;

      if (remaining_size > sizeof(block_header_t)) {

        block_header_t *new_block =
            (block_header_t *)((char *)(curr + 1) + size);
        new_block->size = remaining_size - sizeof(block_header_t);
        new_block->next = curr->next;
        new_block->free = 1;

        curr->size = size;
        curr->next = NULL;
        curr->free = 0;

        if (!prev) {
          free_list = curr->next;
        } else {
          prev->next = curr->next;
        }
        return (void *)(curr + 1);
      }
      if (!prev) {
        free_list = curr->next;
      } else {
        prev->next = curr->next;
      }
      curr->next = NULL;
      curr->free = 0;
      return (void *)(curr + 1);
    }

    prev = curr;
    curr = curr->next;
  }

  if (!current_brk || current_brk + total_size > chunk_brk) {
    chunk_start = sbrk(CHUNK_SIZE);
    if (chunk_start == (void *)-1) {
      return NULL;
    }

    chunk_brk = chunk_start + CHUNK_SIZE;
    current_brk = chunk_start;
  }

  block_header_t *header = (block_header_t *)current_brk;

  header->size = size;
  header->next = NULL;
  header->free = 0;

  current_brk += total_size;
  return (void *)(header + 1);
}

void c_free(void *ptr) {
  if (!ptr) {
    return;
  }
  block_header_t *header = (block_header_t *)ptr - 1;
  header->free = 1;

  header->next = free_list;
  free_list = header;
}
