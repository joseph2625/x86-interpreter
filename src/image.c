#include "image.h"
#include <stdlib.h>
#include "pe.h"
#include "elf.h"
#include <assert.h>
#include "diagnostics.h"
#include "raw.h"
#include "options.h"

bool open_and_read_file_from_disk( char *file_path, void **file_buffer, size_t *file_size )
{
#ifdef _WIN32 //fopen_s warning.
  FILE *file;
  errno_t err = fopen_s( &file, file_path, "rb" );
  if( err != 0 ) {
#else
	FILE *file = fopen( file_path, "rb" );
  if( file == NULL ) {
#endif
    log_message( ERROR, "Unable to open the executable image for reading: %s", file_path );
    return false;
  }

	unsigned char *buffer=(unsigned char *)malloc( READ_BUFFER_SIZE );
	size_t current_file_size=0;
	size_t bytes_read;
  size_t current_buffer_size=64*1024;

	while( true ) {
    bytes_read = fread( &buffer[current_file_size], 1, current_buffer_size - current_file_size, file );
    
    if( bytes_read < current_buffer_size - current_file_size ) {

      fclose( file );

      if( ferror( file ) ) {
        free( buffer );
        return false;
      } else { //eof
        *file_size = current_file_size + bytes_read;
        *file_buffer = buffer;
        return true;
      }
    }

    current_file_size += bytes_read;
    
    current_buffer_size *= 2;

    buffer = (unsigned char *)realloc( buffer, current_buffer_size );
    if( buffer == NULL ) {
      fclose( file );
      return false;
	  }
  }

  return false;
}


bool load_image( InterpreterOptions_t *options, Image_t *const image ) {

  if( !open_and_read_file_from_disk( options->executable_file_path, (void **)&image->buffer, &image->raw_size ) )
    return false;

  if( !process_image( options->executable_type_override, image, options->base_address, options->entry_point ) )
    return false;

	return true;
}

void *get_raw_image_at_offset( Image_t *const image, const uint32_t offset, const size_t size ) {
  
  if( offset + size > image->raw_size )
    return NULL;

  return &image->buffer[offset];
}

bool process_image( ExecutableType_t type_detection_override, Image_t *image, uint32_t base_address, uint32_t entry_point )
{

  if( image->raw_size < 4 )
    return false;

  if( type_detection_override == PE )
    return process_pe( image );
  else if( type_detection_override == ELF )
    return process_elf( image );
  else
    return process_raw( image, base_address, entry_point );

  if( image->buffer[0] == 0x4D && image->buffer[1] == 0x5A ) {
    image->executable_type = PE;
    return process_pe( image );
  } else if( image->buffer[0] == 0x7F && image->buffer[1] == 0x45 && image->buffer[2] ==  0x4C && image->buffer[3] == 0x46 ) {
    image->executable_type = ELF;
    return process_elf( image );
  } else {
    image->executable_type = RAW;
    return process_raw( image, base_address, entry_point );
  }
  return false;
}
