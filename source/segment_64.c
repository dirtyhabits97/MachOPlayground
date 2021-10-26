#include <mach-o/loader.h>
#include <mach/vm_prot.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "segment_64.h"
#include "utils.h"

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

static void format_section_type(uint8_t type, char *out) {
  if (type == S_REGULAR) {
    strcpy(out, "S_REGULAR");
  } else if (type == S_ZEROFILL) {
    strcpy(out, "S_ZEROFILL");
  } else if (type == S_CSTRING_LITERALS) {
    strcpy(out, "S_CSTRING_LITERALS");
  } else if (type == S_4BYTE_LITERALS) {
    strcpy(out, "S_4BYTE_LITERALS");
  } else if (type == S_8BYTE_LITERALS) {
    strcpy(out, "S_8BYTE_LITERALS");
  } else if (type == S_LITERAL_POINTERS) {
    strcpy(out, "S_LITERAL_POINTERS");
  } else if (type == S_NON_LAZY_SYMBOL_POINTERS) {
    strcpy(out, "S_NON_LAZY_SYMBOL_POINTERS");
  } else if (type == S_LAZY_SYMBOL_POINTERS) {
    strcpy(out, "S_LAZY_SYMBOL_POINTERS");
  } else if (type == S_LITERAL_POINTERS) {
    strcpy(out, "S_LITERAL_POINTERS");
  } else if (type == S_SYMBOL_STUBS) {
    strcpy(out, "S_SYMBOL_STUBS");
  } else if (type == S_MOD_INIT_FUNC_POINTERS) {
    strcpy(out, "S_MOD_INIT_FUNC_POINTERS");
  } else if (type == S_THREAD_LOCAL_ZEROFILL) {
    strcpy(out, "S_THREAD_LOCAL_ZEROFILL");
  } else if (type == S_THREAD_LOCAL_VARIABLES) {
    strcpy(out, "S_THREAD_LOCAL_VARIABLES");
  }else {
    sprintf(out, "OTHER(0x%x)", type);
  }
}

void parse_section(struct section_64 section) {
  char formatted_type[32];
  char formatted_size[16];
  char formatted_seg_sec[64];

  const uint8_t type = section.flags & SECTION_TYPE;

  format_section_type(type, formatted_type);
  sprintf(formatted_size, "(%lld)", section.size);
  sprintf(formatted_seg_sec, "(%s, %s)", section.segname, section.sectname);

  printf(
    "    0x%9llx-0x%9llx %-11s %-34s type: %s\n", 
    section.addr, section.addr + section.size,
    formatted_size,
    formatted_seg_sec,
    formatted_type
  );
}

void parse_segment(
    struct segment_command_64 *seg_cmd,
    FILE* obj_file
) {
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

  // section_64 is immediately after segment_command_64.
  void *sectionOffset = (void*)seg_cmd + sizeof(struct segment_command_64);

  for (int section = 0; section < seg_cmd->nsects; ++section) {
    int offset = sizeof(struct section_64) * section;
    struct section_64 *sect = load_bytes(
      obj_file,
      (long)sectionOffset + offset,
      sizeof(struct section_64)
    );
    parse_section(*sect);
  }
}
