#include "image.h"
#include <stdlib.h>
#include "pe.h"
#include <assert.h>

bool open_and_read_image_from_disk( Image_t *const image )
{
	FILE *file = fopen( image->name, "rb" );

	if( file == NULL )
		return false;

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
        image->raw_size = current_file_size + bytes_read;
        image->buffer = buffer;
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


bool load_image( Image_t *const image ) {

  if( !open_and_read_image_from_disk( image ) )
    return false;

  if( !process_image( image ) )
    return false;

	return true;
}

void *get_raw_image_at_offset( Image_t *const image, const uint32_t offset, const size_t size ) {
  
  if( offset + size > image->raw_size )
    return NULL;

  return &image->buffer[offset];
}

bool process_image( Image_t * const image ) {

  if( image->raw_size < 4 )
    return false;

  if( image->buffer[0] == 0x4D && image->buffer[1] == 0x5A ) {
    image->executable_type = PE;
    return process_pe( image );
  } else if( image->buffer[0] == 0x7F && image->buffer[1] == 0x45 && image->buffer[2] ==  0x4C && image->buffer[3] == 0x46 ) {
    image->executable_type = ELF;
  } else {
    image->executable_type = RAW;
  }
  return false;
}
