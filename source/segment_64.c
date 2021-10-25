#include "segment_64.h"

#include <mach/vm_prot.h>
#include <stdio.h>
#include <string.h>

static void format_protection(vm_prot_t mask, char * protection) {
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

void parse_segment(struct segment_command_64 *seg_cmd) {
  char formatted_file_size[16];
  char formatted_memory_size[16];
  char formatted_protection[20] = {'\0'};

  sprintf(formatted_file_size, "(%lld)", seg_cmd->filesize);
  sprintf(formatted_memory_size, "(%lld)", seg_cmd->vmsize);
  format_protection(seg_cmd->initprot, formatted_protection);

  printf(
    "%-20s nsects: %-4d cmdsize: %-6d segname: %-16s file:0x%08llx-0x%08llx %-11s vm: 0x%09llx-0x%09llx %-12s protection: %d/%d (%s)\n",
    "LC_SEGMENT_64",
    seg_cmd->nsects,
    seg_cmd->cmdsize,
    seg_cmd->segname,
    // file data
    seg_cmd->fileoff, seg_cmd->fileoff + seg_cmd->filesize, formatted_file_size,
    // memory data
    seg_cmd->vmaddr, seg_cmd->vmaddr + seg_cmd->vmsize, formatted_memory_size,
    // protection: NONE, READ, WRITE, EXECUTION
    seg_cmd->initprot, seg_cmd->maxprot, formatted_protection
  );
}
