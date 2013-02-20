#ifdef _WIN32
#include "interpreter.h"
#endif

HANDLER_DEF_BEGIN(addsubcmp_ar1632_imm1632_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip);
  uint32_t dummy;
  assert( context->code[0] == 0x05 || context->code[0] == 0x2D || context->code[0] == 0x3D );

  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
    perform_16bit_arithmetic( context->eax, *((uint16_t *)(&context->code[1])), context->code[0] == 0x3D ? (uint16_t *)&dummy : (uint16_t *)&context->eax, &context->eflags, ( context->code[0] == 0x05 ));
    context->code+=3;
    context->eip+=3;
  } else {
    perform_32bit_arithmetic( context->eax, *((uint32_t *)(&context->code[1])), context->code[0] == 0x3D ? &dummy : &context->eax, &context->eflags, ( context->code[0] == 0x05 ));
    context->code+=5;
    context->eip+=5;
  }
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(addsubcmp_al_imm8_handler) {

  uint8_t dummy;
  assert( context->code[0] == 0x04 || context->code[0] == 0x2C || context->code[0] == 0x3C );

  perform_8bit_arithmetic( context->eax, context->code[1], context->code[0] == 0x3C ? &dummy : (uint8_t *)&context->eax, &context->eflags, (context->code[0] == 0x04));
  context->code+=2;
  context->eip+=2;
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(addsubcmp_eax_imm32_handler) {
    uint32_t dummy;
    assert( context->code[0] == 0x05 || context->code[0] == 0x2D || context->code[0] == 0x3D );
    perform_32bit_arithmetic( context->eax, *((uint32_t *)(&context->code[1])), context->code[0] == 0x3D ? &dummy : &context->eax, &context->eflags, ( context->code[0] == 0x05 ));
    context->code+=5;
    context->eip+=5;
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(addsubcmp_rm8_imm8_handler) {
    assert( context->code[0] == 0x80 );

    uint32_t displacement = INT32_MAX;
    uint8_t dummy;
    uint8_t *dest;

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 0:
    case 5:
    case 7:
      {
        dest = get_rm8( &context->code[1], context->general_purpose_registers, &displacement, table);
        perform_8bit_arithmetic( *dest, context->code[1+displacement], GETEXTOPCODE(context->code[1]) == 7 ? &dummy : dest, &context->eflags, GETEXTOPCODE(context->code[1]) == 0);
        context->eip +=displacement+2;
        context->code += displacement+2;
      }
      break;
    default:
      fprintf( stderr, "ERROR: Undefined extended opcode for ADD/SUB/CMP (ADD/SUB/CMP rm8, imm8\n");
      assert(0);
      break;
    }
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(addsubcmp_rm8_r8_handler) {
    assert( context->code[0] == 0x00 || context->code[0] == 0x28 || context->code[0] == 0x38 );

    uint32_t displacement = INT32_MAX;
    uint8_t dummy;
    uint8_t *dest;

    dest = get_rm8( &context->code[1], context->general_purpose_registers, &displacement, table);
    perform_8bit_arithmetic( *dest, GETREG(context, GETREGNUM(context->code[1])), context->code[0] == 0x38 ? &dummy : dest, &context->eflags, context->code[0] == 0x00 );

    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(addsubcmp_r8_rm8_handler) {

    assert( context->code[0] == 0x02 || context->code[0] == 0x2A || context->code[0] == 0x3A );

    uint32_t displacement = INT32_MAX;
    uint8_t dummy;
    uint8_t *src;

    src = get_rm8( &context->code[1], context->general_purpose_registers, &displacement, table);
    perform_8bit_arithmetic( GETREG(context, GETREGNUM(context->code[1])), *src, context->code[0] == 0x3A ? &dummy : (uint8_t *)&GETREG(context, GETREGNUM(context->code[1])), &context->eflags, context->code[0] == 0x02 );
    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(addsubcmp_rm1632_r1632_handler){
    uint32_t prefixes = get_prefixes( &context->code, &context->eip);
    assert( context->code[0] == 0x01 || context->code[0] == 0x29 || context->code[0] == 0x39 );

    uint32_t displacement = INT32_MAX;
    uint32_t dummy;
    uint32_t *dest;

    dest = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);

    if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
      perform_16bit_arithmetic( *dest, GETREG(context, GETREGNUM(context->code[1])), context->code[0] == 0x39 ? (uint16_t *)&dummy : (uint16_t *)dest, &context->eflags, context->code[0] == 0x01);
    } else {
      perform_32bit_arithmetic( *dest, GETREG(context, GETREGNUM(context->code[1])), context->code[0] == 0x39 ? &dummy : dest, &context->eflags, context->code[0] == 0x01);
    }
    
    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(addsubcmp_rm32_r32_handler) {
    assert( context->code[0] == 0x01 || context->code[0] == 0x29 || context->code[0] == 0x39 );

    uint32_t displacement = INT32_MAX;
    uint32_t dummy;
    uint32_t *dest;

    dest = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);
    perform_32bit_arithmetic( *dest, GETREG(context, GETREGNUM(context->code[1])), context->code[0] == 0x39 ? &dummy : dest, &context->eflags, context->code[0] == 0x01);
    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(addsubcmp_r1632_rm1632_handler) {

    uint32_t prefixes = get_prefixes( &context->code, &context->eip);
    assert( context->code[0] == 0x03 || context->code[0] == 0x2B || context->code[0] == 0x3B);

    uint32_t dummy;
    uint32_t displacement = INT32_MAX;
    uint32_t *src;

    src = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);

    if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
      perform_16bit_arithmetic( GETREG(context, GETREGNUM(context->code[1])), *src, context->code[0] == 0x3B ? (uint16_t *)&dummy : (uint16_t *)&GETREG(context, GETREGNUM(context->code[1])), &context->eflags, context->code[0] == 0x03);
    } else {
      perform_32bit_arithmetic( GETREG(context, GETREGNUM(context->code[1])), *src, context->code[0] == 0x3B ? &dummy : &GETREG(context, GETREGNUM(context->code[1])), &context->eflags, context->code[0] == 0x03);
    }

    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(addsubcmp_r32_rm32_handler) {
    assert( context->code[0] == 0x03 || context->code[0] == 0x2B || context->code[0] == 0x39);

    uint32_t displacement = INT32_MAX;
    uint32_t dummy;
    uint32_t *src;

    src = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);

    perform_32bit_arithmetic( GETREG(context, GETREGNUM(context->code[1])), *src, context->code[0] == 0x3B ? &dummy : &GETREG(context, GETREGNUM(context->code[1])), &context->eflags, context->code[0] == 0x03);
    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(addsubcmp_rm1632_imm8_handler) {
    uint32_t prefixes = get_prefixes( &context->code, &context->eip);
    assert( context->code[0] == 0x83);

    uint32_t displacement = INT32_MAX;
    uint32_t dummy;
    uint32_t *dest;

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 0:
    case 5:
    case 7:
      {
        dest = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);

        if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
          perform_16bit_arithmetic( *dest, (int8_t)context->code[1+displacement], GETEXTOPCODE(context->code[1]) == 7 ? (uint16_t *)&dummy : (uint16_t *)dest, &context->eflags, GETEXTOPCODE(context->code[1]) == 0 );
        }
        else {
          perform_32bit_arithmetic( *dest, (int8_t)context->code[1+displacement], GETEXTOPCODE(context->code[1]) == 7 ? &dummy : dest, &context->eflags, GETEXTOPCODE(context->code[1]) == 0 );
        }
        context->eip +=displacement+2;
        context->code += displacement+2;
      }
      break;
    default:
      fprintf( stderr, "ERROR: Undefined extended opcode for ADD/SUB/CMP (ADD/SUB/CMP rm32, imm8)\n");
      assert(0);
      break;
    }
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(addsubcmp_rm1632_imm1632_handler) {
    uint32_t prefixes = get_prefixes( &context->code, &context->eip);
    assert( context->code[0] == 0x81);

    uint32_t displacement = INT32_MAX;
    uint32_t dummy;
    uint32_t *dest;

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 0:
    case 5:
    case 7:
      {
        dest = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);

        if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
          perform_16bit_arithmetic( *dest, *((uint16_t *)(&context->code[1+displacement])), GETEXTOPCODE(context->code[1]) == 7 ? (uint16_t *)&dummy : (uint16_t *)dest, &context->eflags, GETEXTOPCODE(context->code[1]) == 0 );
          context->eip +=displacement+3;
          context->code += displacement+3;
        } else {
          perform_32bit_arithmetic( *dest, *((uint32_t *)(&context->code[1+displacement])), GETEXTOPCODE(context->code[1]) == 7 ? &dummy : dest, &context->eflags, GETEXTOPCODE(context->code[1]) == 0 );
          context->eip +=displacement+5;
          context->code += displacement+5;
        }
      }
      break;
    default:
      fprintf( stderr, "ERROR: Undefined extended opcode for ADD/SUB/CMP (ADD/SUB/CMP rm1632, imm1632)\n");
      assert(0);
      break;
    }
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(addsubcmp_rm32_imm32_handler) {
    assert( context->code[0] == 0x81 );

    uint32_t displacement = INT32_MAX;
    uint32_t dummy;
    uint32_t *dest;

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 0:
    case 5:
      {
        dest = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);
        perform_32bit_arithmetic( *dest, *((uint32_t *)(&context->code[1+displacement])), GETEXTOPCODE(context->code[1]) == 7 ? &dummy : dest, &context->eflags, GETEXTOPCODE(context->code[1]) == 0 );
        context->eip +=displacement+5;
        context->code += displacement+5;
      }
      break;
    default:
      fprintf( stderr, "ERROR: Undefined extended opcode for ADD/SUB/CMP (ADD/SUB/CMP rm32, imm32)\n");
      assert(0);
      break;
    }
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(addsubcmp_rm32_imm8_handler) {
    assert( context->code[0] == 0x83 );

    uint32_t displacement = INT32_MAX;
    uint32_t dummy;
    uint32_t *dest;

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 0:
    case 5:
    case 7:
      {
        dest = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);
        perform_32bit_arithmetic( *dest, (int8_t)context->code[1+displacement], GETEXTOPCODE(context->code[1]) == 7 ? &dummy : dest, &context->eflags, GETEXTOPCODE(context->code[1]) == 0 );
        context->eip +=displacement+2;
        context->code += displacement+2;
      }
      break;
    default:
      fprintf( stderr, "ERROR: Undefined extended opcode for ADD/SUB/CMP (ADD/SUB/CMP rm32, imm8)\n");
      assert(0);
      break;
    }
}
HANDLER_DEF_END