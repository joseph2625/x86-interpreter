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
#include "interpreter_add_sub.cpp"
#include "interpreter_call_push.cpp"
}
#else
HANDLER_DEF_BEGIN( dispatch )
HANDLER_DEF_END
#endif



int interpret( RuntimeEnvironment_t *const environment, ThreadNode_t *const thread )
{
  return dispatch( &thread->context, &environment->directory_table );
}
