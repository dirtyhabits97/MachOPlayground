#include "segment_64.h"
#include <stdio.h>

void parse_segment(struct segment_command_64 *segment) {
  char formatted_file_size[16];
  char formatted_memory_size[16];

  sprintf(formatted_file_size, "(%lld)", segment->filesize);
  sprintf(formatted_memory_size, "(%lld)", segment->vmsize);

  printf(
    "%-20s cmdsize: %-6d segname: %-12s    file:0x%08llx-0x%08llx %-11s vm: 0x%09llx-0x%09llx %-12s\n",
    "LC_SEGMENT_64",
    segment->cmdsize,
    segment->segname,
    // file data
    segment->fileoff, segment->fileoff + segment->filesize, formatted_file_size,
    // memory data
    segment->vmaddr, segment->vmaddr + segment->vmsize, formatted_memory_size
  );
}
