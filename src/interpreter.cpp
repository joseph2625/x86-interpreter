#include "interpreter.h"
#include "runtime.h"
#include <assert.h>
#include "syscall.h"

#ifndef _WIN32
#include "interpreter_opcode_dispatch_table.cpp"
#endif
HANDLER_DEF_BEGIN(nop_handler) {
  context->eip++;
  context->code++;
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(interrupt_handler) {
  switch(context->code[1]){
  case 0x80:
    switch(context->eax) {
    case 1:
      fprintf( stdout, "INFO: sys_exit invoked with exit code %X (%d). Exiting...\n", context->ebx, context->ebx);
#ifdef _WIN32
      __asm {
        mov ecx, context
        lea ecx, [ecx]context.ebx
        mov eax, [ecx]
        mov esp, ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
      }
#else
      return context->ebx;
#endif
      break;
    case 4:
      context->eax = handle_sys_write( table, context->ebx, context->ecx, context->edx );
      break;
    case 3:
      context->eax = handle_sys_read( table, context->ebx, context->ecx, context->edx );
      break;
    case 265:
      context->eax = handle_sys_clock_gettime( table, context->ebx, context->ecx );
      break;
    default:
      fprintf( stderr, "ERROR: Invalid system call number\n");
      break;
    }
    break;
  default:
    fprintf( stderr, "ERROR: Invalid interrupt vector number\n");
    assert(0);
  }
  context->eip+=2;
  context->code+=2;
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(ret_handler){
  assert( context->code[0] == 0xC2 || context->code[0] == 0xC3);

  uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, READ );
  context->eip = *stack;
  unsigned char*ret_address = get_real_address( context->eip, table, EXECUTE );

  if( context->code[0] == 0xC3 ) {
    context->esp+=4;
  } else {
    context->esp+=4+(*((uint16_t *)&context->code[1]));
  }
  context->code = ret_address;
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(leave1632_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip );
  assert( context->code[0] == 0xC9);

  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ){
    *((uint16_t *)&context->esp) = context->ebp;
    uint16_t *stack = (uint16_t *)get_real_address( context->esp, table, READ );
    *((uint16_t *)&context->ebp) = *stack;
    context->esp += 2;
  } else {
    context->esp = context->ebp;
    uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, READ );
    context->ebp = *stack;
    context->esp += 4;
  }

  context->eip+=1;
  context->code+=1;
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(leave32_handler) {
  assert( context->code[0] == 0xC9);
  context->esp = context->ebp;
  uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, READ );
  context->ebp = *stack;
  context->esp += 4;

  context->eip+=1;
  context->code+=1;
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(lea_r1632_rm1632_handler) {

  uint32_t prefixes = get_prefixes( &context->code, &context->eip);
  assert( context->code[0] == 0x8D);

  uint32_t displacement = INT32_MAX;
  uint32_t src;

  src = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );

  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
    *((uint16_t *)&GETREG(context, GETREGNUM(context->code[1]))) = src;
  } else {
    GETREG(context, GETREGNUM(context->code[1])) = src;
  }

  context->eip +=displacement+1;
  context->code += displacement+1;
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(lea_r32_rm32_handler) {
    assert( context->code[0] == 0x8D );

    uint32_t displacement = INT32_MAX;
    uint32_t src;
    
    src = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );

    GETREG(context, GETREGNUM(context->code[1])) = src;
    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END


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


  HANDLER_DEF_BEGIN(undefined_opcode_handler) {
    fprintf( stderr, "ERROR: Undefined opcode %02X at [%08X]\n", context->code[0], context->eip);
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
#include "interpreter_add_sub.cpp"
#include "interpreter_call_push.cpp"
#include "interpreter_mov.cpp"
}
#else
HANDLER_DEF_BEGIN( dispatch )
HANDLER_DEF_END
#endif



int interpret( RuntimeEnvironment_t *const environment, ThreadNode_t *const thread )
{
  return dispatch( &thread->context, &environment->directory_table );
}
