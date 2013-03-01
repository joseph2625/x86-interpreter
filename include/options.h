#ifndef X86INTERPRETER_OPTIONS_H
#define X86INTERPRETER_OPTIONS_H

#ifndef _WIN32
#include <stdbool.h>
#endif
#include "image.h"
#include "runtime.h"
#include "diagnostics.h"

typedef struct InterpreterOptions {
  char *executable_file_path;
  char *log_file_path;
  ExecutableType_t executable_type_override;
  char *initial_thread_context_file_path;
  int executable_argc;
  char **executable_argv;
  VerbosityLevel_t verbosity_level;
  uint32_t base_address;
  uint32_t entry_point;
} InterpreterOptions_t;

bool parse_options( int argc, char *argv[], InterpreterOptions_t *options );
void print_usage( char *name );

#endif X86INTERPRETER_OPTIONS_H