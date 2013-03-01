#ifndef X86INTERPRETER_IMAGE_H
#define X86INTERPRETER_IMAGE_H
#ifndef _WIN32
#include <stdbool.h>
#endif
#include <stdio.h>
#include <stdint.h>

#define READ_BUFFER_SIZE 64*1024

typedef enum ExecutableType{
  PE,
  ELF,
  RAW,
  TXT,
  INVALID
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
  uint32_t relative_virtual_base_address;
  uint32_t virtual_size;
  uint32_t characteristics;

  unsigned char *buffer;
} ImageSection_t;

typedef struct Image {
	unsigned char *buffer; //should be freed after use
	uint32_t relative_virtual_entry_point;
  uint32_t default_thread_entry_point; //normally points to the entry trampoline for PEs. same as relative_virtual_entry_point for binary blobs
  unsigned int image_base;
  size_t raw_size;
  uint32_t stack_size;

  ArchitectureType_t architecture_type;
  ExecutableType_t executable_type;
  uint32_t number_of_sections;

  ImageSection_t *sections; //should be freed after use

} Image_t;
struct InterpreterOptions;
bool load_image( struct InterpreterOptions *options, Image_t *image );
bool process_image( ExecutableType_t type_detection_override, Image_t *image, uint32_t base_address, uint32_t entry_point );
void *get_raw_image_at_offset( Image_t *image, uint32_t offset, size_t size );
bool open_and_read_file_from_disk( char *file_path, void **file_buffer, size_t *file_size );
#endif //X86INTERPRETER_IMAGE_H
