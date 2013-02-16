#include "image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BYTE uint8_t
#define WORD uint16_t
#define DWORD uint32_t
#define LONG DWORD
#define ULONGLONG uint64_t

typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
  WORD   e_magic;                     // Magic number
  WORD   e_cblp;                      // Bytes on last page of file
  WORD   e_cp;                        // Pages in file
  WORD   e_crlc;                      // Relocations
  WORD   e_cparhdr;                   // Size of header in paragraphs
  WORD   e_minalloc;                  // Minimum extra paragraphs needed
  WORD   e_maxalloc;                  // Maximum extra paragraphs needed
  WORD   e_ss;                        // Initial (relative) SS value
  WORD   e_sp;                        // Initial SP value
  WORD   e_csum;                      // Checksum
  WORD   e_ip;                        // Initial IP value
  WORD   e_cs;                        // Initial (relative) CS value
  WORD   e_lfarlc;                    // File address of relocation table
  WORD   e_ovno;                      // Overlay number
  WORD   e_res[4];                    // Reserved words
  WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
  WORD   e_oeminfo;                   // OEM information; e_oemid specific
  WORD   e_res2[10];                  // Reserved words
  LONG   e_lfanew;                    // File address of new exe header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
  WORD    Machine;
  WORD    NumberOfSections;
  DWORD   TimeDateStamp;
  DWORD   PointerToSymbolTable;
  DWORD   NumberOfSymbols;
  WORD    SizeOfOptionalHeader;
  WORD    Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16

typedef struct _IMAGE_DATA_DIRECTORY {
  DWORD   VirtualAddress;
  DWORD   Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER {
  //
  // Standard fields.
  //

  WORD    Magic;
  BYTE    MajorLinkerVersion;
  BYTE    MinorLinkerVersion;
  DWORD   SizeOfCode;
  DWORD   SizeOfInitializedData;
  DWORD   SizeOfUninitializedData;
  DWORD   AddressOfEntryPoint;
  DWORD   BaseOfCode;
  DWORD   BaseOfData;

  //
  // NT additional fields.
  //

  DWORD   ImageBase;
  DWORD   SectionAlignment;
  DWORD   FileAlignment;
  WORD    MajorOperatingSystemVersion;
  WORD    MinorOperatingSystemVersion;
  WORD    MajorImageVersion;
  WORD    MinorImageVersion;
  WORD    MajorSubsystemVersion;
  WORD    MinorSubsystemVersion;
  DWORD   Win32VersionValue;
  DWORD   SizeOfImage;
  DWORD   SizeOfHeaders;
  DWORD   CheckSum;
  WORD    Subsystem;
  WORD    DllCharacteristics;
  DWORD   SizeOfStackReserve;
  DWORD   SizeOfStackCommit;
  DWORD   SizeOfHeapReserve;
  DWORD   SizeOfHeapCommit;
  DWORD   LoaderFlags;
  DWORD   NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
  WORD        Magic;
  BYTE        MajorLinkerVersion;
  BYTE        MinorLinkerVersion;
  DWORD       SizeOfCode;
  DWORD       SizeOfInitializedData;
  DWORD       SizeOfUninitializedData;
  DWORD       AddressOfEntryPoint;
  DWORD       BaseOfCode;
  ULONGLONG   ImageBase;
  DWORD       SectionAlignment;
  DWORD       FileAlignment;
  WORD        MajorOperatingSystemVersion;
  WORD        MinorOperatingSystemVersion;
  WORD        MajorImageVersion;
  WORD        MinorImageVersion;
  WORD        MajorSubsystemVersion;
  WORD        MinorSubsystemVersion;
  DWORD       Win32VersionValue;
  DWORD       SizeOfImage;
  DWORD       SizeOfHeaders;
  DWORD       CheckSum;
  WORD        Subsystem;
  WORD        DllCharacteristics;
  ULONGLONG   SizeOfStackReserve;
  ULONGLONG   SizeOfStackCommit;
  ULONGLONG   SizeOfHeapReserve;
  ULONGLONG   SizeOfHeapCommit;
  DWORD       LoaderFlags;
  DWORD       NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_NT_HEADERS {
  DWORD Signature;
  IMAGE_FILE_HEADER FileHeader;
  IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;

typedef struct _IMAGE_NT_HEADERS64 {
  DWORD Signature;
  IMAGE_FILE_HEADER FileHeader;
  IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;

#define IMAGE_NT_OPTIONAL_HDR32_MAGIC      0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC      0x20b

#define IMAGE_SIZEOF_SHORT_NAME              8

typedef struct _IMAGE_SECTION_HEADER {
  BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
  union {
    DWORD   PhysicalAddress;
    DWORD   VirtualSize;
  } Misc;
  DWORD   VirtualAddress;
  DWORD   SizeOfRawData;
  DWORD   PointerToRawData;
  DWORD   PointerToRelocations;
  DWORD   PointerToLinenumbers;
  WORD    NumberOfRelocations;
  WORD    NumberOfLinenumbers;
  DWORD   Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00
//
// Section characteristics.
//
//      IMAGE_SCN_TYPE_REG                   0x00000000  // Reserved.
//      IMAGE_SCN_TYPE_DSECT                 0x00000001  // Reserved.
//      IMAGE_SCN_TYPE_NOLOAD                0x00000002  // Reserved.
//      IMAGE_SCN_TYPE_GROUP                 0x00000004  // Reserved.
#define IMAGE_SCN_TYPE_NO_PAD                0x00000008  // Reserved.
//      IMAGE_SCN_TYPE_COPY                  0x00000010  // Reserved.

#define IMAGE_SCN_CNT_CODE                   0x00000020  // Section contains code.
#define IMAGE_SCN_CNT_INITIALIZED_DATA       0x00000040  // Section contains initialized data.
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA     0x00000080  // Section contains uninitialized data.

#define IMAGE_SCN_LNK_OTHER                  0x00000100  // Reserved.
#define IMAGE_SCN_LNK_INFO                   0x00000200  // Section contains comments or some other type of information.
//      IMAGE_SCN_TYPE_OVER                  0x00000400  // Reserved.
#define IMAGE_SCN_LNK_REMOVE                 0x00000800  // Section contents will not become part of image.
#define IMAGE_SCN_LNK_COMDAT                 0x00001000  // Section contents comdat.
//                                           0x00002000  // Reserved.
//      IMAGE_SCN_MEM_PROTECTED - Obsolete   0x00004000
#define IMAGE_SCN_NO_DEFER_SPEC_EXC          0x00004000  // Reset speculative exceptions handling bits in the TLB entries for this section.
#define IMAGE_SCN_GPREL                      0x00008000  // Section content can be accessed relative to GP
#define IMAGE_SCN_MEM_FARDATA                0x00008000
//      IMAGE_SCN_MEM_SYSHEAP  - Obsolete    0x00010000
#define IMAGE_SCN_MEM_PURGEABLE              0x00020000
#define IMAGE_SCN_MEM_16BIT                  0x00020000
#define IMAGE_SCN_MEM_LOCKED                 0x00040000
#define IMAGE_SCN_MEM_PRELOAD                0x00080000

#define IMAGE_SCN_ALIGN_1BYTES               0x00100000  //
#define IMAGE_SCN_ALIGN_2BYTES               0x00200000  //
#define IMAGE_SCN_ALIGN_4BYTES               0x00300000  //
#define IMAGE_SCN_ALIGN_8BYTES               0x00400000  //
#define IMAGE_SCN_ALIGN_16BYTES              0x00500000  // Default alignment if no others are specified.
#define IMAGE_SCN_ALIGN_32BYTES              0x00600000  //
#define IMAGE_SCN_ALIGN_64BYTES              0x00700000  //
#define IMAGE_SCN_ALIGN_128BYTES             0x00800000  //
#define IMAGE_SCN_ALIGN_256BYTES             0x00900000  //
#define IMAGE_SCN_ALIGN_512BYTES             0x00A00000  //
#define IMAGE_SCN_ALIGN_1024BYTES            0x00B00000  //
#define IMAGE_SCN_ALIGN_2048BYTES            0x00C00000  //
#define IMAGE_SCN_ALIGN_4096BYTES            0x00D00000  //
#define IMAGE_SCN_ALIGN_8192BYTES            0x00E00000  //
// Unused                                    0x00F00000
#define IMAGE_SCN_ALIGN_MASK                 0x00F00000

#define IMAGE_SCN_LNK_NRELOC_OVFL            0x01000000  // Section contains extended relocations.
#define IMAGE_SCN_MEM_DISCARDABLE            0x02000000  // Section can be discarded.
#define IMAGE_SCN_MEM_NOT_CACHED             0x04000000  // Section is not cachable.
#define IMAGE_SCN_MEM_NOT_PAGED              0x08000000  // Section is not pageable.
#define IMAGE_SCN_MEM_SHARED                 0x10000000  // Section is shareable.
#define IMAGE_SCN_MEM_EXECUTE                0x20000000  // Section is executable.
#define IMAGE_SCN_MEM_READ                   0x40000000  // Section is readable.
#define IMAGE_SCN_MEM_WRITE                  0x80000000  // Section is writeable.

bool process_pe( Image_t *image )
{
  IMAGE_DOS_HEADER *dos_header = (IMAGE_DOS_HEADER *)get_raw_image_at_offset( image, 0, sizeof( IMAGE_DOS_HEADER ));
  
  if( !dos_header )
    return false;

  IMAGE_NT_HEADERS32 *nt_header = (IMAGE_NT_HEADERS32 *)get_raw_image_at_offset( image, dos_header->e_lfanew, sizeof( IMAGE_NT_HEADERS32 ));

  if( !nt_header )
    return false;

  if( nt_header->Signature != IMAGE_NT_SIGNATURE )
    return false;

  if( nt_header->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC )
    image->architecture_type = IA32;
  else if( nt_header->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC )
    image->architecture_type = INTEL64;
  else
    return false;

  IMAGE_SECTION_HEADER *section_header = NULL;

  if( image->architecture_type == IA32 ){
    image->image_base = nt_header->OptionalHeader.ImageBase;
    image->relative_virtual_entry_point = nt_header->OptionalHeader.AddressOfEntryPoint;
    image->number_of_sections = nt_header->FileHeader.NumberOfSections;
    image->stack_size = nt_header->OptionalHeader.SizeOfStackReserve;
    section_header = (IMAGE_SECTION_HEADER *)(nt_header + 1);

  } else if( image->architecture_type == INTEL64 ){
    IMAGE_NT_HEADERS64 *nt_header64 = (IMAGE_NT_HEADERS64 *)nt_header;

    //TODO: HANDLE THE CASE WHERE RV ENTRY POINT > 4GB
    image->image_base = nt_header64->OptionalHeader.ImageBase;
    image->relative_virtual_entry_point = nt_header64->OptionalHeader.AddressOfEntryPoint;
    image->number_of_sections = nt_header64->FileHeader.NumberOfSections;

    //TODO: HANDLE THE CASE WHERE SIZE OF STACK > 4GB
    image->stack_size = nt_header64->OptionalHeader.SizeOfStackReserve;
    section_header = (IMAGE_SECTION_HEADER *)(nt_header64 + 1);
  } else
    return false;

  image->sections = (ImageSection_t *)calloc( image->number_of_sections, sizeof(ImageSection_t) );
  
  if(image->sections == NULL)
    return false;

  for( unsigned int i = 0; i < image->number_of_sections; i++, section_header++ ){
    image->sections[i].buffer = (unsigned char *)get_raw_image_at_offset( image, section_header->PointerToRawData, section_header->SizeOfRawData );
    if( image->sections[i].buffer == NULL ) {
      free( image->sections );
      return false;
    }
    memcpy( image->sections[i].name, section_header->Name, sizeof( image->sections[i].name ) );
    image->sections[i].raw_size = section_header->SizeOfRawData;
    image->sections[i].relative_virtual_base_address = section_header->VirtualAddress;
    image->sections[i].virtual_size = section_header->Misc.VirtualSize;

    image->sections[i].characteristics = 0;
    if( section_header->Characteristics &  IMAGE_SCN_MEM_READ)
      image->sections[i].characteristics |= SECTION_CHARACTERISTIC_MEM_READ;
    if( section_header->Characteristics &  IMAGE_SCN_MEM_WRITE)
      image->sections[i].characteristics |= SECTION_CHARACTERISTIC_MEM_WRITE;
    if( section_header->Characteristics &  IMAGE_SCN_MEM_EXECUTE)
      image->sections[i].characteristics |= SECTION_CHARACTERISTIC_MEM_EXECUTE;

  }
  return true;
}
