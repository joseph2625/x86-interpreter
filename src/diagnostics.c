#include "diagnostics.h"

#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static FILE *g_log_file = NULL;
static VerbosityLevel_t g_verbosity_level = SILENT;

static char *prefixes[] = { "INFO", "WARNING", "ERROR" };

void initialize_diagnostics( const char *log_file_path, VerbosityLevel_t verbosity_level ) {

  if( verbosity_level == SILENT )
    return;

  g_verbosity_level = verbosity_level;

  if( verbosity_level == PRINT_ALL_ACCESS_VIOLATIONS && log_file_path != NULL ) {
    fprintf( stderr, "[WARNING] File path for logging was supplied even though message logging is disabled; ignoring the path\n" );
    log_file_path = NULL;
  }

  if( verbosity_level == PRINT_AND_LOG_ALL_ACCESS_VIOLATIONS && log_file_path == NULL ) {
    fprintf( stderr, "[WARNING] No file path for message logging was supplied; logging will be disabled\n" );
    verbosity_level = PRINT_ALL_ACCESS_VIOLATIONS;
  }

  if( log_file_path ) {
#ifdef _MSC_VER
    errno_t err = fopen_s( &g_log_file, log_file_path, "w" );
    if( err != 0 ) {
#else
    g_log_file = fopen( log_file_path, "w" );
    if( g_log_file == 0 ) {
#endif
      fprintf( g_log_file = stderr, "[WARNING] Failed to open the log file for message logging; logging will be disabled\n" );
      verbosity_level = PRINT_ALL_ACCESS_VIOLATIONS;
    }
  }
  if( g_log_file == NULL )
    g_log_file = stderr;

}

void log_message( MessageLevel_t level, char *fmt, ... ) {
  if( g_verbosity_level == SILENT )
    return;

  if( level >= sizeof(prefixes)/sizeof(char *) )
    level = ERROR;

  va_list argptr;

  va_start( argptr, fmt );
#ifdef _MSC_VER
  size_t length_required = _vscprintf( fmt, argptr );
#else
  size_t length_required = vsnprintf( NULL, 0, fmt, argptr );
#endif

  char  *buffer = (char *)malloc( length_required + 1 );

#ifdef _MSC_VER
  _vsnprintf_s( buffer, length_required + 1, _TRUNCATE, fmt, argptr );
#else
  vsnprintf( buffer, length_required + 1, fmt, argptr );
#endif

  fprintf( g_log_file, "[%-7s] %s\n", prefixes[level], buffer );

  free( buffer );

  va_end(argptr);
}