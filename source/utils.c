#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void *load_bytes(FILE *obj_file, int offset, int size) {
  void *buf = calloc(1, size);
  fseek(obj_file, offset, SEEK_SET);
  fread(buf, size, 1, obj_file);
  return buf;
}
