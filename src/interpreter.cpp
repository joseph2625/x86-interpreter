#include "interpreter.h"
#include "runtime.h"
#include <assert.h>

#ifndef _WIN32
void FASTCALL dispatch( ThreadContext_t *context, VirtualDirectoryLookupTable_t *table ){
static void *opcode_dispatch_table[256] = {
#else

int FASTCALL undefined_opcode_handler( ThreadContext_t *, VirtualDirectoryLookupTable_t *);
int FASTCALL add_al_imm8_handler( ThreadContext_t *, VirtualDirectoryLookupTable_t * );
int FASTCALL call_rm32_handler( ThreadContext_t *, VirtualDirectoryLookupTable_t * );

int (FASTCALL * opcode_dispatch_table[256]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *) = {
#endif
  HANDLER_DECL(undefined_opcode_handler), //0x00
  HANDLER_DECL(undefined_opcode_handler), //0x01
  HANDLER_DECL(undefined_opcode_handler), //0x02
  HANDLER_DECL(undefined_opcode_handler), //0x03
  HANDLER_DECL(add_al_imm8_handler), //0x04
  HANDLER_DECL(undefined_opcode_handler), //0x05
  HANDLER_DECL(undefined_opcode_handler), //0x06
  HANDLER_DECL(undefined_opcode_handler), //0x07
  HANDLER_DECL(undefined_opcode_handler), //0x08
  HANDLER_DECL(undefined_opcode_handler), //0x09
  HANDLER_DECL(undefined_opcode_handler), //0x0A
  HANDLER_DECL(undefined_opcode_handler), //0x0B
  HANDLER_DECL(undefined_opcode_handler), //0x0C
  HANDLER_DECL(undefined_opcode_handler), //0x0D
  HANDLER_DECL(undefined_opcode_handler), //0x0E
  HANDLER_DECL(undefined_opcode_handler), //0x0F
  HANDLER_DECL(undefined_opcode_handler), //0x10
  HANDLER_DECL(undefined_opcode_handler), //0x11
  HANDLER_DECL(undefined_opcode_handler), //0x12
  HANDLER_DECL(undefined_opcode_handler), //0x13
  HANDLER_DECL(undefined_opcode_handler), //0x14
  HANDLER_DECL(undefined_opcode_handler), //0x15
  HANDLER_DECL(undefined_opcode_handler), //0x16
  HANDLER_DECL(undefined_opcode_handler), //0x17
  HANDLER_DECL(undefined_opcode_handler), //0x18
  HANDLER_DECL(undefined_opcode_handler), //0x19
  HANDLER_DECL(undefined_opcode_handler), //0x1A
  HANDLER_DECL(undefined_opcode_handler), //0x1B
  HANDLER_DECL(undefined_opcode_handler), //0x1C
  HANDLER_DECL(undefined_opcode_handler), //0x1D
  HANDLER_DECL(undefined_opcode_handler), //0x1E
  HANDLER_DECL(undefined_opcode_handler), //0x1F
  HANDLER_DECL(undefined_opcode_handler), //0x20
  HANDLER_DECL(undefined_opcode_handler), //0x21
  HANDLER_DECL(undefined_opcode_handler), //0x22
  HANDLER_DECL(undefined_opcode_handler), //0x23
  HANDLER_DECL(undefined_opcode_handler), //0x24
  HANDLER_DECL(undefined_opcode_handler), //0x25
  HANDLER_DECL(undefined_opcode_handler), //0x26
  HANDLER_DECL(undefined_opcode_handler), //0x27
  HANDLER_DECL(undefined_opcode_handler), //0x28
  HANDLER_DECL(undefined_opcode_handler), //0x29
  HANDLER_DECL(undefined_opcode_handler), //0x2A
  HANDLER_DECL(undefined_opcode_handler), //0x2B
  HANDLER_DECL(undefined_opcode_handler), //0x2C
  HANDLER_DECL(undefined_opcode_handler), //0x2D
  HANDLER_DECL(undefined_opcode_handler), //0x2E
  HANDLER_DECL(undefined_opcode_handler), //0x2F
  HANDLER_DECL(undefined_opcode_handler), //0x30
  HANDLER_DECL(undefined_opcode_handler), //0x31
  HANDLER_DECL(undefined_opcode_handler), //0x32
  HANDLER_DECL(undefined_opcode_handler), //0x33
  HANDLER_DECL(undefined_opcode_handler), //0x34
  HANDLER_DECL(undefined_opcode_handler), //0x35
  HANDLER_DECL(undefined_opcode_handler), //0x36
  HANDLER_DECL(undefined_opcode_handler), //0x37
  HANDLER_DECL(undefined_opcode_handler), //0x38
  HANDLER_DECL(undefined_opcode_handler), //0x39
  HANDLER_DECL(undefined_opcode_handler), //0x3A
  HANDLER_DECL(undefined_opcode_handler), //0x3B
  HANDLER_DECL(undefined_opcode_handler), //0x3C
  HANDLER_DECL(undefined_opcode_handler), //0x3D
  HANDLER_DECL(undefined_opcode_handler), //0x3E
  HANDLER_DECL(undefined_opcode_handler), //0x3F
  HANDLER_DECL(undefined_opcode_handler), //0x40
  HANDLER_DECL(undefined_opcode_handler), //0x41
  HANDLER_DECL(undefined_opcode_handler), //0x42
  HANDLER_DECL(undefined_opcode_handler), //0x43
  HANDLER_DECL(undefined_opcode_handler), //0x44
  HANDLER_DECL(undefined_opcode_handler), //0x45
  HANDLER_DECL(undefined_opcode_handler), //0x46
  HANDLER_DECL(undefined_opcode_handler), //0x47
  HANDLER_DECL(undefined_opcode_handler), //0x48
  HANDLER_DECL(undefined_opcode_handler), //0x49
  HANDLER_DECL(undefined_opcode_handler), //0x4A
  HANDLER_DECL(undefined_opcode_handler), //0x4B
  HANDLER_DECL(undefined_opcode_handler), //0x4C
  HANDLER_DECL(undefined_opcode_handler), //0x4D
  HANDLER_DECL(undefined_opcode_handler), //0x4E
  HANDLER_DECL(undefined_opcode_handler), //0x4F
  HANDLER_DECL(undefined_opcode_handler), //0x50
  HANDLER_DECL(undefined_opcode_handler), //0x51
  HANDLER_DECL(undefined_opcode_handler), //0x52
  HANDLER_DECL(undefined_opcode_handler), //0x53
  HANDLER_DECL(undefined_opcode_handler), //0x54
  HANDLER_DECL(undefined_opcode_handler), //0x55
  HANDLER_DECL(undefined_opcode_handler), //0x56
  HANDLER_DECL(undefined_opcode_handler), //0x57
  HANDLER_DECL(undefined_opcode_handler), //0x58
  HANDLER_DECL(undefined_opcode_handler), //0x59
  HANDLER_DECL(undefined_opcode_handler), //0x5A
  HANDLER_DECL(undefined_opcode_handler), //0x5B
  HANDLER_DECL(undefined_opcode_handler), //0x5C
  HANDLER_DECL(undefined_opcode_handler), //0x5D
  HANDLER_DECL(undefined_opcode_handler), //0x5E
  HANDLER_DECL(undefined_opcode_handler), //0x5F
  HANDLER_DECL(undefined_opcode_handler), //0x60
  HANDLER_DECL(undefined_opcode_handler), //0x61
  HANDLER_DECL(undefined_opcode_handler), //0x62
  HANDLER_DECL(undefined_opcode_handler), //0x63
  HANDLER_DECL(undefined_opcode_handler), //0x64
  HANDLER_DECL(undefined_opcode_handler), //0x65
  HANDLER_DECL(undefined_opcode_handler), //0x66
  HANDLER_DECL(undefined_opcode_handler), //0x67
  HANDLER_DECL(undefined_opcode_handler), //0x68
  HANDLER_DECL(undefined_opcode_handler), //0x69
  HANDLER_DECL(undefined_opcode_handler), //0x6A
  HANDLER_DECL(undefined_opcode_handler), //0x6B
  HANDLER_DECL(undefined_opcode_handler), //0x6C
  HANDLER_DECL(undefined_opcode_handler), //0x6D
  HANDLER_DECL(undefined_opcode_handler), //0x6E
  HANDLER_DECL(undefined_opcode_handler), //0x6F
  HANDLER_DECL(undefined_opcode_handler), //0x70
  HANDLER_DECL(undefined_opcode_handler), //0x71
  HANDLER_DECL(undefined_opcode_handler), //0x72
  HANDLER_DECL(undefined_opcode_handler), //0x73
  HANDLER_DECL(undefined_opcode_handler), //0x74
  HANDLER_DECL(undefined_opcode_handler), //0x75
  HANDLER_DECL(undefined_opcode_handler), //0x76
  HANDLER_DECL(undefined_opcode_handler), //0x77
  HANDLER_DECL(undefined_opcode_handler), //0x78
  HANDLER_DECL(undefined_opcode_handler), //0x79
  HANDLER_DECL(undefined_opcode_handler), //0x7A
  HANDLER_DECL(undefined_opcode_handler), //0x7B
  HANDLER_DECL(undefined_opcode_handler), //0x7C
  HANDLER_DECL(undefined_opcode_handler), //0x7D
  HANDLER_DECL(undefined_opcode_handler), //0x7E
  HANDLER_DECL(undefined_opcode_handler), //0x7F
  HANDLER_DECL(undefined_opcode_handler), //0x80
  HANDLER_DECL(undefined_opcode_handler), //0x81
  HANDLER_DECL(undefined_opcode_handler), //0x82
  HANDLER_DECL(undefined_opcode_handler), //0x83
  HANDLER_DECL(undefined_opcode_handler), //0x84
  HANDLER_DECL(undefined_opcode_handler), //0x85
  HANDLER_DECL(undefined_opcode_handler), //0x86
  HANDLER_DECL(undefined_opcode_handler), //0x87
  HANDLER_DECL(undefined_opcode_handler), //0x88
  HANDLER_DECL(undefined_opcode_handler), //0x89
  HANDLER_DECL(undefined_opcode_handler), //0x8A
  HANDLER_DECL(undefined_opcode_handler), //0x8B
  HANDLER_DECL(undefined_opcode_handler), //0x8C
  HANDLER_DECL(undefined_opcode_handler), //0x8D
  HANDLER_DECL(undefined_opcode_handler), //0x8E
  HANDLER_DECL(undefined_opcode_handler), //0x8F
  HANDLER_DECL(undefined_opcode_handler), //0x90
  HANDLER_DECL(undefined_opcode_handler), //0x91
  HANDLER_DECL(undefined_opcode_handler), //0x92
  HANDLER_DECL(undefined_opcode_handler), //0x93
  HANDLER_DECL(undefined_opcode_handler), //0x94
  HANDLER_DECL(undefined_opcode_handler), //0x95
  HANDLER_DECL(undefined_opcode_handler), //0x96
  HANDLER_DECL(undefined_opcode_handler), //0x97
  HANDLER_DECL(undefined_opcode_handler), //0x98
  HANDLER_DECL(undefined_opcode_handler), //0x99
  HANDLER_DECL(undefined_opcode_handler), //0x9A
  HANDLER_DECL(undefined_opcode_handler), //0x9B
  HANDLER_DECL(undefined_opcode_handler), //0x9C
  HANDLER_DECL(undefined_opcode_handler), //0x9D
  HANDLER_DECL(undefined_opcode_handler), //0x9E
  HANDLER_DECL(undefined_opcode_handler), //0x9F
  HANDLER_DECL(undefined_opcode_handler), //0xA0
  HANDLER_DECL(undefined_opcode_handler), //0xA1
  HANDLER_DECL(undefined_opcode_handler), //0xA2
  HANDLER_DECL(undefined_opcode_handler), //0xA3
  HANDLER_DECL(undefined_opcode_handler), //0xA4
  HANDLER_DECL(undefined_opcode_handler), //0xA5
  HANDLER_DECL(undefined_opcode_handler), //0xA6
  HANDLER_DECL(undefined_opcode_handler), //0xA7
  HANDLER_DECL(undefined_opcode_handler), //0xA8
  HANDLER_DECL(undefined_opcode_handler), //0xA9
  HANDLER_DECL(undefined_opcode_handler), //0xAA
  HANDLER_DECL(undefined_opcode_handler), //0xAB
  HANDLER_DECL(undefined_opcode_handler), //0xAC
  HANDLER_DECL(undefined_opcode_handler), //0xAD
  HANDLER_DECL(undefined_opcode_handler), //0xAE
  HANDLER_DECL(undefined_opcode_handler), //0xAF
  HANDLER_DECL(undefined_opcode_handler), //0xB0
  HANDLER_DECL(undefined_opcode_handler), //0xB1
  HANDLER_DECL(undefined_opcode_handler), //0xB2
  HANDLER_DECL(undefined_opcode_handler), //0xB3
  HANDLER_DECL(undefined_opcode_handler), //0xB4
  HANDLER_DECL(undefined_opcode_handler), //0xB5
  HANDLER_DECL(undefined_opcode_handler), //0xB6
  HANDLER_DECL(undefined_opcode_handler), //0xB7
  HANDLER_DECL(undefined_opcode_handler), //0xB8
  HANDLER_DECL(undefined_opcode_handler), //0xB9
  HANDLER_DECL(undefined_opcode_handler), //0xBA
  HANDLER_DECL(undefined_opcode_handler), //0xBB
  HANDLER_DECL(undefined_opcode_handler), //0xBC
  HANDLER_DECL(undefined_opcode_handler), //0xBD
  HANDLER_DECL(undefined_opcode_handler), //0xBE
  HANDLER_DECL(undefined_opcode_handler), //0xBF
  HANDLER_DECL(undefined_opcode_handler), //0xC0
  HANDLER_DECL(undefined_opcode_handler), //0xC1
  HANDLER_DECL(undefined_opcode_handler), //0xC2
  HANDLER_DECL(undefined_opcode_handler), //0xC3
  HANDLER_DECL(undefined_opcode_handler), //0xC4
  HANDLER_DECL(undefined_opcode_handler), //0xC5
  HANDLER_DECL(undefined_opcode_handler), //0xC6
  HANDLER_DECL(undefined_opcode_handler), //0xC7
  HANDLER_DECL(undefined_opcode_handler), //0xC8
  HANDLER_DECL(undefined_opcode_handler), //0xC9
  HANDLER_DECL(undefined_opcode_handler), //0xCA
  HANDLER_DECL(undefined_opcode_handler), //0xCB
  HANDLER_DECL(undefined_opcode_handler), //0xCC
  HANDLER_DECL(undefined_opcode_handler), //0xCD
  HANDLER_DECL(undefined_opcode_handler), //0xCE
  HANDLER_DECL(undefined_opcode_handler), //0xCF
  HANDLER_DECL(undefined_opcode_handler), //0xD0
  HANDLER_DECL(undefined_opcode_handler), //0xD1
  HANDLER_DECL(undefined_opcode_handler), //0xD2
  HANDLER_DECL(undefined_opcode_handler), //0xD3
  HANDLER_DECL(undefined_opcode_handler), //0xD4
  HANDLER_DECL(undefined_opcode_handler), //0xD5
  HANDLER_DECL(undefined_opcode_handler), //0xD6
  HANDLER_DECL(undefined_opcode_handler), //0xD7
  HANDLER_DECL(undefined_opcode_handler), //0xD8
  HANDLER_DECL(undefined_opcode_handler), //0xD9
  HANDLER_DECL(undefined_opcode_handler), //0xDA
  HANDLER_DECL(undefined_opcode_handler), //0xDB
  HANDLER_DECL(undefined_opcode_handler), //0xDC
  HANDLER_DECL(undefined_opcode_handler), //0xDD
  HANDLER_DECL(undefined_opcode_handler), //0xDE
  HANDLER_DECL(undefined_opcode_handler), //0xDF
  HANDLER_DECL(undefined_opcode_handler), //0xE0
  HANDLER_DECL(undefined_opcode_handler), //0xE1
  HANDLER_DECL(undefined_opcode_handler), //0xE2
  HANDLER_DECL(undefined_opcode_handler), //0xE3
  HANDLER_DECL(undefined_opcode_handler), //0xE4
  HANDLER_DECL(undefined_opcode_handler), //0xE5
  HANDLER_DECL(undefined_opcode_handler), //0xE6
  HANDLER_DECL(undefined_opcode_handler), //0xE7
  HANDLER_DECL(undefined_opcode_handler), //0xE8
  HANDLER_DECL(undefined_opcode_handler), //0xE9
  HANDLER_DECL(undefined_opcode_handler), //0xEA
  HANDLER_DECL(undefined_opcode_handler), //0xEB
  HANDLER_DECL(undefined_opcode_handler), //0xEC
  HANDLER_DECL(undefined_opcode_handler), //0xED
  HANDLER_DECL(undefined_opcode_handler), //0xEE
  HANDLER_DECL(undefined_opcode_handler), //0xEF
  HANDLER_DECL(undefined_opcode_handler), //0xF0
  HANDLER_DECL(undefined_opcode_handler), //0xF1
  HANDLER_DECL(undefined_opcode_handler), //0xF2
  HANDLER_DECL(undefined_opcode_handler), //0xF3
  HANDLER_DECL(undefined_opcode_handler), //0xF4
  HANDLER_DECL(undefined_opcode_handler), //0xF5
  HANDLER_DECL(undefined_opcode_handler), //0xF6
  HANDLER_DECL(undefined_opcode_handler), //0xF7
  HANDLER_DECL(undefined_opcode_handler), //0xF8
  HANDLER_DECL(undefined_opcode_handler), //0xF9
  HANDLER_DECL(undefined_opcode_handler), //0xFA
  HANDLER_DECL(undefined_opcode_handler), //0xFB
  HANDLER_DECL(undefined_opcode_handler), //0xFC
  HANDLER_DECL(undefined_opcode_handler), //0xFD
  HANDLER_DECL(undefined_opcode_handler), //0xFE
  HANDLER_DECL(call_rm32_handler), //0xFF
};

#ifndef _WIN32
goto *opcode_dispatch_table[Context->code[0]];
#endif

HANDLER_DEF_BEGIN(call_rm32_handler) {
  register uint32_t i = context->code[1];
  register uint32_t j;
  unsigned char *dest;
  switch( GETEXTOPCODE(i) ) {
  case 2:
    {
      j = GETREG(context,GETRM(i));
      dest = get_real_address(j, table, EXECUTE);
      if(dest == NULL) {
        fprintf( stderr, "ERROR: Invalid destination address for CALL\n");
        assert(0);
      }
      context->stack-=sizeof(uint32_t);
      context->esp-=sizeof(uint32_t);
      *((unsigned int *)(context->stack)) = context->eip+2;
      context->eip = j;
      context->code = dest;
    }
    break;
  default:
    fprintf( stderr, "ERROR: Undefined extended opcode for CALL\n");
    assert(0);
    break;
  }
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(undefined_opcode_handler) {
  fprintf( stderr, "ERROR: Undefined opcode\n");
}
__asm mov eax, 0xFFFFFFFF;
__asm ret;
HANDLER_DEF_END

HANDLER_DEF_BEGIN(add_al_imm8_handler) {
    register uint32_t i =context->eax & 0xFF;//AL
    register uint32_t j =(uint32_t)context->code[1];//imm8
    register uint32_t k = i + j;

    if( k >> 8 )
      SETCF(context->eflags);
    else
      UNSETCF(context->eflags);
    
    if( (i ^ j) & 0x80 )
      UNSETOF(context->eflags);
    else if( (i ^ k) & 0x80 )
      SETOF(context->eflags);
    else
      UNSETOF(context->eflags);

    if( (k & 0x80) > 0 )
      SETSF(context->eflags);
    else
      UNSETSF(context->eflags);

    if( k == 0 )
      SETZF(context->eflags);
    else
      UNSETZF(context->eflags);

    UNSETAF(context->eflags);

    k &= 0x80;
    if( parity_table[k] )
      SETPF(context->eflags);
    else
      UNSETPF(context->eflags);

    context->eax = ( context->eax & 0xFFFFFF00 ) | k;
    context->code+=2;
    context->eip+=2;
  }
HANDLER_DEF_END

#ifndef _WIN32
}
#else
HANDLER_DEF_BEGIN( dispatch )
HANDLER_DEF_END
#endif

void print_access_violation( uint32_t virtual_address, unsigned int requested_access ) {
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

inline unsigned char * get_real_address( uint32_t virtual_address, VirtualDirectoryLookupTable_t *directory_lookup_table, unsigned int requested_access )
{
  if( directory_lookup_table->tlb_key == ( virtual_address >> 12 ) )
    return directory_lookup_table->tlb_value + ( virtual_address & 4095 );
  else {
    register VirtualPageLookupTable_t *page_lookup_table;
    if( page_lookup_table = directory_lookup_table->page_lookup_table[virtual_address >> 22] ) {
      register unsigned char *frame;
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


int interpret( RuntimeEnvironment_t *environment, ThreadNode_t *thread )
{
  return dispatch( &thread->context, &environment->directory_table );
}
