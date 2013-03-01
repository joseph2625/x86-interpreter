#include "options.h"
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include "XGetopt.h"
#else
#include <getopt.h>
#define _stricmp strcmp
#endif

static VerbosityLevel_t level_mapping[] = { SILENT, PRINT_ALL_ACCESS_VIOLATIONS, PRINT_AND_LOG_ALL_ACCESS_VIOLATIONS };

bool parse_options( int argc, char *argv[], InterpreterOptions_t *options ){
  int c;
  unsigned int level = 1;
  uint32_t base_address = 0x400000;
  uint32_t entry_point = 0x400000;

  options->executable_type_override = INVALID;

  while ((c = getopt( argc, argv, "v:f:t:e:b:c:" ) ) != EOF) {
    switch( c ){
    case 'v':
      level = strtoul( optarg, NULL, 0 );
      if( level >= sizeof(level_mapping)/sizeof(VerbosityLevel_t) ) {
        printf( "[ERROR  ] Invalid verbosity level: %s\n", optarg );
        return false;
      }
      break;
    case 'f':
      options->log_file_path = optarg;
      break;
    case 'e':
      entry_point = strtoul( optarg, NULL, 0 );
      break;
    case 'b':
      base_address = strtoul( optarg, NULL, 0 );
      break;
    case 't':
      if( _stricmp( optarg, "pe" ) == 0 )
        options->executable_type_override = PE;
      else if( _stricmp( optarg, "elf" ) == 0 )
        options->executable_type_override = ELF;
      else if( _stricmp( optarg, "raw" ) == 0 )
        options->executable_type_override = RAW;
      else if( _stricmp( optarg, "txt" ) == 0 )
        options->executable_type_override = TXT;
      else
        printf( "[ERROR  ] Invalid executable type: %s\n", optarg );
      break;
    case 'c':
      options->initial_thread_context_file_path = optarg;
      break;
    case '?':
      printf( "[ERROR  ] Illegal option: %s\n", argv[optind-1] );
      return false;
      break;
    }
  }

  if( optind >= argc ) {
    printf( "[  ERROR] Executable name must be specified\n" );
    return false;
  }
  options->executable_file_path = argv[optind];

  options->executable_argv = (char **)malloc( argc * sizeof(char *) );
  options->executable_argv[0] = argv[0];

  int i = optind + 1, j = 1;

  while( i < argc ) {
    options->executable_argv[j] = argv[i];
    i++;
    j++;
  }
  options->executable_argc = j;
  
  options->verbosity_level = level_mapping[level];

  options->base_address = base_address;

  options->entry_point = entry_point;

  return true;
}

void print_usage(char *name){
  printf( "x86 Process VM Interpreter (c) Joseph Hong\n"
    "Provides Linux (x86) runtime environment via interpretation\n" );

  printf( "Usage: %s <options> filename [arguments], where <options> is as follows:\n"
    "\t-v [level]\tSets the verbosity level. [level] can be one of the\n"
    "\t\t\tfollowing:\n\n"
    "\t\t\t0: Silent \t --does not output any message\n"
    "\t\t\t1: Print-only \t --only prints out messages on stderr;\n"
    "\t\t\t\t\t   does not log any message into a file\n"
    "\t\t\t2: Print-and-log --both prints and logs all messages\n"
    "\t\t\t\t\t   to stderr and into a file. The file\n"
    "\t\t\t\t\t   path must be provided via -f flag.\n\n"
    "\t\t\tThe default value is 1.\n\n"
    "\t-f [logfile]\tLogs all message into [logfile]. -v 2 must be specified\n\n"
    "\t-t [filetype]\tOverrides automatic executable file type detection.\n"
    "\t\t\t[filetype] can be one of the following:\n\n"
    "\t\t\tpe : Portable Executable (x86)\n"
    "\t\t\telf: Executable and Linkable Format (x86)\n"
    "\t\t\traw: Raw binary blob\n"
    "\t\t\ttxt: Binary encoded in hexadecimal string\n\n"
    "\t-c [contextfile]Path to thread context file that contains the initial\n"
    "\t\t\tthread context (optional). This file is consulted only\n"
    "\t\t\tif the executable file is a binary blob or a binary\n"
    "\t\t\tencoded in hexadecimal string. The intial stack for\n"
    "\t\t\tthe executable will be positioned according to the \n"
    "\t\t\tvalue of esp register, modulo the page size (4KB)\n\n"
    "\t-b [base]\tBase address at which the program will be loaded\n"
    "\t\t\t(optional). This value is used only if the\n"
    "\t\t\texecutable file is a binary blob or a binary\n"
    "\t\t\tencoded in hexadecimal string. The default value is\n"
    "\t\t\t0x400000\n\n"
    "\t-e [entrypoint] Initial entry point that will be executed (optional)\n"
    "\t\t\tThis value is used only if the executable file is a\n"
    "\t\t\tbinary blob or a binary encoded in hexadecimal\n"
    "\t\t\tstring. The default value is 0x400000\n\n"
    "\tAny arguments following filename will be passed directly to the\n"
    "\texecutable as argv (only applicable to PE and ELF file formats).\n"
    , name);
}
