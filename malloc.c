#include "malloc.h"
#include <stddef.h>
#include <unistd.h>

#define CHUNK_SIZE 4096

static void *chunk_start = NULL;
static void *chunk_brk = NULL;
static void *current_brk = NULL;

void *c_malloc(size_t size) {

  size_t total_size = sizeof(block_header_t) + size;
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
}
