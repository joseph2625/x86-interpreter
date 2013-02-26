#include <stdio.h>
#include "image.h"
#include "runtime.h"
#include "string.h"
#include <stdlib.h>

int main( int argc, char *argv[] ) {

  Image_t image = { 0 };
  uint32_t verbosity_level = 2;

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
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  sem_t notifier_sem;
  sem_t wait_sem;
  sem_init( &notifier_sem, 0, 0 );
  sem_init( &wait_sem, 0, 0 );

  if( !set_up_runtime_environment( &image, &environment, &mutex, &notifier_sem, &wait_sem ) ) {
    fprintf( stderr, "ERROR: Unable to set up the runtime environment\n" );
    return 1;
  }

  while( true ){
    sem_wait( &notifier_sem );
    sem_post( &wait_sem );
    if( !update_runtime_environment( &environment ))
      break;
  }
  fprintf( stderr, "INFO: All threads terminated. Exiting...\n" );
  return 0;
err_no_level:
  fprintf( stderr, "ERROR: Missing/Invalid verbosity level\n" );
  return 1;
err_no_name:
  fprintf( stderr, "ERROR: Executable name must be specified\n" );
  return 1;
}
