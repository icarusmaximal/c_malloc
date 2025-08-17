#include "../malloc.h"
#include <stdio.h>
#include <unistd.h>

int main() {
  printf("Initial program break: %p\n", sbrk(0));

  int *a = c_malloc(5 * sizeof(int));
  printf("After malloc a: %p\n", a);

  int *b = c_malloc(10 * sizeof(int));
  printf("After malloc b: %p\n", b);
  printf("Program break now: %p\n", sbrk(0));

  int *c = c_malloc(1025 * sizeof(int)); // for another sbrk call in c_malloc
  printf("After malloc c: %p\n", c);
  printf("Program break now: %p\n", sbrk(0));

  c_free(a);
  c_free(b);
  c_free(c);

  // Check headers manually
  block_header_t *header_a = (block_header_t *)a - 1;
  block_header_t *header_b = (block_header_t *)b - 1;
  block_header_t *header_c = (block_header_t *)c - 1;

  printf("Header a: size=%zu, free=%d\n", header_a->size, header_a->free);
  printf("Header b: size=%zu, free=%d\n", header_b->size, header_b->free);
  printf("Header c: size=%zu, free=%d\n", header_b->size, header_b->free);

  return 0;
}
