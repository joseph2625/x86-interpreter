#ifdef _WIN32
#include "interpreter.h"
#endif

HANDLER_DEF_BEGIN(push_imm8_handler){
  assert( context->code[0] == 6A );

  context->stack-=sizeof(uint32_t);
  context->esp-=sizeof(uint32_t);
  *((uint32_t *)(context->stack)) = (int32_t)context->code[1];
  context->eip+=2;
  context->code+=2;
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(push_imm32_handler){
    assert( context->code[0] == 68 );

    context->stack-=sizeof(uint32_t);
    context->esp-=sizeof(uint32_t);
    *((uint32_t *)(context->stack)) = *((uint32_t *)(&context->code[1]));
    context->eip+=5;
    context->code+=5;
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(push_imm1632_handler){
    uint32_t prefixes = get_prefixes( &context->code, &context->eip);
    assert( context->code[0] == 68 );

    if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ){
      context->stack-=sizeof(uint16_t);
      context->esp-=sizeof(uint16_t);
      *((uint16_t *)(context->stack)) = *((uint16_t *)(&context->code[1]));
      context->eip+=3;
      context->code+=3;
    } else {
      context->stack-=sizeof(uint32_t);
      context->esp-=sizeof(uint32_t);
      *((uint32_t *)(context->stack)) = *((uint32_t *)(&context->code[1]));
      context->eip+=5;
      context->code+=5;
    }
}
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
    fprintf( stderr, "ERROR: Invalid opcode for PUSH r1632\n");
    assert(0);
    break;
  }

  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
    context->stack-=sizeof(uint16_t);
    context->esp-=sizeof(uint16_t);
    *((uint16_t *)(context->stack)) = to_push & 0xFFFF;
  } else {
    context->stack-=sizeof(uint32_t);
    context->esp-=sizeof(uint32_t);
    *((uint32_t *)(context->stack)) = to_push;
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

    if( GETMOD(context->code[1]) ==3 ) {
      value = GETREG(context,GETRM(context->code[1]));
      displacement=1;
    } else {
      value = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
      dest = get_real_address( value, table, READ );
      if( dest == NULL ) {
        fprintf( stderr, "ERROR: Invalid referenced address for CALL/PUSH rm1632\n");
        assert(0);
      }
      value = *((uint32_t *)dest);
    }

    if ( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE )
      value &= 0xFFFF;

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 2:
      {
        dest = get_real_address(value, table, EXECUTE);
        if(dest == NULL) {
          fprintf( stderr, "ERROR: Invalid destination address for CALL rm1632\n");
          assert(0);
        }
        if ( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
          context->stack-=sizeof(uint16_t);
          context->esp-=sizeof(uint16_t);
          context->eip += displacement+1;
          *((uint16_t *)(context->stack)) = (uint16_t)context->eip;
        } else {
          context->stack-=sizeof(uint32_t);
          context->esp-=sizeof(uint32_t);
          context->eip += displacement+1;
          *((uint32_t *)(context->stack)) = context->eip;
        }
        context->eip = value;
        context->code = dest;
      }
      break;
    case 6:
      {
        if ( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
          context->stack-=sizeof(uint16_t);
          context->esp-=sizeof(uint16_t);
          *((uint16_t *)(context->stack)) = (uint16_t)value;
        } else {
          context->stack-=sizeof(uint32_t);
          context->esp-=sizeof(uint32_t);
          *((uint32_t *)(context->stack)) = value;
        }
        context->eip += displacement+1;
        context->code += displacement+1;
      }
      break;
    default:
      fprintf( stderr, "ERROR: Undefined extended opcode for 0xFF (CALL rm1632,PUSH rm1632)\n");
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