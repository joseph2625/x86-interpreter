#include "raw.h"
#include <string.h>
#include "diagnostics.h"
#include <stdlib.h>

// destination buffer is assumed to be big enough
int parse_hex_string( char *source, size_t source_size, unsigned char *destination ) {
  unsigned int j = 0;
  unsigned char val = 0;
  for( unsigned int i = 0; i < source_size; i++ ) {
    if( source[i] == ' ' )
      continue;
    else if( source[i] >= '0' && source[i] <= '9' )
      val |= source[i] - '0';
    else if( source[i] >= 'a' && source[i] <= 'f' )
      val |= source[i] - 'a' + 10;
    else if( source[i] >= 'A' && source[i] <= 'F' )
      val |= source[i] - 'A' + 10;
    else {
      log_message( ERROR, "Invalid character found in the binary" );
      return 0;
    }

    j++;

    if( j % 2 )
      val = val << 4;
    else {
      destination[j/2 - 1] = val;
      val = 0;
    }
  }
  if( j % 2 ) {
    log_message( ERROR, "Unexpected EOF" );
    return 0;
  }

  return j/2;
}

bool process_raw( Image_t *image, uint32_t base_address, uint32_t entry_point )
{
  unsigned int i;
  for( i=0; i < image->raw_size; i++ ) {
    if( !( ( image->buffer[i] >= '0' && image->buffer[i] <= '9' ) || ( image->buffer[i] >= 'A' && image->buffer[i] <= 'F') || ( image->buffer[i] >= 'a' && image->buffer[i] <= 'f' ) || ( image->buffer[i] == ' ' ) ) ) {
      image->executable_type = RAW;
      break;
    }
  }
  unsigned char *blob;
  size_t blob_size;

  unsigned char sys_exit_code[] = { 0x8B, 0xD8, 0xB8, 0x01, 0x00, 0x00, 0x00, 0xCD, 0x80 };

  if( i == image->raw_size ) {
    image->executable_type = TXT;
    //Increase the size of the raw buffer by a factor of 2, then use the second half to store the binary. This is to ensure that we can free the buffer in a uniform manner during the cleanup.
    blob = (unsigned char *)realloc( image->buffer, image->raw_size * 2 + sizeof(sys_exit_code));
    if( blob == NULL )
      return false;
    
    image->buffer = blob;

    blob_size = parse_hex_string( (char *)image->buffer, image->raw_size, &image->buffer[image->raw_size] );
    
    if( blob_size == 0 )
      return false;

    blob = &image->buffer[image->raw_size];
    image->raw_size += blob_size;

    memcpy( &blob[blob_size], sys_exit_code, sizeof( sys_exit_code ) );
    blob_size += sizeof( sys_exit_code );

  } else {
    blob = (unsigned char *)realloc( image->buffer, image->raw_size + sizeof( sys_exit_code ) );
    if( blob == NULL )
      return false;

    image->buffer = blob;
    memcpy( &blob[image->raw_size], sys_exit_code, sizeof( sys_exit_code ) );
    image->raw_size += sizeof( sys_exit_code );
    blob_size = image->raw_size;
  }

  image->image_base = 0;
  image->relative_virtual_entry_point = entry_point;
  image->number_of_sections = 1;
  image->stack_size = 1048576;

  image->sections = (ImageSection_t *)calloc( image->number_of_sections, sizeof(ImageSection_t) );

  if(image->sections == NULL)
    return false;

  image->sections->buffer = blob;
  image->sections->raw_size = blob_size;
  image->sections->characteristics = SECTION_CHARACTERISTIC_MEM_EXECUTE | SECTION_CHARACTERISTIC_MEM_READ;
  image->sections->relative_virtual_base_address = base_address;
  image->sections->virtual_size = blob_size + (4096 - (blob_size % 4096));
  image->sections->name[0] = '\0';

  return true;
}
