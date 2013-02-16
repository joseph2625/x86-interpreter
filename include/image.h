#ifndef X86INTERPRETER_IMAGE_H
#define X86INTERPRETER_IMAGE_H

#include <stdio.h>

#define READ_BUFFER_SIZE 64*1024

typedef enum ExecutableType{
  PE,
  ELF,
  RAW
} ExecutableType_t;

typedef enum ArchitectureType{
  INTEL64,
  IA32
} ArchitectureType_t;

#define SECTION_CHARACTERISTIC_MEM_READ (1 << 0)
#define SECTION_CHARACTERISTIC_MEM_WRITE (1 << 1)
#define SECTION_CHARACTERISTIC_MEM_EXECUTE (1 << 2)

typedef struct ImageSection {
  unsigned name[8]; //this is NOT null-terminated string
  size_t raw_size;
  unsigned int relative_virtual_base_address;
  unsigned int virtual_size;
  unsigned int characteristics;

  unsigned char *buffer;
} ImageSection_t;

typedef struct Image {
	char *name;
	unsigned char *buffer; //should be freed after use
	unsigned int relative_virtual_entry_point;
  unsigned int image_base;
  size_t raw_size;
  unsigned int stack_size;

  ArchitectureType_t architecture_type;
  ExecutableType_t executable_type;
  unsigned int number_of_sections;

  ImageSection_t *sections; //should be freed after use

} Image_t;

bool open_and_read_image_from_disk( char *file_name, void **destination_buffer );
bool load_image( Image_t *image );
bool process_image( Image_t *image );
void *get_raw_image_at_offset( Image_t *image, unsigned int offset, size_t size );
#endif //X86INTERPRETER_IMAGE_H