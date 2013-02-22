#ifndef X86INTERPRETER_INTERPRETER_MACRO_EXPANSION_H
#define X86INTERPRETER_INTERPRETER_MACRO_EXPANSION_H

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

#define HANDLER_DEF_PROLOG __asm { \
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

#define HANDLER_DEF( cmd, bit, destname, srcname, rm, arg1, arg2, dest, incr )  __declspec( naked ) int FASTCALL cmd ## _ ## destname ## _ ## srcname ## _handler( ThreadContext_t * const context , VirtualDirectoryLookupTable_t * const table) { \
  HANDLER_DEF_PROLOG \
  { \
  uint32_t displacement = 0; \
  uint32_t *rm_field = (uint32_t *)rm; \
  perform_ ## bit ## bit_ ## cmd( *((uint ## bit ## _t *)&arg1 ), *((uint ## bit ## _t *)&arg2 ), (uint ## bit ## _t *)dest, &context->eflags ); \
  context->code+= incr + displacement; \
  context->eip+= incr + displacement; \
} \
  HANDLER_DEF_END

#define HANDLER_WITH_PREFIX_DEF( cmd, destname, srcname, rm, arg1, arg2, dest, incr_normal )  __declspec( naked ) int FASTCALL cmd ## _ ## destname ## _ ## srcname ## _handler( ThreadContext_t * const context , VirtualDirectoryLookupTable_t * const table) { \
  HANDLER_DEF_PROLOG \
  { \
   uint32_t displacement = 0; \
   uint32_t prefixes = get_prefixes( &context->code, &context->eip ); \
   uint32_t *rm_field = (uint32_t *)rm; \
   if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ){ \
    perform_16bit_ ## cmd( *((uint16_t *)&arg1 ), *((uint16_t *)&arg2), (uint16_t *)dest, &context->eflags ); \
    context->code+= displacement + incr_normal - 2; \
    context->eip+= displacement + incr_normal - 2; \
  } else { \
    perform_32bit_ ## cmd( *((uint32_t *)&arg1 ), *((uint32_t *)&arg2 ), (uint32_t *)dest, &context->eflags ); \
    context->code+= displacement + incr_normal; \
    context->eip+= displacement + incr_normal; \
  } \
  } \
  HANDLER_DEF_END

#define HANDLER_EXTOPCODE_DISPATCH( cmd, destname, srcname ) __declspec( naked ) int FASTCALL cmd ## _ ## destname ## _ ## srcname ## _handler( ThreadContext_t * const context , VirtualDirectoryLookupTable_t * const table) { __asm { \
  __asm push ebx \
  __asm lea eax, cmd ## _ ## destname ## _ ## srcname ## _dispatch_table  \
  __asm lea ebx, [ecx]context.code \
  __asm mov ebx, [ebx] \
  __asm inc ebx \
  __asm movzx ebx, byte ptr[ebx] \
  __asm shr ebx, 3 \
  __asm and ebx, 0x7 \
  __asm lea eax, [eax+ebx*4] \
  __asm mov eax, [eax] \
  __asm pop ebx \
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

#define HANDLER_DEF_AL_IMM8( cmd ) HANDLER_DEF (cmd, 8, al, imm8, NULL, context->eax, context->code[1], &context->eax, 2 )
#define HANDLER_DEF_AR1632_IMM1632( cmd ) HANDLER_WITH_PREFIX_DEF (cmd, ar1632, imm1632, NULL, context->eax, context->code[1], &context->eax, 5)
#define HANDLER_DEF_EAX_IMM32( cmd ) HANDLER_DEF (cmd, 32, eax, imm32, NULL, context->eax, context->code[1], &context->eax, 5 );

#define HANDLER_DEF_RM8_IMM8( cmd ) HANDLER_DEF (cmd, 8, rm8, imm8, get_rm8( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, context->code[1+displacement], rm_field, 2 )

#endif //X86INTERPRETER_INTERPRETER_MACRO_EXPANSION_H