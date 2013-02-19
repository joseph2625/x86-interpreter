#include "interpreter.h"
#include "runtime.h"
#include <assert.h>

#ifndef _WIN32
#include "interpreter_opcode_dispatch_table.cpp"
#endif
HANDLER_DEF_BEGIN(prefix_handler)
#ifdef _WIN32
__asm{
lea eax, opcode_with_prefix_dispatch_table
xor esi, esi
mov ecx, context
mov edx, table
lea ebx, [ecx]context.code
mov ebx, [ebx]
prefix_loop:
inc esi
  movzx ebx, byte ptr[ebx+esi]
lea eax, [eax+ebx*4]
mov eax, [eax]
cmp eax, prefix_handler
je prefix_loop
mov esp, ebp
pop edi
pop esi
pop ebx
pop ebp
jmp eax
}
#else
  //TODO: FIX FOR GCC
goto *opcode_dispatch_table[Context->code[1]];
#endif
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(prefix_with_escape_sequence_handler)
#ifdef _WIN32
  __asm{
    lea eax, opcode_with_prefix_and_escape_sequence_dispatch_table
      xor esi, esi
      mov ecx, context
      mov edx, table
      lea ebx, [ecx]context.code
      mov ebx, [ebx]
prefix_loop:
    inc esi
      movzx ebx, byte ptr[ebx+esi]
    lea eax, [eax+ebx*4]
    mov eax, [eax]
    cmp eax, prefix_with_escape_sequence_handler
      je prefix_loop
      mov esp, ebp
      pop edi
      pop esi
      pop ebx
      pop ebp
      jmp eax
}
#else
  //TODO: FIX FOR GCC
  goto *opcode_dispatch_table[Context->code[1]];
#endif
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(escape_sequence_handler) //NO PREFIX
#ifdef _WIN32
  __asm{
    lea eax, opcode_with_escape_sequence_dispatch_table
      mov ecx, context
      mov edx, table
      lea ebx, [ecx]context.code
      mov ebx, [ebx]
      movzx ebx, byte ptr[ebx+1]
      lea eax, [eax+ebx*4]
      mov eax, [eax]
      mov esp, ebp
      pop edi
      pop esi
      pop ebx
      pop ebp
      jmp eax
}
#else
  //TODO: FIX FOR GCC
  goto *opcode_dispatch_table[Context->code[1]];
#endif
HANDLER_DEF_END

HANDLER_DEF_BEGIN(call_rel32_handler){
  assert( context->code[0] == 0xE8 );

  uint32_t value;
  unsigned char *dest;
  value = context->eip + 5 + *((uint32_t *)(&context->code[1]));
  dest = get_real_address(value, table, EXECUTE);
  if(dest == NULL) {
    fprintf( stderr, "ERROR: Invalid destination address for CALL rel32\n");
    assert(0);
  }

  context->stack-=sizeof(uint32_t);
  context->esp-=sizeof(uint32_t);
  *((uint32_t *)(context->stack)) = context->eip + 5;
  context->eip = value;
  context->code = dest;
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(push_fgs_handler) { //NO PREFIX
  uint32_t to_push;
  switch( context->code[1] ) {
  case 0xA0:
    to_push = context->fs;
    break;
  case 0xA8:
    to_push = context->gs;
    break;
  default:
    fprintf( stderr, "ERROR: Invalid opcode for PUSH fgs\n");
    assert(0);
    break;
  }
  context->stack-=sizeof(uint32_t);
  context->esp-=sizeof(uint32_t);
  *((uint32_t *)(context->stack)) = to_push;
  context->eip++;
  context->code++;
}
  HANDLER_DEF_END
//TODO: OPTIMIZE BY CREATING SEPARATE HANDLER (USING MACROS) FOR EACH PUSH r
HANDLER_DEF_BEGIN(push_r_handler) {
  uint32_t to_push;
  switch( context->code[0] ) {
  case 0x50:
  case 0x51:
  case 0x52:
  case 0x53:
  case 0x54:
  case 0x55:
  case 0x56:
  case 0x57:
    to_push = GETREG(context,context->code[0]-0x50);
    break;
  case 0x0E:
    to_push = context->cs;
    break;
  case 0x16:
    to_push = context->ss;
    break;
  case 0x1E:
    to_push = context->ds;
    break;
  case 0x06:
    to_push = context->es;
    break;
  default:
    fprintf( stderr, "ERROR: Invalid opcode for PUSH r\n");
    assert(0);
    break;
  }

  context->stack-=sizeof(uint32_t);
  context->esp-=sizeof(uint32_t);
  *((uint32_t *)(context->stack)) = to_push;
  context->eip++;
  context->code++;
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(callpush_rm32_handler) {

  assert( context->code[0] == 0xFF );

  uint32_t value;
  uint32_t displacement = INT32_MAX;
  unsigned char *dest;

  if( GETMOD(context->code[1]) ==3 ) {
    value = GETREG(context,GETRM(context->code[1]));
    displacement=1;
  } else {
    value = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
    dest = get_real_address( value, table, READ );
    if( dest == NULL ) {
      fprintf( stderr, "ERROR: Invalid referenced address for CALL/PUSH rm32\n");
      assert(0);
    }
    value = *((uint32_t *)dest);
  }

  switch( GETEXTOPCODE(context->code[1]) ) {
  case 2:
    {
      dest = get_real_address(value, table, EXECUTE);
      if(dest == NULL) {
        fprintf( stderr, "ERROR: Invalid destination address for CALL rm32\n");
        assert(0);
      }
      context->stack-=sizeof(uint32_t);
      context->esp-=sizeof(uint32_t);
      context->eip += displacement+1;
      *((uint32_t *)(context->stack)) = context->eip;
      context->eip = value;
      context->code = dest;
    }
    break;
  case 6:
    {
      context->stack-=sizeof(uint32_t);
      context->esp-=sizeof(uint32_t);
      *((uint32_t *)(context->stack)) = value;
      context->eip += displacement+1;
      context->code += displacement+1;
    }
    break;
  default:
    fprintf( stderr, "ERROR: Undefined extended opcode for 0xFF (CALL rm32,PUSH rm32)\n");
    assert(0);
    break;
  }
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(undefined_opcode_handler) {
    fprintf( stderr, "ERROR: Undefined opcode\n");
#ifndef _WIN32
    return -1;
}
#else
}
__asm {
  mov eax, 0xFFFFFFFF
    mov esp, ebp
    pop edi
    pop esi
    pop ebx
    pop ebp
    ret
}
}
#endif

#ifndef _WIN32
#include "interpreter_add.cpp"
}
#else
HANDLER_DEF_BEGIN( dispatch )
HANDLER_DEF_END
#endif



int interpret( RuntimeEnvironment_t *const environment, ThreadNode_t *const thread )
{
  return dispatch( &thread->context, &environment->directory_table );
}
