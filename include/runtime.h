#ifndef X86INTERPRETER_RUNTIME_H
#define X86INTERPRETER_RUNTIME_H

#include "image.h"
#include "pthread.h"
#include "semaphore.h"

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
} VirtualDirectoryLookupTable_t;

typedef struct ThreadContext {
  unsigned int status;
  
  unsigned int gs;
  unsigned int fs;
  unsigned int es;
  unsigned int ds;

  unsigned int edi;
  unsigned int esi;
  unsigned int ebx;
  unsigned int edx;
  unsigned int ecx;
  unsigned int eax;

  unsigned int ebp;
  unsigned int eip;
  unsigned int cs;
  unsigned int eflags;
  unsigned int esp;
  unsigned int ss;

} ThreadContext_t;

typedef enum ThreadState {
  SUSPENDED,
  RUNNING
} ThreadState_t;

typedef struct ThreadNode{
  ThreadContext_t context;
  ThreadState_t state;
  pthread_t pthread;
  ThreadNode *next;
} ThreadNode_t;



typedef struct VirtualPageNode {
  unsigned int begin;
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
  sem_t notifier_sem;
  sem_t wait_sem;
  pthread_mutex_t mutex;
} PthreadContext_t;

ThreadNode_t *set_up_runtime_environment( Image_t *image, RuntimeEnvironment_t *environment );
ThreadNode_t *create_thread( RuntimeEnvironment_t *environment, unsigned int entry_point, unsigned int stack_size );
#endif //X86INTERPRETER_RUNTIME_H