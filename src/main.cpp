#include <stdio.h>
#include "image.h"
#include "runtime.h"
#include "string.h"
#include <stdlib.h>

int main( int argc, char *argv[] ) {

  Image_t image = { 0 };
  unsigned int verbosity_level = 2;

  for( int i = 1; i < argc; i++ ) {

    if( !strcmp( argv[i], "-v" ) ) {

      if( argc == ++i )
        goto err_no_level;

      verbosity_level = atoi(argv[i]);

      if( verbosity_level < 1 || verbosity_level > 2)
        goto err_no_level;

    } else {
      image.name = argv[i];
    }

  }

  if( image.name == NULL )
    goto err_no_name;

  if( !load_image( &image ) ) {
    fprintf( stderr, "ERROR: Unable to load the specified executable image\n" );
    return 1;
  }

  RuntimeEnvironment_t environment;
  ThreadNode_t *main_thread;

  main_thread = set_up_runtime_environment( &image, &environment );
  if( main_thread == NULL ) {
    fprintf( stderr, "ERROR: Unable to set up the runtime environment\n" );
    return 1;
  }




  return 0;
err_no_level:
  fprintf( stderr, "ERROR: Missing/Invalid verbosity level\n" );
  return 1;
err_no_name:
  fprintf( stderr, "ERROR: Executable name must be specified\n" );
  return 1;
}