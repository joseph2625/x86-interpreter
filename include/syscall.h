#ifndef X86INTERPRETER_SYSCALL_H
#define X86INTERPRETER_SYSCALL_H
#include "interpreter.h"
#ifdef _MSC_VER
typedef struct{
  time_t   tv_sec;        /* seconds */
  long     tv_nsec;       /* nanoseconds */
} timespec_t;
#define CLOCK_MONOTONIC 1
#endif

size_t handle_sys_write( VirtualDirectoryLookupTable_t *table, int fd, uint32_t buffer_address, size_t count );
size_t handle_sys_read( VirtualDirectoryLookupTable_t *table, int fd, uint32_t buffer_address, size_t count );
int handle_sys_clock_gettime( VirtualDirectoryLookupTable_t *table, uint32_t clkid, uint32_t timespec_address );

#endif //X86INTERPRETER_SYSCALL_H
