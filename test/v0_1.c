
#include "../malloc.h"
#include <stdio.h>
#include <string.h>

void *c_malloc(size_t size);
void c_free(void *ptr);

int main() {
  printf("== Allocator Free List Test ==\n");

  // allocate two blocks
  char *a = (char *)c_malloc(16);
  char *b = (char *)c_malloc(32);

  printf("Allocated a=%p, b=%p\n", (void *)a, (void *)b);

  // write to them to ensure memory works
  strcpy(a, "Hello");
  strcpy(b, "World");
  printf("a='%s', b='%s'\n", a, b);

  // free one block
  c_free(a);
  printf("Freed a=%p\n", (void *)a);

  // allocate again (should reuse the freed block if free list works)
  char *c = (char *)c_malloc(16);
  printf("Allocated c=%p (should reuse a)\n", (void *)c);

  // check if it really reused
  if (c == a) {
    printf("Free list reused the freed block!\n");
  } else {
    printf("Free list did not reuse as expected.\n");
  }

  return 0;
}
