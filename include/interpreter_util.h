#ifndef X86INTERPRETER_INTERPRETER_UTIL_H
#define X86INTERPRETER_INTERPRETER_UTIL_H

#include <stdlib.h>
#include "diagnostics.h"
#ifdef _MSC_VER
void clear_stdout();
#endif

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

#define GETEXTOPCODE(modrm) ( ( modrm >> 3 ) & 0x7 )
#define GETREGNUM(modrm) GETEXTOPCODE(modrm)
#define GETRM(modrm) ( modrm & 0x7 )
#define GETREG(context, regnum) (context)->general_purpose_registers[regnum]
#define GETREG8BIT(context, regnum) ((context)->register_field[( (regnum) < 4 ) ? (regnum)*4 : ((regnum) - 4)*4 + 1] )
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

#define ISSETCF(eflags) (((eflags) & 1) != 0 )
#define ISSETPF(eflags) (((eflags) & (1 << 2))!= 0 )
#define ISSETAF(eflags) (((eflags) & (1 << 4))!= 0 )
#define ISSETZF(eflags) (((eflags) & (1 << 6))!= 0 )
#define ISSETSF(eflags) (((eflags) & (1 << 7))!= 0 )
#define ISSETTF(eflags) (((eflags) & (1 << 8))!= 0 )
#define ISSETIF(eflags) (((eflags) & (1 << 9))!= 0 )
#define ISSETDF(eflags) (((eflags) & (1 << 10))!= 0 )
#define ISSETOF(eflags) (((eflags) & (1 << 11))!= 0 )
//IOPL GOES HERE
#define ISSETNT(eflags) (((eflags) & (1 << 14))!= 0 )
#define ISSETRF(eflags) ((eflags) & (1 << 16))!= 0 )
#define ISSETVM(eflags) (((eflags) & (1 << 17))!= 0 )
#define ISSETAC(eflags) (((eflags) & (1 << 18))!= 0 )
#define ISSETVIF(eflags) (((eflags) & (1 << 19))!= 0 )
#define ISSETVIP(eflags) (((eflags) & (1 << 20))!= 0 )
#define ISSETID(eflags) (((eflags) & (1 << 21))!= 0 )

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

inline static uint32_t get_prefixes( unsigned char **code, uint32_t *eip ) {
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

#define CHECKCFADDITION if( k < i ) SETCF(eflags); else UNSETCF(eflags);
#define CHECKCFSUBTRACTION if( k > i ) SETCF(eflags); else UNSETCF(eflags);

#define CHECKOF32BITADDITION if( ((i ^ j) & 0x80000000) != 0 ) UNSETOF(eflags); else if( (i ^ k) & 0x80000000 ) SETOF(eflags); else UNSETOF(eflags);
#define CHECKOF16BITADDITION if( ((i ^ j) & 0x8000) != 0 ) UNSETOF(eflags); else if( (i ^ k) & 0x8000 ) SETOF(eflags); else UNSETOF(eflags);
#define CHECKOF8BITADDITION if( ((i ^ j) & 0x80) != 0 ) UNSETOF(eflags); else if( (i ^ k) & 0x80 ) SETOF(eflags); else UNSETOF(eflags);



#define CHECKOF32BITSUBTRACTION if( ((i ^ j) & 0x80000000) == 0 ) UNSETOF(eflags); else if( (i ^ k) & 0x80000000 ) SETOF(eflags); else UNSETOF(eflags);
#define CHECKOF16BITSUBTRACTION if( ((i ^ j) & 0x8000) == 0 ) UNSETOF(eflags); else if( (i ^ k) & 0x8000 ) SETOF(eflags); else UNSETOF(eflags);
#define CHECKOF8BITSUBTRACTION if( ((i ^ j) & 0x80) == 0 ) UNSETOF(eflags); else if( (i ^ k) & 0x80 ) SETOF(eflags); else UNSETOF(eflags);


#define CHECKSF32BIT if( (k & 0x80000000) > 0 ) SETSF(eflags); else UNSETSF(eflags);
#define CHECKSF16BIT if( (k & 0x8000) > 0 ) SETSF(eflags); else UNSETSF(eflags);
#define CHECKSF8BIT if( (k & 0x80) > 0 ) SETSF(eflags); else UNSETSF(eflags);

#define CHECKZF32BIT if( k == 0 ) SETZF(eflags); else UNSETZF(eflags);
#define CHECKZF16BIT if( (k & 0xFFFF) == 0 ) SETZF(eflags); else UNSETZF(eflags);
#define CHECKZF8BIT if( (k & 0xFF ) == 0 ) SETZF(eflags); else UNSETZF(eflags);

#define CHECKPF if( !parity_table[k & 0xFF] ) SETPF(eflags); else UNSETPF(eflags);

#define CHECKAFADDITION if( ( (i & 0xF) + (j & 0xF) ) > 15 ) SETAF(eflags); else UNSETAF(eflags); 

#define CHECKAFSUBTRACTION if( ( (int8_t)(i & 0xF) - (int8_t)(j & 0xF) ) < 0 ) SETAF(eflags); else UNSETAF(eflags);

inline static void print_access_violation( const uint32_t virtual_address, const unsigned int requested_access ) {
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

  log_message(ERROR, "Access violation while %s [%08X]", access_string, virtual_address );
  assert(0);
}

//TODO: HANDLE ADDRESS-MODE OVERRIDE PREFIX
inline static uint32_t resolve_rm( const uint32_t *general_purpose_registers, const unsigned char *code, uint32_t * const displacement) {
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
    return_value = general_purpose_registers[GETRM(code[0])];
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
      log_message( ERROR, "Illegal SIB index");
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
        log_message( ERROR, "Illegal MOD value for SIBBASE 5");
        assert(0);
        break;
      }
    }

  }

  switch( GETMOD( code[0] ) ) {
  case 0:
    break;
  case 1:
    return_value += (int8_t)code[code_displacement];
    code_displacement++;
    break;
  case 2:
    return_value += *((uint32_t *)(&code[code_displacement]));
    code_displacement+=4;
    break;
  case 3:
  default:
    log_message( ERROR, "Illegal MOD value for displacements");
    break;
  }

  *displacement = code_displacement;
  return return_value;  
}

inline static unsigned char * get_real_address( const uint32_t virtual_address, VirtualDirectoryLookupTable_t *const directory_lookup_table, const unsigned int requested_access, const bool suppress_access_violation_assertion )
{
  if( directory_lookup_table->tlb_key == ( virtual_address >> 12 ) )
    return directory_lookup_table->tlb_value + ( virtual_address & 4095 );
  else {
    VirtualPageLookupTable_t *page_lookup_table;
    if( ( page_lookup_table = directory_lookup_table->page_lookup_table[virtual_address >> 22] ) != NULL ) {
      unsigned char *frame;
      if( ( frame = page_lookup_table->frames[(virtual_address >> 12) & 1023]) != NULL ) {
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
inline static void *get_rm( unsigned char *modrm_pointer, uint32_t *const registers, uint32_t *displacement, VirtualDirectoryLookupTable_t *table ){
  void *dest;
  if( GETMOD(modrm_pointer[0]) == 3 ) {
    dest = &registers[GETRM(modrm_pointer[0])];
    *displacement=1;
  }
  else {
    dest = get_real_address( resolve_rm(registers, modrm_pointer, displacement ), table, READ, false );
    if( dest == NULL ) {
      assert(0);
    }
  }

  return dest;
}
//REQUIRE SEPARATE FUNCTION DUE TO DIFFERENT REGISTER ACCESS MODE IN 8BIT (FOR AL/AH; EBP,ESP,EDI,ESI ARE NOT 8BIT ACCESSIBLE)
inline static uint8_t *get_rm8( unsigned char *modrm_pointer, uint32_t *const registers, uint32_t *displacement, VirtualDirectoryLookupTable_t *table ){
  uint8_t *dest;
  if( GETMOD(modrm_pointer[0]) == 3 ) { 
    switch(GETRM(modrm_pointer[0])){
    case 0:
    case 1:
    case 2:
    case 3:
      dest = (uint8_t *)&registers[GETRM(modrm_pointer[0])];
      break;
    case 4:
    case 5:
    case 6:
    case 7:
      dest = (uint8_t *)&registers[GETRM(modrm_pointer[0])-4];
      dest++;
      break;
    }
    *displacement=1;
  }
  else {
    dest = get_real_address( resolve_rm(registers, modrm_pointer, displacement ), table, READ, false );
    if( dest == NULL ) {
      assert(0);
    }
  }

  return dest;
}

inline static void dump_thread_context( ThreadContext_t *context, VirtualDirectoryLookupTable_t *table ){
#ifdef _MSC_VER //as per Microsoft KB 99261 (http://support.microsoft.com/kb/99261)
  clear_stdout();
#else
  printf("\033[2J\033[1;1H");
#endif
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
