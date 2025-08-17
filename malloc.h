#pragma once
#include <stddef.h>

typedef struct block_header {
  size_t size;
  struct block_header *next;
  char free;
} block_header_t;

void *c_malloc(size_t size);
void c_free(void *ptr);
