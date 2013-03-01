#include <stdio.h>
#include "image.h"
#include "runtime.h"
#include "string.h"
#include "options.h"
#include <stdlib.h>
#include "diagnostics.h"

void cleanup(Image_t *image, RuntimeEnvironment_t *environment, InterpreterOptions_t *options, sem_t * notifier_sem, sem_t *wait_sem ){
  if( image->buffer != NULL )
    free(image->buffer);
}
int main( int argc, char *argv[] ) {

  Image_t image = { 0 };
  InterpreterOptions_t options = { 0 };

  if( argc < 2 ) {
    print_usage( argv[0] );
    return -1;
  }

  if( !parse_options( argc, argv, &options ) ) {
    print_usage(argv[0]);
    return 0;
  }

  initialize_diagnostics( options.log_file_path, options.verbosity_level );

  if( !load_image( &options, &image ) ) {
    log_message( ERROR, "Unable to load the specified executable image" );
    return 1;
  }

  RuntimeEnvironment_t environment;
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  sem_t notifier_sem;
  sem_t wait_sem;
  sem_init( &notifier_sem, 0, 0 );
  sem_init( &wait_sem, 0, 0 );

  if( !set_up_runtime_environment( &options, &image, &environment, &mutex, &notifier_sem, &wait_sem ) ) {
    log_message( ERROR, "Unable to set up the runtime environment" );
    goto on_error;
  }

  while( true ){
    sem_wait( &notifier_sem );
    sem_post( &wait_sem );
    if( !update_runtime_environment( &environment ))
      break;
  }
  log_message( INFO ,"All threads terminated. Exiting..." );
  return 0;
on_error:
  cleanup( &image, &environment, &options, &notifier_sem, &wait_sem );
  return -1;
}
