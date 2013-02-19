#ifndef X86INTERPRETER_RUNTIME_H
#define X86INTERPRETER_RUNTIME_H

#include "image.h"
#include "pthread.h"
#include "semaphore.h"
#include <stdint.h>

#define UNSPECIFIED_BASE 0xFFFFFFFF
#define UNSPECIFIED_SIZE 0xFFFFFFFF

typedef struct InterpreterOptions {
  unsigned int verbose_level;
} InterpreterOptions_t;

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

typedef struct ThreadContext {
  
  uint16_t gs;
  uint16_t fs;
  uint16_t es;
  uint16_t ds;
  uint16_t cs;
  uint16_t ss;

  union {
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

} ThreadContext_t;

typedef enum ThreadState {
  EXITED,
  RUNNING
} ThreadState_t;

typedef struct ThreadNode{
  ThreadContext_t context;
  ThreadState_t state;
  uint32_t exit_code;
  pthread_t pthread;
  ThreadNode *next;
} ThreadNode_t;

#define READ 0x1
#define WRITE 0x2
#define EXECUTE 0x4

typedef struct VirtualPageNode {
  uint32_t begin;
  size_t size;
  unsigned char *buffer;

  VirtualPageNode *next;
} VirtualPageNode_t;

typedef struct RuntimeEnvironment {
  VirtualDirectoryLookupTable_t directory_table;
  VirtualPageNode *page_list;

  unsigned int thread_count;
  ThreadNode_t *threads;

} RuntimeEnvironment_t;

typedef struct PthreadContext{
  ThreadNode_t *thread_node;
  RuntimeEnvironment_t *runtime_environment;
  pthread_mutex_t mutex;
  sem_t notifier_sem;
  sem_t wait_sem;
} PthreadContext_t;

bool set_up_runtime_environment( Image_t *image, RuntimeEnvironment_t *environment, pthread_mutex_t mutex, sem_t notifier_sem, sem_t wait_sem );
bool create_thread( RuntimeEnvironment_t *environment, uint32_t entry_point, uint32_t stack_size, pthread_mutex_t mutex, sem_t notifier_sem, sem_t wait_sem );
bool update_runtime_environment( RuntimeEnvironment_t *environment );
#endif //X86INTERPRETER_RUNTIME_H