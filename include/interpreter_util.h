#ifndef X86INTERPRETER_INTERPRETER_UTIL_H
#define X86INTERPRETER_INTERPRETER_UTIL_H

#include <stdlib.h>

#define PREFIX_LOCK (1 << 0)
#define PREFIX_REPN (1 << 1)
#define PREFIX_REPZ (1 << 2)
#define PREFIX_GROUP1 (PREFIX_LOCK | PREFIX_REPN | PREFIX_REPZ)

#define PREFIX_CS_OVERRIDE (1 << 3)
#define PREFIX_SS_OVERRIDE (1 << 4)
#define PREFIX_DS_OVERRIDE (1 << 5)
#define PREFIX_ES_OVERRIDE (1 << 6)
#define PREFIX_FS_OVERRIDE (1 << 7)
#define PREFIX_GS_OVERRIDE (1 << 8)
#define PREFIX_GROUP2_NONE_BRANCH (PREFIX_CS_OVERRIDE | PREFIX_SS_OVERRIDE | PREFIX_DS_OVERRIDE | PREFIX_ES_OVERRIDE | PREFIX_FS_OVERRIDE | PREFIX_GS_OVERRIDE)

#define PREFIX_BRANCH_NOT_TAKEN PREFIX_CS_OVERRIDE
#define PREFIX_BRANCH_TAKEN PREFIX_SS_OVERRIDE
#define PREFIX_GROUP2_BRANCH (PREFIX_BRANCH_NOT_TAKEN | PREFIX_BRANCH_TAKEN)

#define PREFIX_GROUP2 (PREFIX_GROUP2_BRANCH | PREFIX_GROUP2_NONE_BRANCH)

#define PREFIX_OPERAND_SIZE_OVERRIDE (1 << 11)
#define PREFIX_GROUP3 (PREFIX_OPERAND_SIZE_OVERRIDE)

#define PREFIX_ADDRESS_SIZE_OVERRIDE (1 << 12)
#define PREFIX_GROUP4 (PREFIX_ADDRESS_SIZE_OVERRIDE)

#define PREFIX_INVALID UINT32_MAX



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

#define HANDLER_DEF_END { dump_thread_context( context, table );}__asm { \
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
#define HANDLER_DEF_END goto *opcode_dispatch_table[Context->code[0]];
#endif

#define GETEXTOPCODE(modrm) ( ( modrm >> 3 ) & 0x7 )
#define GETREGNUM(modrm) GETEXTOPCODE(modrm)
#define GETRM(modrm) ( modrm & 0x7 )
#define GETREG(context, regnum) (context)->general_purpose_registers[regnum]
#define GETMOD(modrm) ( modrm >> 6 )
#define GETSS(sib) ( sib >> 6 )
#define GETSIBINDEX(sib) ( ( sib >> 3 ) & 0x7 )
#define GETSIBBASE(sib) ( sib & 0x7 )

//Compute parity by lookup table
//Borrowed the code from http://graphics.stanford.edu/~seander/bithacks.html
static const bool parity_table[256] = 
{
#   define P2(n) n, n^1, n^1, n
#   define P4(n) P2(n), P2(n^1), P2(n^1), P2(n)
#   define P6(n) P4(n), P4(n^1), P4(n^1), P4(n)
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

inline uint32_t get_prefixes( unsigned char **code, uint32_t *eip ) {
  uint32_t prefixes = 0;
  do{
    switch((*code)[0]) {
    case 0xF0:
      prefixes |= PREFIX_LOCK;
      break;
    case 0xF2:
      prefixes |= PREFIX_REPN;
      break;
    case 0xF3:
      prefixes |= PREFIX_REPZ;
      break;
    case 0x2E:
      prefixes |= PREFIX_CS_OVERRIDE;
      break;
    case 0x36:
      prefixes |= PREFIX_SS_OVERRIDE;
      break;
    case 0x3E:
      prefixes |= PREFIX_DS_OVERRIDE;
      break;
    case 0x26:
      prefixes |= PREFIX_ES_OVERRIDE;
      break;
    case 0x64:
      prefixes |= PREFIX_FS_OVERRIDE;
      break;
    case 0x65:
      prefixes |= PREFIX_GS_OVERRIDE;
      break;
    case 0x66:
      prefixes |= PREFIX_OPERAND_SIZE_OVERRIDE;
      break;
    case 0x67:
      prefixes |= PREFIX_ADDRESS_SIZE_OVERRIDE;
      break;
    default:
      return prefixes;
      break;
    }
    (*eip)++;
    (*code)++;
  } while( true );
}

inline uint32_t update_eflags_for_32bit_arithmetics( uint32_t eflags, const uint32_t i, const uint32_t j, const uint32_t k ){

  if( k < i )
    SETCF(eflags);
  else
    UNSETCF(eflags);

  if( (i ^ j) & 0x80000000 )
    UNSETOF(eflags);
  else if( (i ^ k) & 0x80000000 )
    SETOF(eflags);
  else
    UNSETOF(eflags);

  if( (k & 0x80000000) > 0 )
    SETSF(eflags);
  else
    UNSETSF(eflags);

  if( k == 0 )
    SETZF(eflags);
  else
    UNSETZF(eflags);

  UNSETAF(eflags);

  if( !parity_table[k & 0xFF] )
    SETPF(eflags);
  else
    UNSETPF(eflags);

  if( ( (i & 0xF) + (j & 0xF) ) > 15 )
    SETAF(eflags);
  else
    UNSETAF(eflags);

  return eflags;
}
inline uint32_t update_eflags_for_16bit_arithmetics( uint32_t eflags, const uint32_t i, const uint32_t j, const uint32_t k ){

  if( k >> 16 )
    SETCF(eflags);
  else
    UNSETCF(eflags);

  if( (i ^ j) & 0x8000 )
    UNSETOF(eflags);
  else if( (i ^ k) & 0x8000 )
    SETOF(eflags);
  else
    UNSETOF(eflags);

  if( (k & 0x8000) > 0 )
    SETSF(eflags);
  else
    UNSETSF(eflags);

  if( k == 0 )
    SETZF(eflags);
  else
    UNSETZF(eflags);

  UNSETAF(eflags);

  if( !parity_table[k & 0xFF] )
    SETPF(eflags);
  else
    UNSETPF(eflags);

  if( ( (i & 0xF) + (j & 0xF) ) > 15 )
    SETAF(eflags);
  else
    UNSETAF(eflags);

  return eflags;
}
inline uint32_t update_eflags_for_8bit_arithmetics( uint32_t eflags, const uint32_t i, const uint32_t j, const uint32_t k ){

  if( k >> 8 )
    SETCF(eflags);
  else
    UNSETCF(eflags);

  if( (i ^ j) & 0x80 )
    UNSETOF(eflags);
  else if( (i ^ k) & 0x80 )
    SETOF(eflags);
  else
    UNSETOF(eflags);

  if( (k & 0x80) > 0 )
    SETSF(eflags);
  else
    UNSETSF(eflags);

  if( k == 0 )
    SETZF(eflags);
  else
    UNSETZF(eflags);

  UNSETAF(eflags);

  if( !parity_table[k & 0xFF] )
    SETPF(eflags);
  else
    UNSETPF(eflags);

  if( ( (i & 0xF) + (j & 0xF) ) > 15 )
    SETAF(eflags);
  else
    UNSETAF(eflags);

  return eflags;
}
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

//TODO: HANDLE ADDRESS-MODE OVERRIDE PREFIX
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
      return_value = 0;
      break;
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
inline unsigned char * get_real_address( const uint32_t virtual_address, VirtualDirectoryLookupTable_t *const directory_lookup_table, const unsigned int requested_access, const bool suppress_access_violation_assertion = false )
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
  if( !suppress_access_violation_assertion )
    print_access_violation( virtual_address, requested_access );
  return NULL;
}

inline void dump_thread_context( ThreadContext_t *context, VirtualDirectoryLookupTable_t *table ){
  system("cls");
  printf( "EAX %08X\nECX %08X\nEDX %08X\nEBX %08X\nESP %08X\nEBP %08X\nESI %08X\nEDI %08X\n\nEIP %08X\n\nC %d ES %04X\nP %d CS %04X\nA %d SS %04X\nZ %d DS %04X\nS %d FS %04X\nT %d GS %04X\nD %d\nO %d\n\nEFL %08X\n\n",
    context->general_purpose_registers[0],
    context->general_purpose_registers[1],
    context->general_purpose_registers[2],
    context->general_purpose_registers[3],
    context->general_purpose_registers[4],
    context->general_purpose_registers[5],
    context->general_purpose_registers[6],
    context->general_purpose_registers[7],
    context->eip,
    ((context->eflags & 1) > 0 ? 1: 0),
    context->es,
    ((context->eflags & 1 << 2) > 0 ? 1: 0),
    context->cs,
    ((context->eflags & 1 << 4) > 0 ? 1: 0),
    context->ss,
    ((context->eflags & 1 << 6) > 0 ? 1: 0),
    context->ds,
    ((context->eflags & 1 << 7) > 0 ? 1: 0),
    context->fs,
    ((context->eflags & 1 << 8) > 0 ? 1: 0),
    context->gs,
    ((context->eflags & 1 << 10) > 0 ? 1: 0),
    ((context->eflags & 1 << 11) > 0 ? 1: 0),
    context->eflags
    );
  for( int i = 0; i < 20; i++ ) {
    uint32_t *dest = (uint32_t *)get_real_address( context->esp + i * 4, table, READ, true );
    if( dest )
      printf( "%08X %08X\n", context->esp + i * 4, *dest );
  }
  printf( "Press any key to continue...\n" );
  getc(stdin);

}

#endif //X86INTERPRETER_INTERPRETER_UTIL_H