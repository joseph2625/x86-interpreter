#ifndef X86INTERPRETER_RAW_H
#define X86INTERPRETER_RAW_H

#include "image.h"

bool process_raw( Image_t *image, uint32_t base_address, uint32_t entry_point );

#endif //X86INTERPRETER_RAW_H