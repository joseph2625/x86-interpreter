#ifndef X86INTERPRETER_INTERPRETER_H
#define X86INTERPRETER_INTERPRETER_H

#include "runtime.h"

#define GETEXTOPCODE(modrm) ( ( modrm >> 3 ) & 0x7 )
#define GETRM(modrm) ( modrm & 0x7 )
#define GETREG(context, regnum) (context)->registers[regnum]

//Compute parity by lookup table
//Borrowed the code from http://graphics.stanford.edu/~seander/bithacks.html
#define P2(n) n, n^1, n^1, n
#define P4(n) P2(n), P2(n^1), P2(n^1), P2(n)
#define P6(n) P4(n), P4(n^1), P4(n^1), P4(n)
static const bool parity_table[256] = {
  P6(0), P6(1), P6(1), P6(0)
};

#define SETCF(eflags) ((eflags) = (eflags) | 1)
#define SETPF(eflags) ((eflags) = (eflags) | (1 << 2))
#define SETAF(eflags) ((eflags) = (eflags) | (1 << 4))
#define SETZF(eflags) ((eflags) = (eflags) | (1 << 6))
#define SETSF(eflags) ((eflags) = (eflags) | (1 << 7))
#define SETTF(eflags) ((eflags) = (eflags) | (1 << 8))
#define SETIF(eflags) ((eflags) = (eflags) | (1 << 9))
#define SETDF(eflags) ((eflags) = (eflags) | (1 << 10))
#define SETOF(eflags) ((eflags) = (eflags) | (1 << 11))
//IOPL GOES HERE
#define SETNT(eflags) ((eflags) = (eflags) | (1 << 14))
#define SETRF(eflags) ((eflags) = (eflags) | (1 << 16))
#define SETVM(eflags) ((eflags) = (eflags) | (1 << 17))
#define SETAC(eflags) ((eflags) = (eflags) | (1 << 18))
#define SETVIF(eflags) ((eflags) = (eflags) | (1 << 19))
#define SETVIP(eflags) ((eflags) = (eflags) | (1 << 20))
#define SETID(eflags) ((eflags) = (eflags) | (1 << 21))

#define UNSETCF(eflags) ((eflags) = (eflags) & ~1)
#define UNSETPF(eflags) ((eflags) = (eflags) & ~(1 << 2))
#define UNSETAF(eflags) ((eflags) = (eflags) & ~(1 << 4))
#define UNSETZF(eflags) ((eflags) = (eflags) & ~(1 << 6))
#define UNSETSF(eflags) ((eflags) = (eflags) & ~(1 << 7))
#define UNSETTF(eflags) ((eflags) = (eflags) & ~(1 << 8))
#define UNSETIF(eflags) ((eflags) = (eflags) & ~(1 << 9))
#define UNSETDF(eflags) ((eflags) = (eflags) & ~(1 << 10))
#define UNSETOF(eflags) ((eflags) = (eflags) & ~(1 << 11))
//IOPL GOES HERE
#define UNSETNT(eflags) ((eflags) = (eflags) & ~(1 << 14))
#define UNSETRF(eflags) ((eflags) = (eflags) & ~(1 << 16))
#define UNSETVM(eflags) ((eflags) = (eflags) & ~(1 << 17))
#define UNSETAC(eflags) ((eflags) = (eflags) & ~(1 << 18))
#define UNSETVIF(eflags) ((eflags) = (eflags) & ~(1 << 19))
#define UNSETVIP(eflags) ((eflags) = (eflags) & ~(1 << 20))
#define UNSETID(eflags) ((eflags) = (eflags) & ~(1 << 21))

#ifdef _WIN32
#define FORCEINLINE __forceinline
#define FASTCALL __fastcall
#define HANDLER_DECL(name) name
#define HANDLER_DEF_BEGIN(name) __declspec( naked ) int FASTCALL name( ThreadContext_t *context , VirtualDirectoryLookupTable_t *table) { \
__asm { \
  __asm push ebp \
  __asm push ebx \
  __asm push esi \
  __asm push edi \
  __asm mov ebp, esp \
  __asm sub esp, __LOCAL_SIZE \
  __asm mov context, ecx \
  __asm mov table, edx \
}

#define HANDLER_DEF_END __asm { \
  __asm lea eax, opcode_dispatch_table \
  __asm mov ecx, context \
  __asm mov edx, table \
  __asm lea ebx, [ecx]context.code \
  __asm mov ebx, [ebx] \
  __asm movzx ebx, byte ptr[ebx] \
  __asm lea eax, [eax+ebx*4] \
  __asm mov eax, [eax] \
  __asm mov esp, ebp \
  __asm pop edi \
  __asm pop esi \
  __asm pop ebx\
  __asm pop ebp \
  __asm jmp eax \
  } \
}


#else
#define FASTCALL __attribute__((fastcall))
#define FORCEINLINE __attribute__((always_inline))
#define HANDLER_DECL(name) &&name
#define HANDLER_DEF_BEGIN(name) name:
#define HANDLER_DEF_END
#endif
int interpret( RuntimeEnvironment_t *environment, ThreadNode_t *thread );
inline unsigned char *get_real_address( uint32_t virtual_address, VirtualDirectoryLookupTable_t *directory_lookup_table, unsigned int requested_access );

#endif //X86INTERPRETER_INTERPRETER_H