#ifndef X86INTERPRETER_DIAGNOSTICS_H
#define X86INTERPRETER_DIAGNOSTICS_H

typedef enum MessageLevel {
  INFO = 0,
  WARNING = 1,
  ERROR = 2
} MessageLevel_t;

typedef enum VerbosityLevel {
  SILENT = 0,
  PRINT_ALL_ACCESS_VIOLATIONS = 1,
  PRINT_AND_LOG_ALL_ACCESS_VIOLATIONS = 2,
} VerbosityLevel_t;

void log_message( MessageLevel_t level, char *fmt, ... );
void initialize_diagnostics( const char *log_file_path, VerbosityLevel_t verbosity_level );

#endif //X86INTERPRETER_DIAGNOSTICS_H