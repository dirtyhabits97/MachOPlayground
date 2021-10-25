#include "segment_64.h"

#include <mach/vm_prot.h>
#include <stdio.h>
#include <string.h>

void formatted_protection(vm_prot_t mask, char * protection) {
  if (mask == VM_PROT_NONE) {
    strcat(protection, "NONE");
    return;
  }

  if (mask & VM_PROT_READ)
    strcat(protection, "READ");

  if (mask & VM_PROT_WRITE) {
    if (strcmp(protection, ""))
      strcat(protection, ",");
    strcat(protection, "WRITE");
  }

  if (mask & VM_PROT_EXECUTE) {
    if (strcmp(protection, ""))
      strcat(protection, ",");
    strcat(protection, "EXECUTE");
  }
}

void parse_segment(struct segment_command_64 *segment) {
  char formatted_file_size[16];
  char formatted_memory_size[16];
  char formatted_protection_str[20] = {'\0'};

  sprintf(formatted_file_size, "(%lld)", segment->filesize);
  sprintf(formatted_memory_size, "(%lld)", segment->vmsize);
  formatted_protection(segment->initprot, formatted_protection_str);

  printf(
    "%-20s nsects: %-4d cmdsize: %-6d segname: %-16s file:0x%08llx-0x%08llx %-11s vm: 0x%09llx-0x%09llx %-12s protection: %d/%d (%s)\n",
    "LC_SEGMENT_64",
    segment->nsects,
    segment->cmdsize,
    segment->segname,
    // file data
    segment->fileoff, segment->fileoff + segment->filesize, formatted_file_size,
    // memory data
    segment->vmaddr, segment->vmaddr + segment->vmsize, formatted_memory_size,
    // protection: NONE, READ, WRITE, EXECUTION
    segment->initprot, segment->maxprot, formatted_protection_str
  );
}
