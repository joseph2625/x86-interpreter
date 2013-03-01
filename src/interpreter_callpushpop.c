#ifdef _WIN32
#include "interpreter.h"
#endif

HANDLER_DEF_BEGIN(push_imm8_handler){
  assert( context->code[0] == 0x6A );

  context->esp-=sizeof(uint32_t);
  uint32_t *dest = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
  *dest = (int32_t)context->code[1];
  context->eip+=2;
  context->code+=2;
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(push_imm32_handler){
    assert( context->code[0] == 0x68 );

    context->esp-=sizeof(uint32_t);
    uint32_t *dest = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
    *dest = *((uint32_t *)(&context->code[1]));
    context->eip+=5;
    context->code+=5;
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(push_imm1632_handler){
    uint32_t prefixes = get_prefixes( &context->code, &context->eip);
    assert( context->code[0] == 68 );

    if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ){
      context->esp-=sizeof(uint16_t);
      uint32_t *dest = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
      *((uint16_t *)(dest)) = *((uint16_t *)(&context->code[1]));
      context->eip+=3;
      context->code+=3;
    } else {
      context->esp-=sizeof(uint32_t);
      uint32_t *dest = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
      *dest = *((uint32_t *)(&context->code[1]));
      context->eip+=5;
      context->code+=5;
    }
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(jmp_rm1632_handler){
    uint32_t prefixes = get_prefixes( &context->code, &context->eip );
    uint32_t displacement;
    uint32_t dest_address;
    if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE )
      dest_address = (int16_t)*((int16_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table));
    else
      dest_address = *((int32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table));

    dest_address += context->eip + displacement + 1;
    unsigned char *dest = get_real_address( dest_address, table, EXECUTE, false );
    if( dest == NULL ) {
      log_message( ERROR, "Invalid destination address for JMP rm1632" );
      assert(0);
    }

    context->eip = dest_address;
    context->code = dest;
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(jmp_rm32_handler){
    uint32_t displacement;
    uint32_t dest_address = *((int32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table));
    dest_address += context->eip + displacement + 1;
    unsigned char *dest = get_real_address( dest_address, table, EXECUTE, false );
    
    context->eip = dest_address;
    context->code = dest;
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(call_rel32_handler){
  assert( context->code[0] == 0xE8 );

  uint32_t value;
  uint32_t *dest;
  value = context->eip + 5 + *((uint32_t *)(&context->code[1]));
  dest = (uint32_t *)get_real_address(value, table, EXECUTE, false);

  context->esp-=sizeof(uint32_t);
  *((uint32_t *)get_real_address( context->esp, table, WRITE, false )) = context->eip + 5;
  context->eip = value;
  context->code = (unsigned char *)dest;
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
      log_message( ERROR, "Invalid opcode for PUSH fgs" );
      assert(0);
      break;
    }
    uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
    context->esp-=sizeof(uint32_t);
    *stack = to_push;
    context->eip+=2;
    context->code+=2;
}
HANDLER_DEF_END
  //TODO: OPTIMIZE BY CREATING SEPARATE HANDLER (USING MACROS) FOR EACH PUSH r
  HANDLER_DEF_BEGIN(push_r32_handler) {
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
      log_message( ERROR, "Invalid opcode for PUSH r");
      assert(0);
      break;
    }

    context->esp-=sizeof(uint32_t);
    uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
    *stack = to_push;
    context->eip++;
    context->code++;
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(push_r1632_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip );
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
    log_message( ERROR, "Invalid opcode for PUSH r1632");
    assert(0);
    break;
  }

  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
    context->esp-=sizeof(uint16_t);
    uint16_t *stack = (uint16_t *)get_real_address( context->esp, table, WRITE, false );
    *stack = to_push;
  } else {
    uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
    context->esp-=sizeof(uint32_t);
    *stack = to_push;
  }
  context->eip++;
  context->code++;
  
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(pop_r32_handler) {

  uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
  context->esp+=sizeof(uint32_t);

  switch( context->code[0] ) {
  case 0x58:
  case 0x59:
  case 0x5A:
  case 0x5B:
  case 0x5C:
  case 0x5D:
  case 0x5E:
  case 0x5F:
    GETREG(context,context->code[0]-0x58) = *stack;
    break;
  default:
    log_message( ERROR, "Invalid opcode for POP r");
    assert(0);
    break;
  }
  context->eip++;
  context->code++;
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(pop_r1632_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip );

  uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, WRITE, false );

  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE )
    context->esp+=sizeof(uint16_t);
  else
    context->esp+=sizeof(uint32_t);

  switch( context->code[0] ) {
  case 0x58:
  case 0x59:
  case 0x5A:
  case 0x5B:
  case 0x5C:
  case 0x5D:
  case 0x5E:
  case 0x5F:
    if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE )
      *((uint16_t *)&GETREG(context,context->code[0]-0x58)) = *stack;
    else
      GETREG(context,context->code[0]-0x58) = *stack;
    break;
  default:
    log_message( ERROR, "Invalid opcode for POP r");
    assert(0);
    break;
  }

  context->eip++;
  context->code++;

}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(callpush_rm1632_handler) {
    uint32_t prefixes = get_prefixes( &context->code, &context->eip );
    assert( context->code[0] == 0xFF );

    uint32_t value;
    uint32_t displacement = INT32_MAX;
    unsigned char *dest;

    dest = (unsigned char*)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);
    value = *((uint32_t *)dest);

    if ( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE )
      value &= 0xFFFF;

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 2:
      {
        dest = (unsigned char*)get_real_address(value, table, EXECUTE, false);
        if ( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
          context->esp-=sizeof(uint16_t);
          context->eip += displacement+1;
          uint16_t *stack = (uint16_t *)get_real_address( context->esp, table, WRITE, false );
          *stack = (uint16_t)context->eip;
        } else {
          context->esp-=sizeof(uint32_t);
          context->eip += displacement+1;
          uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
          *stack = context->eip;
        }
        context->eip = value;
        context->code = dest;
      }
      break;
    case 6:
      {
        if ( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
          context->esp-=sizeof(uint16_t);
          uint16_t *stack = (uint16_t *)get_real_address( context->esp, table, WRITE, false );
          *stack = (uint16_t)value;
        } else {
          context->esp-=sizeof(uint32_t);
          uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
          *stack = value;
        }
        context->eip += displacement+1;
        context->code += displacement+1;
      }
      break;
    default:
      log_message( ERROR, "Undefined extended opcode for 0xFF (CALL rm1632,PUSH rm1632)");
      assert(0);
      break;
    }
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(callpush_rm32_handler) {

    assert( context->code[0] == 0xFF );

    uint32_t value;
    uint32_t displacement = INT32_MAX;
    unsigned char *dest;

    dest = (unsigned char*)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);
    value = *((uint32_t *)dest);

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 2:
      {
        dest = get_real_address(value, table, EXECUTE, false);
        context->esp-=sizeof(uint32_t);
        context->eip += displacement+1;
        uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
        *stack = context->eip;
        context->eip = value;
        context->code = dest;
      }
      break;
    case 6:
      {
        context->esp-=sizeof(uint32_t);
        uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
        *stack = value;
        context->eip += displacement+1;
        context->code += displacement+1;
      }
      break;
    default:
      log_message( ERROR , "Undefined extended opcode for 0xFF (CALL rm32,PUSH rm32)" );
      assert(0);
      break;
    }
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(pop_rm1632_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip );
  assert( context->code[0] == 0x8F );

  uint32_t displacement = INT32_MAX;
  uint32_t *dest;

  dest = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);

  switch( GETEXTOPCODE(context->code[1]) ) {
  case 6:
    {
      if ( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
        uint16_t *stack = (uint16_t *)get_real_address( context->esp, table, WRITE, false );
        *((uint16_t *)dest) = *stack;
        context->esp+=sizeof(uint16_t);
      } else {
        uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
        *dest = *stack;
        context->esp+=sizeof(uint32_t);
      }
      context->eip += displacement+1;
      context->code += displacement+1;
    }
    break;
  default:
    log_message( ERROR, "Undefined extended opcode for 0x8F (POP rm1632)");
    assert(0);
    break;
  }
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(pop_rm32_handler) {

  assert( context->code[0] == 0x8F );

  uint32_t displacement = INT32_MAX;
  unsigned char *dest;

  dest = (unsigned char*)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);

  switch( GETEXTOPCODE(context->code[1]) ) {
  case 6:
    {
      uint32_t *stack = (uint32_t *)get_real_address( context->esp, table, WRITE, false );
      *dest = *stack;
      context->esp+=sizeof(uint32_t);
    }
    break;
  default:
    log_message( ERROR, "Undefined extended opcode for 0x8F (POP rm32)");
    assert(0);
    break;
  }
}
HANDLER_DEF_END
