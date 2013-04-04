#ifndef X86INTERPRETER_RUNTIME_H
#define X86INTERPRETER_RUNTIME_H

#include "image.h"
#include "pthread.h"
#include "semaphore.h"
#include <stdint.h>

#define UNSPECIFIED_BASE 0xFFFFFFFF
#define UNSPECIFIED_SIZE 0xFFFFFFFF
#define RESERVED_REGION_HIGH_START 0xC0000000
#define RESERVED_REGION_HIGH_END 0xFFFFFFFF
#define RESERVED_REGION_HIGH_SIZE ( RESERVED_REGION_HIGH_END - RESERVED_REGION_HIGH_START + 1 )

#define RESERVED_REGION_LOW_START 0x0
#define RESERVED_REGION_LOW_END 0xFFFF
#define RESERVED_REGION_LOW_SIZE ( RESERVED_REGION_LOW_END - RESERVED_REGION_LOW_START + 1 )

#define DEFAULT_JUMP_TRAMPOLINE_ADDRESS 0x10000

typedef struct VirtualPageLookupTable {
  unsigned char *frames[1024];
} VirtualPageLookupTable_t;

typedef struct VirtualDirectoryLookupTable {
  VirtualPageLookupTable_t *page_lookup_table[1024];
  uint32_t tlb_key;
  unsigned char *tlb_value;
} VirtualDirectoryLookupTable_t;

#define REGISTER_INDEX_EAX 0
#define REGISTER_INDEX_ECX 1
#define REGISTER_INDEX_EDX 2
#define REGISTER_INDEX_EBX 3
#define REGISTER_INDEX_ESP 4
#define REGISTER_INDEX_EBP 5
#define REGISTER_INDEX_ESI 6
#define REGISTER_INDEX_EDI 7

typedef enum ThreadState {
  EXITED,
  RUNNING
} ThreadState_t;

#define READ 0x1
#define WRITE 0x2
#define EXECUTE 0x4

typedef struct VirtualPageNode {
  uint32_t begin;
  size_t size;
  unsigned char *buffer;

  struct VirtualPageNode *next;
} VirtualPageNode_t;

struct ThreadNode;
struct InterpreterOptions;

typedef struct RuntimeEnvironment {
  VirtualDirectoryLookupTable_t directory_table;
  VirtualPageNode_t *page_list;

  unsigned int thread_count;
  struct ThreadNode *threads;
  struct InterpreterOptions *options;

  uint32_t default_thread_entry_point;
  
  uint32_t virtual_argv_address;

} RuntimeEnvironment_t;
#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

typedef struct ThreadContextFile {
  uint32_t general_purpose_registers[8];
  uint16_t segment_registers[6];
  uint32_t eflags;
} 
#ifndef _MSC_VER
__attribute__((packed))
#endif
ThreadContextFile_t;

#ifdef _MSC_VER
#pragma pack(pop)
#endif

typedef struct ThreadContext {

  union {
    uint16_t segment_registers[6];
    struct {
      uint16_t es;
      uint16_t cs;
      uint16_t ss;
      uint16_t ds;
      uint16_t fs;
      uint16_t gs;
    };
  };


  union {
    uint8_t register_field[32];
    uint32_t general_purpose_registers[8];
    struct {
      uint32_t eax;
      uint32_t ecx;
      uint32_t edx;
      uint32_t ebx;
      uint32_t esp;
      uint32_t ebp;
      uint32_t esi;
      uint32_t edi;
    };
  };
  uint32_t eip;

  uint32_t eflags;


  unsigned char *code;

  RuntimeEnvironment_t *environment;
} ThreadContext_t;

typedef struct ThreadNode{
  ThreadContext_t context;
  ThreadState_t state;
  uint32_t exit_code;
  pthread_t pthread;
  struct ThreadNode *next;
} ThreadNode_t;


typedef struct PthreadContext{
  ThreadNode_t *thread_node;
  RuntimeEnvironment_t *runtime_environment;
  pthread_mutex_t *mutex;
  sem_t *notifier_sem;
  sem_t *wait_sem;
} PthreadContext_t;

bool set_up_runtime_environment( struct InterpreterOptions *options, Image_t *image, RuntimeEnvironment_t *environment, pthread_mutex_t *mutex, sem_t *notifier_sem, sem_t *wait_sem );
bool update_runtime_environment( RuntimeEnvironment_t *environment );
bool create_thread( RuntimeEnvironment_t *environment, char * initial_thread_context_file_path, bool use_default_entry_point, uint32_t entry_point, uint32_t argument1, uint32_t argument2 ,uint32_t stack_size, pthread_mutex_t *mutex, sem_t *notifier_sem, sem_t *wait_sem );
#endif //X86INTERPRETER_RUNTIME_H
