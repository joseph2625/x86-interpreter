#ifndef X86INTERPRETER_INTERPRETER_MACRO_EXPANSION_H
#define X86INTERPRETER_INTERPRETER_MACRO_EXPANSION_H

#ifdef _MSC_VER
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

#ifdef _DEBUG

#define HANDLER_DEF_END { dump_thread_context( context, table ); } __asm { \
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

#endif
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

#define HANDLER_EXTOPCODE_WITH_PREFIX_DISPATCH( cmd, destname, srcname, opcode ) __declspec( naked ) int FASTCALL cmd ## _ ## destname ## _ ## srcname ## _handler( ThreadContext_t * const context , VirtualDirectoryLookupTable_t * const table) { __asm { \
  __asm push ebx \
  __asm lea eax, cmd ## _ ## destname ## _ ## srcname ## _dispatch_table  \
  __asm lea ebx, [ecx]context.code \
  __asm mov ebx, [ebx] \
  __asm increment_ptr: \
  __asm inc ebx \
  __asm cmp byte ptr[ebx], opcode \
  __asm jne increment_ptr \
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

#define HANDLER_INTERFACE( cmd, destname, srcname ) __declspec( naked ) int FASTCALL cmd ## _ ## destname ## _ ## srcname ## _handler( ThreadContext_t * const context , VirtualDirectoryLookupTable_t * const table) {
#define HANDLER_JCC_REL_INTERFACE( cmd, bit ) __declspec( naked ) int FASTCALL cmd ## _rel ## bit ## _handler( ThreadContext_t * const context , VirtualDirectoryLookupTable_t * const table) {
#define HANDLER_JCC_WITH_PREFIX_REL_INTERFACE( cmd ) __declspec( naked ) int FASTCALL cmd ## _rel1632_handler( ThreadContext_t * const context , VirtualDirectoryLookupTable_t * const table) {
#define HANDLER_SETCC_RM8_INTERFACE( cmd ) __declspec( naked ) int FASTCALL cmd ## _rm8_handler( ThreadContext_t * const context , VirtualDirectoryLookupTable_t * const table) {
#else
#define FASTCALL __attribute__((fastcall))
#define FORCEINLINE __attribute__((always_inline))
#define HANDLER_DECL(name) &&name
#define HANDLER_DEF_BEGIN(name) name:
#ifdef _DEBUG
#define HANDLER_DEF_END dump_thread_context( context, table ); goto *opcode_dispatch_table[context->code[0]];
#else
#define HANDLER_DEF_END goto *opcode_dispatch_table[context->code[0]];
#endif
#define HANDLER_INTERFACE( cmd, destname, srcname ) cmd ## _ ## destname ## _ ## srcname ## _handler:
#define HANDLER_JCC_REL_INTERFACE( cmd, bit ) cmd ## _rel ## bit ## _handler:
#define HANDLER_JCC_WITH_PREFIX_REL_INTERFACE( cmd ) cmd ## _rel1632_handler:
#define HANDLER_SETCC_RM8_INTERFACE( cmd ) cmd ## _rm8_handler:


#define HANDLER_EXTOPCODE_DISPATCH( cmd, destname, srcname ) cmd ## _ ## destname ## _ ## srcname ## _handler: { \
  goto *cmd ## _ ## destname ## _ ## srcname ## _dispatch_table[GETEXTOPCODE(context->code[1])]; \
}

#define HANDLER_EXTOPCODE_WITH_PREFIX_DISPATCH( cmd, destname, srcname, opcode ) cmd ## _ ## destname ## _ ## srcname ## _handler: { \
  int i=0; \
  while ( context->code[i] != opcode ) \
    i++; \
  goto *cmd ## _ ## destname ## _ ## srcname ## _dispatch_table[GETEXTOPCODE(context->code[i+1])]; \
}

#define HANDLER_DEF_PROLOG
#endif

#define HANDLER_DEF( cmd, bit, destname, srcname, rm, arg1, arg2, dest, incr )  HANDLER_INTERFACE( cmd, destname, srcname ) \
  HANDLER_DEF_PROLOG \
  { \
  uint32_t displacement = 0; \
  uint32_t *rm_field = (uint32_t *)rm; \
  perform_ ## bit ## bit_ ## cmd( *((uint ## bit ## _t *)&arg1 ), *((uint ## bit ## _t *)&arg2 ), (uint ## bit ## _t *)dest, &context->eflags ); \
  context->code+= incr + displacement; \
  context->eip+= incr + displacement; \
} \
  HANDLER_DEF_END

#define HANDLER_TWO_DEST_OPS_DEF( cmd, bit, destname, srcname, rm, arg1, arg2, dest1, dest2, incr )  HANDLER_INTERFACE( cmd, destname, srcname ) \
  HANDLER_DEF_PROLOG \
  { \
  uint32_t displacement = 0; \
  uint32_t *rm_field = (uint32_t *)rm; \
  perform_ ## bit ## bit_ ## cmd( *((uint ## bit ## _t *)&arg1 ), *((uint ## bit ## _t *)&arg2 ), (uint ## bit ## _t *)dest1, (uint ## bit ## _t *)dest2, &context->eflags ); \
  context->code+= incr + displacement; \
  context->eip+= incr + displacement; \
} \
  HANDLER_DEF_END

// exception for sign-extended operand
#define HANDLER_SIGN_EXT_DEF( cmd, bit, destname, srcname, rm, arg1, arg2, dest, incr ) HANDLER_INTERFACE(cmd, destname, srcname) \
  HANDLER_DEF_PROLOG \
  { \
  uint32_t displacement = 0; \
  uint32_t *rm_field = (uint32_t *)rm; \
  perform_ ## bit ## bit_ ## cmd( *((uint ## bit ## _t *)&arg1 ), arg2, (uint ## bit ## _t *)dest, &context->eflags ); \
  context->code+= incr + displacement; \
  context->eip+= incr + displacement; \
} \
  HANDLER_DEF_END


#define HANDLER_WITH_PREFIX_DEF( cmd, destname, srcname, rm, arg1, arg2, dest, incr_normal, incr_operand_size_override ) HANDLER_INTERFACE( cmd, destname, srcname ) \
  HANDLER_DEF_PROLOG \
  { \
  uint32_t displacement = 0; \
  uint32_t prefixes = get_prefixes( &context->code, &context->eip ); \
  uint32_t *rm_field = (uint32_t *)rm; \
  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ){ \
  perform_16bit_ ## cmd( *((uint16_t *)&arg1 ), *((uint16_t *)&arg2), (uint16_t *)dest, &context->eflags ); \
  context->code+= displacement + incr_operand_size_override; \
  context->eip+= displacement + incr_operand_size_override; \
  } else { \
  perform_32bit_ ## cmd( *((uint32_t *)&arg1 ), *((uint32_t *)&arg2 ), (uint32_t *)dest, &context->eflags ); \
  context->code+= displacement + incr_normal; \
  context->eip+= displacement + incr_normal; \
  } \
  } \
  HANDLER_DEF_END

#define HANDLER_TWO_DEST_OPS_WITH_PREFIX_DEF( cmd, destname, srcname, rm, arg1, arg2, dest1, dest2, incr_normal, incr_operand_size_override ) HANDLER_INTERFACE( cmd, destname, srcname ) \
  HANDLER_DEF_PROLOG \
  { \
  uint32_t displacement = 0; \
  uint32_t prefixes = get_prefixes( &context->code, &context->eip ); \
  uint32_t *rm_field = (uint32_t *)rm; \
  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ){ \
  perform_16bit_ ## cmd( *((uint16_t *)&arg1 ), *((uint16_t *)&arg2), (uint16_t *)dest1, (uint16_t *)dest2, &context->eflags ); \
  context->code+= displacement + incr_operand_size_override; \
  context->eip+= displacement + incr_operand_size_override; \
  } else { \
  perform_32bit_ ## cmd( *((uint32_t *)&arg1 ), *((uint32_t *)&arg2 ), (uint32_t *)dest1, (uint32_t *)dest2, &context->eflags ); \
  context->code+= displacement + incr_normal; \
  context->eip+= displacement + incr_normal; \
  } \
  } \
  HANDLER_DEF_END

// exception for sign-extended operand
#define HANDLER_WITH_PREFIX_SIGN_EXT_DEF( cmd, destname, srcname, rm, arg1, arg2, dest, incr_normal, incr_operand_size_override ) HANDLER_INTERFACE( cmd, destname, srcname ) \
  HANDLER_DEF_PROLOG \
  { \
  uint32_t displacement = 0; \
  uint32_t prefixes = get_prefixes( &context->code, &context->eip ); \
  uint32_t *rm_field = (uint32_t *)rm; \
  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ){ \
  perform_16bit_ ## cmd( *((uint16_t *)&arg1 ), arg2, (uint16_t *)dest, &context->eflags ); \
  context->code+= displacement + incr_operand_size_override; \
  context->eip+= displacement + incr_operand_size_override; \
  } else { \
  perform_32bit_ ## cmd( *((uint32_t *)&arg1 ), arg2, (uint32_t *)dest, &context->eflags ); \
  context->code+= displacement + incr_normal; \
  context->eip+= displacement + incr_normal; \
  } \
  } \
  HANDLER_DEF_END


#define HANDLER_DEF_AL_IMM8( cmd ) HANDLER_DEF (cmd, 8, al, imm8, NULL, context->eax, context->code[1], &context->eax, 2 )
#define HANDLER_DEF_AR1632_IMM1632( cmd ) HANDLER_WITH_PREFIX_DEF (cmd, ar1632, imm1632, NULL, context->eax, context->code[1], &context->eax, 5, 3)
#define HANDLER_DEF_EAX_IMM32( cmd ) HANDLER_DEF (cmd, 32, eax, imm32, NULL, context->eax, context->code[1], &context->eax, 5 );

#define HANDLER_DEF_RM8_IMM8( cmd ) HANDLER_DEF (cmd, 8, rm8, imm8, get_rm8( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, context->code[1+displacement], rm_field, 2 )
#define HANDLER_DEF_RM1632_IMM1632( cmd ) HANDLER_WITH_PREFIX_DEF (cmd, rm1632, imm1632, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, context->code[1+displacement], rm_field, 5, 3 )
#define HANDLER_DEF_RM32_IMM32( cmd ) HANDLER_DEF (cmd, 32, rm32, imm32, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, context->code[1+displacement], rm_field, 5 )

#define HANDLER_DEF_RM1632_IMM8( cmd ) HANDLER_WITH_PREFIX_SIGN_EXT_DEF (cmd, rm1632, imm8, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, (int8_t)context->code[1+displacement], rm_field, 2, 2 )
#define HANDLER_DEF_RM32_IMM8( cmd ) HANDLER_SIGN_EXT_DEF (cmd, 32, rm32, imm8, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, (int8_t)context->code[1+displacement], rm_field, 2 )

#define HANDLER_DEF_RM8_R8( cmd ) HANDLER_DEF (cmd, 8, rm8, r8, get_rm8( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, GETREG8BIT(context, GETREGNUM(context->code[1])), rm_field, 1)
#define HANDLER_DEF_RM1632_R1632( cmd ) HANDLER_WITH_PREFIX_DEF (cmd, rm1632, r1632, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, GETREG(context, GETREGNUM(context->code[1])), rm_field, 1, 1 )
#define HANDLER_DEF_RM32_R32( cmd ) HANDLER_DEF (cmd, 32, rm32, r32, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, GETREG(context, GETREGNUM(context->code[1])), rm_field, 1)

#define HANDLER_DEF_R8_RM8( cmd ) HANDLER_DEF (cmd, 8, r8, rm8, get_rm8( &context->code[1], context->general_purpose_registers, &displacement, table), GETREG8BIT(context, GETREGNUM(context->code[1])), *rm_field, &GETREG8BIT(context, GETREGNUM(context->code[1])), 1)
#define HANDLER_DEF_R1632_RM1632( cmd ) HANDLER_WITH_PREFIX_DEF (cmd, r1632, rm1632, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), GETREG(context, GETREGNUM(context->code[1])), *rm_field, &GETREG(context, GETREGNUM(context->code[1])), 1, 1 )
#define HANDLER_DEF_R32_RM32( cmd ) HANDLER_DEF (cmd, 32, r32, rm32, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), GETREG(context, GETREGNUM(context->code[1])), *rm_field, &GETREG(context, GETREGNUM(context->code[1])), 1)

#define HANDLER_DEF_R8_IMM8( cmd, base_opcode ) HANDLER_DEF (cmd, 8, r8, imm8, NULL, GETREG8BIT(context, context->code[0] - base_opcode), context->code[1], &GETREG8BIT(context, context->code[0] - base_opcode), 2 )
#define HANDLER_DEF_R1632_IMM1632( cmd, base_opcode ) HANDLER_WITH_PREFIX_DEF (cmd, r1632, imm1632, NULL, GETREG(context, context->code[0] - base_opcode), context->code[1], &GETREG(context, context->code[0] - base_opcode), 5, 3 )
#define HANDLER_DEF_R32_IMM32( cmd, base_opcode ) HANDLER_DEF (cmd, 32, r32, imm32, NULL, GETREG(context, context->code[0] - base_opcode), context->code[1], &GETREG(context, context->code[0] - base_opcode), 5 )

#define HANDLER_DEF_R1632_RM8_SIGN_EXT_WITH_ESCAPE_SEQUENCE( cmd ) HANDLER_WITH_PREFIX_SIGN_EXT_DEF( cmd, r1632, rm8, get_rm8( &context->code[2], context->general_purpose_registers, &displacement, table), GETREG(context, GETREGNUM(context->code[2])), (int8_t)(*rm_field), &GETREG(context, GETREGNUM(context->code[2])), 2, 2)
#define HANDLER_DEF_R32_RM8_SIGN_EXT_WITH_ESCAPE_SEQUENCE( cmd ) HANDLER_SIGN_EXT_DEF( cmd, 32, r32, rm8, get_rm8( &context->code[2], context->general_purpose_registers, &displacement, table), GETREG(context, GETREGNUM(context->code[2])), (int8_t)(*rm_field), &GETREG(context, GETREGNUM(context->code[2])), 2)
#define HANDLER_DEF_R32_RM16_SIGN_EXT_WITH_ESCAPE_SEQUENCE( cmd ) HANDLER_SIGN_EXT_DEF( cmd, 32, r32, rm16, get_rm( &context->code[2], context->general_purpose_registers, &displacement, table), GETREG(context, GETREGNUM(context->code[2])), (int16_t)(*((uint16_t *)rm_field)), &GETREG(context, GETREGNUM(context->code[2])), 2 )

#define HANDLER_DEF_R1632_RM8_WITH_ESCAPE_SEQUENCE( cmd ) HANDLER_WITH_PREFIX_DEF( cmd, r1632, rm8, get_rm8( &context->code[2], context->general_purpose_registers, &displacement, table), GETREG(context, GETREGNUM(context->code[2])), *rm_field, &GETREG(context, GETREGNUM(context->code[2])), 2, 2)
#define HANDLER_DEF_R32_RM8_WITH_ESCAPE_SEQUENCE( cmd ) HANDLER_DEF( cmd, 32, r32, rm8, get_rm8( &context->code[2], context->general_purpose_registers, &displacement, table), GETREG(context, GETREGNUM(context->code[2])), *rm_field, &GETREG(context, GETREGNUM(context->code[2])), 2)
#define HANDLER_DEF_R32_RM16_WITH_ESCAPE_SEQUENCE( cmd ) HANDLER_SIGN_EXT_DEF( cmd, 32, r32, rm16, get_rm( &context->code[2], context->general_purpose_registers, &displacement, table), GETREG(context, GETREGNUM(context->code[2])), *((uint16_t *)rm_field), &GETREG(context, GETREGNUM(context->code[2])), 2 )

#define HANDLER_TWO_DEST_OPS_DEF_R32_RM32_WITH_ESCAPE_SEQUENCE( cmd, dest1_reg, dest2_reg ) HANDLER_TWO_DEST_OPS_DEF( cmd, 32, r32, rm32, get_rm( &context->code[2], context->general_purpose_registers, &displacement, table), GETREG(context, GETREGNUM(context->code[2])), *rm_field, dest1_reg, dest2_reg, 2 )
#define HANDLER_TWO_DEST_OPS_DEF_R1632_RM1632_WITH_ESCAPE_SEQUENCE( cmd, dest1_reg, dest2_reg ) HANDLER_TWO_DEST_OPS_WITH_PREFIX_DEF( cmd, r1632, rm1632, get_rm( &context->code[2], context->general_purpose_registers, &displacement, table), GETREG(context, GETREGNUM(context->code[2])), *rm_field, dest1_reg, dest2_reg, 2, 2)

#define HANDLER_TWO_DEST_OPS_DEF_R32_RM32_IMM8( cmd, dest1_reg, dest2_reg ) HANDLER_TWO_DEST_OPS_DEF( cmd, 32, r32, rm32_imm8, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, context->code[1+displacement], dest1_reg, dest2_reg, 2 )
#define HANDLER_TWO_DEST_OPS_DEF_R1632_RM1632_IMM8( cmd, dest1_reg, dest2_reg ) HANDLER_TWO_DEST_OPS_WITH_PREFIX_DEF( cmd, r1632, rm1632_imm8, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, context->code[1+displacement], dest1_reg, dest2_reg, 2, 2)

#define HANDLER_TWO_DEST_OPS_DEF_R32_RM32_IMM32( cmd, dest1_reg, dest2_reg ) HANDLER_TWO_DEST_OPS_DEF( cmd, 32, r32, rm32_imm32, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, context->code[1+displacement], dest1_reg, dest2_reg, 5 )
#define HANDLER_TWO_DEST_OPS_DEF_R1632_RM1632_IMM1632( cmd, dest1_reg, dest2_reg ) HANDLER_TWO_DEST_OPS_WITH_PREFIX_DEF( cmd, r1632, rm1632_imm1632, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, context->code[1+displacement], dest1_reg, dest2_reg, 5, 3)

#define HANDLER_DEF_RM8( cmd ) HANDLER_SIGN_EXT_DEF( cmd, 8, rm8, , get_rm8( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, 0, rm_field, 1 )
#define HANDLER_DEF_RM1632( cmd ) HANDLER_WITH_PREFIX_SIGN_EXT_DEF( cmd, rm1632, , get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, 0, rm_field, 1, 1 )
#define HANDLER_DEF_RM32( cmd ) HANDLER_SIGN_EXT_DEF( cmd, 32, rm32, , get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, 0, rm_field, 1 )

#define HANDLER_TWO_DEST_OPS_DEF_RM8_AX( cmd, dest1_reg, dest2_reg ) HANDLER_TWO_DEST_OPS_DEF( cmd, 8, rm8, , get_rm8( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, context->eax, dest1_reg, dest2_reg, 1 )
#define HANDLER_TWO_DEST_OPS_DEF_RM1632_EAX( cmd, dest1_reg, dest2_reg ) HANDLER_TWO_DEST_OPS_WITH_PREFIX_DEF( cmd, rm1632, , get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, context->eax, dest1_reg, dest2_reg, 1, 1 )
#define HANDLER_TWO_DEST_OPS_DEF_RM32_EAX( cmd, dest1_reg, dest2_reg ) HANDLER_TWO_DEST_OPS_DEF( cmd, 32, rm32, , get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, context->eax, dest1_reg, dest2_reg, 1 )

#define HANDLER_DEF_R1632( cmd, base_opcode ) HANDLER_WITH_PREFIX_SIGN_EXT_DEF( cmd, r1632, , NULL, GETREG(context, context->code[0] - base_opcode), 0, &GETREG(context, context->code[0] - base_opcode), 1, 1 )
#define HANDLER_DEF_R32( cmd, base_opcode ) HANDLER_SIGN_EXT_DEF( cmd, 32, r32, , NULL, GETREG(context, context->code[0] - base_opcode), 0, &GETREG(context, context->code[0] - base_opcode), 1 )

#define HANDLER_DEF_RM8_1( cmd ) HANDLER_SIGN_EXT_DEF( cmd, 8, rm8, 1, get_rm8( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, 1, rm_field, 1 )
#define HANDLER_DEF_RM1632_1( cmd ) HANDLER_WITH_PREFIX_SIGN_EXT_DEF( cmd, rm1632, 1, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, 1, rm_field, 1, 1 )
#define HANDLER_DEF_RM32_1( cmd ) HANDLER_SIGN_EXT_DEF( cmd, 32, rm32, 1, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, 1, rm_field, 1 )

#define HANDLER_DEF_RM8_CL( cmd ) HANDLER_SIGN_EXT_DEF (cmd, 8, rm8, cl, get_rm8( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, (uint8_t)context->ecx, rm_field, 1 )
#define HANDLER_DEF_RM1632_CL( cmd ) HANDLER_WITH_PREFIX_SIGN_EXT_DEF (cmd, rm1632, cl, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, (uint8_t)context->ecx, rm_field, 1, 1 )
#define HANDLER_DEF_RM32_CL( cmd ) HANDLER_SIGN_EXT_DEF (cmd, 32, rm32, cl, get_rm( &context->code[1], context->general_purpose_registers, &displacement, table), *rm_field, (uint8_t)context->ecx, rm_field, 1 )

#define HANDLER_DEF_JCC_REL( cmd, cond, bit, displacement, operand_position ) HANDLER_JCC_REL_INTERFACE( cmd, bit )  \
  HANDLER_DEF_PROLOG \
  { \
  if( cond ) { \
    uint32_t dest_address = *((int ## bit ## _t *)&context->code[operand_position]) + context->eip + displacement; \
    unsigned char *dest = get_real_address( dest_address, table, EXECUTE, false ); \
    context->code = dest; \
    context->eip = dest_address; \
  } else { \
    context->code+= displacement; \
      context->eip+= displacement; \
  } \
} \
HANDLER_DEF_END

#define HANDLER_WITH_PREFIX_DEF_JCC_REL( cmd, cond, displacement_normal, displacement_override, operand_position ) HANDLER_JCC_WITH_PREFIX_REL_INTERFACE( cmd ) \
  HANDLER_DEF_PROLOG \
  { \
  uint32_t prefixes = get_prefixes( &context->code, &context->eip ); \
  if( cond ) { \
  uint32_t dest_address = (prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ? *((int16_t *)&context->code[1]) + displacement_override : *((int32_t *)&context->code[1]) + displacement_normal) + context->eip; \
  unsigned char *dest = get_real_address( dest_address, table, EXECUTE, false ); \
  context->code = dest; \
  context->eip = dest_address; \
  } else { \
  context->code+= (prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ? displacement_override : displacement_normal); \
  context->eip+= (prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ? displacement_override : displacement_normal); \
  } \
} \
  HANDLER_DEF_END

#define HANDLER_DEF_JCC_REL8( cmd, cond ) HANDLER_DEF_JCC_REL( cmd, cond, 8, 2, 1 )
#define HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( cmd, cond ) HANDLER_WITH_PREFIX_DEF_JCC_REL( cmd, cond, 6, 4, 2 )
#define HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE(cmd, cond ) HANDLER_DEF_JCC_REL( cmd, cond, 32, 6, 2)

#define HANDLER_DEF_SETCC_RM8( cmd, cond ) HANDLER_SETCC_RM8_INTERFACE( cmd ) \
  HANDLER_DEF_PROLOG \
  { \
  uint32_t displacement = 0; \
  uint8_t *dest = get_rm8( &context->code[2], context->general_purpose_registers, &displacement, table); \
  *dest = (cond ? 1 : 0); \
  context->code+=displacement+3; \
  context->eip +=displacement+3; \
} \
  HANDLER_DEF_END

#endif //X86INTERPRETER_INTERPRETER_MACRO_EXPANSION_H
