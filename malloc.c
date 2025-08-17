#include <stddef.h>
#include <unistd.h>

typedef struct {
  size_t size;
  struct block_header_t *next;
  char free;
} block_header_t;

void *malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }

  size_t total_size = size + sizeof(block_header_t);

  block_header_t *header = sbrk(total_size);
  if (header == (void *)-1) {
    return NULL;
  }

  header->size = size;
  header->next = NULL;
  header->free = 0;
  return (void *)(header + 1);
}

void free(void *ptr) {
  if (!ptr) {
    return;
  }
  block_header_t *header = (block_header_t *)ptr - 1;
  header->free = 1;
}
