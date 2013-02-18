#ifndef X86INTERPRETER_INTERPRETER_H
#define X86INTERPRETER_INTERPRETER_H

#include "runtime.h"
#include <assert.h>
#define GETEXTOPCODE(modrm) ( ( modrm >> 3 ) & 0x7 )
#define GETRM(modrm) ( modrm & 0x7 )
#define GETREG(context, regnum) (context)->general_purpose_registers[regnum]
#define GETMOD(modrm) ( modrm >> 6 )
#define GETSS(sib) ( sib >> 6 )
#define GETSIBINDEX(sib) ( ( sib >> 3 ) & 0x7 )
#define GETSIBBASE(sib) ( sib & 0x7 )

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
#define HANDLER_DEF_BEGIN(name) __declspec( naked ) int FASTCALL name( ThreadContext_t * const context , VirtualDirectoryLookupTable_t * const table) { \
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
int interpret( RuntimeEnvironment_t *const environment, ThreadNode_t *const thread );

inline void print_access_violation( const uint32_t virtual_address, const unsigned int requested_access ) {
  char *access_string;
  switch( requested_access ) {
  case READ | WRITE:
    access_string = "reading/writing";
    break;
  case READ:
    access_string = "reading";
    break;
  case WRITE:
    access_string = "writing";
    break;
  case EXECUTE:
    access_string = "executing";
    break;
  default:
    access_string = "accessing";
    break;
  }

  fprintf( stderr, "ERROR: Access violation while %s [%08X]", access_string, virtual_address );
  assert(0);
}
inline uint32_t resolve_rm( const uint32_t *general_purpose_registers, const unsigned char *code, uint32_t * const displacement) {
  uint32_t return_value;

  if( ( code[0] & 0xC7 ) == 0x5 ) { //disp32 exception
    *displacement=5;
    return *((uint32_t *)(&code[1]));
  } 

  unsigned int code_displacement = 1;

  switch( GETRM(code[0]) ){
  case 0:
  case 1:
  case 2:
  case 3:
  case 5:
  case 6:
  case 7:
    return_value = general_purpose_registers[GETMOD(code[0])];
    break;
  case 4:
    code_displacement++;
    switch( GETSIBINDEX(code[1]) ) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
      return_value = general_purpose_registers[GETSIBINDEX(code[1])];
      break;
    case 4:
    default: //error check
      fprintf( stderr, "ERROR: Illegal SIB index\n");
      assert(0);
      break;
    }
    return_value *= 1 << GETSS(code[1]);
    switch( GETSIBBASE( code[1] ) ) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
      return_value += general_purpose_registers[GETSIBBASE( code[1] )];
      break;
    case 5:
      switch( GETMOD( code[0] ) ) {
      case 0:
        break;
      case 1:
      case 2:
        return_value += general_purpose_registers[REGISTER_INDEX_EBP];
        break;
      case 3:
        fprintf( stderr, "ERROR: Illegal MOD value for SIBBASE 5\n");
        assert(0);
        break;
      }
    }

  }

  switch( GETMOD( code[0] ) ) {
  case 0:
    break;
  case 1:
    return_value += code[code_displacement];
    code_displacement++;
    break;
  case 2:
    return_value += *((uint32_t *)(&code[code_displacement]));
    code_displacement+=4;
    break;
  case 3:
  default:
    fprintf( stderr, "ERROR: Illegal MOD value for displacements\n");
    break;
  }

  *displacement = code_displacement;
  return return_value;  
}
inline unsigned char * get_real_address( const uint32_t virtual_address, VirtualDirectoryLookupTable_t *const directory_lookup_table, const unsigned int requested_access )
{
  if( directory_lookup_table->tlb_key == ( virtual_address >> 12 ) )
    return directory_lookup_table->tlb_value + ( virtual_address & 4095 );
  else {
    VirtualPageLookupTable_t *page_lookup_table;
    if( page_lookup_table = directory_lookup_table->page_lookup_table[virtual_address >> 22] ) {
      unsigned char *frame;
      if( frame = page_lookup_table->frames[(virtual_address >> 12) & 1023]) {
        directory_lookup_table->tlb_key = ( virtual_address >> 12 );
        directory_lookup_table->tlb_value = frame;
        return frame + ( virtual_address & 4095 );
      }
    }
  }

  print_access_violation( virtual_address, requested_access );
  return NULL;
}


#endif //X86INTERPRETER_INTERPRETER_H