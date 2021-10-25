#include "segment_64.h"
#include <stdio.h>

void parse_segment(struct segment_command_64 *segment) {
  printf("segname: %s\n", segment->segname);
}
