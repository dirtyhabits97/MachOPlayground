#ifndef SEGMENT_64_H
#define SEGMENT_64_H

#include <mach-o/loader.h>
#include <stdio.h>

void parse_segment(struct segment_command_64 *segment, FILE *obj_file);

#endif  // SOURCE_SEGMENT_64_H_
