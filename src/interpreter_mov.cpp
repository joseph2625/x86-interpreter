#ifdef _WIN32
#include "interpreter.h"
#endif

HANDLER_DEF_BEGIN(mov_rm8_r8_handler) {
  assert( context->code[0] == 0x88 );

  uint32_t displacement = INT32_MAX;
  uint8_t *dest;

  dest = (uint8_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);
  *dest =  GETREG(context, GETREGNUM(context->code[1]));

  context->eip +=displacement+1;
  context->code += displacement+1;
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(mov_rm1632_r1632_handler){
  uint32_t prefixes = get_prefixes( &context->code, &context->eip);
  assert( context->code[0] == 0x89 );

  uint32_t displacement = INT32_MAX;
  uint32_t *dest;

  dest = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);

  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
    *((uint16_t *)dest) = GETREG(context, GETREGNUM(context->code[1]));
  } else {
    *dest = GETREG(context, GETREGNUM(context->code[1]));
  }

  context->eip +=displacement+1;
  context->code += displacement+1;
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(mov_rm32_r32_handler) {
  assert( context->code[0] == 0x89 );

  uint32_t displacement = INT32_MAX;
  uint32_t *dest;

  dest = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);
  *dest = GETREG(context, GETREGNUM(context->code[1]));
  context->eip +=displacement+1;
  context->code += displacement+1;
}

HANDLER_DEF_END

HANDLER_DEF_BEGIN(mov_r8_rm8_handler) {

  assert( context->code[0] == 0x8A );

  uint32_t displacement = INT32_MAX;
  uint8_t *src;

  src = (uint8_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);
  switch(GETREGNUM(context->code[1])){
  case 0:
  case 1:
  case 2:
  case 3:
    *((uint8_t *)&GETREG(context, GETREGNUM(context->code[1]))) = *src;
  case 4:
  case 5:
  case 6:
  case 7:
    *(((uint8_t *)(&GETREG(context, GETREGNUM(context->code[1]) )))+1) = *src;
  }

  context->eip +=displacement+1;
  context->code += displacement+1;
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(mov_r1632_rm1632_handler) {

  uint32_t prefixes = get_prefixes( &context->code, &context->eip);
  assert( context->code[0] == 0x8B);

  uint32_t displacement = INT32_MAX;
  uint32_t *src;

  src = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);

  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
    *((uint16_t *)&GETREG(context, GETREGNUM(context->code[1]))) = *src;
  } else {
    GETREG(context, GETREGNUM(context->code[1])) = *src;
  }

  context->eip +=displacement+1;
  context->code += displacement+1;
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(mov_r32_rm32_handler) {
  assert( context->code[0] == 0x8B );

  uint32_t displacement = INT32_MAX;
  uint32_t *src;

  src = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);

  GETREG(context, GETREGNUM(context->code[1])) = *src;
  context->eip +=displacement+1;
  context->code += displacement+1;
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(mov_rm16_sreg_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip);
  assert( context->code[0] == 0x8C || context->code[0] == 0x8E);

  uint32_t displacement = INT32_MAX;
  uint16_t *srcdest;

  srcdest = (uint16_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);
  if( GETREGNUM(context->code[1]) > 5 ){
    fprintf( stderr, "ERROR: Invalid segment register\n" );
    assert(0);
  }
  if( context->code[0] == 0x8C )
    *srcdest = context->segment_registers[GETREGNUM(context->code[1])];
  else
    context->segment_registers[GETREGNUM(context->code[1])] = *srcdest;

  context->eip +=displacement+1;
  context->code += displacement+1;
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(mov_r8_imm8_handler) {
  assert( context->code[0] >= 0xB0 || context->code[0] <= 0xB7 );

  switch( context->code[0] ) {
  case 0xB0:
  case 0xB1:
  case 0xB2:
  case 0xB3:
    *((uint8_t *)(&GETREG(context, context->code[0] - 0xB0 ))) = context->code[1];
    break;
  case 0xB4:
  case 0xB5:
  case 0xB6:
  case 0xB7:
    *(((uint8_t *)(&GETREG(context, context->code[0] - 0xB0 )))+1) = context->code[1];
    break;
  }
  context->code+=2;
  context->eip+=2;
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(mov_r1632_imm1632_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip );
  assert( context->code[0] >= 0xB8 || context->code[0] <= 0xBF );

  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
    *((uint16_t *)(&GETREG(context, context->code[0] - 0xB8 ))) = *((uint16_t *)(&context->code[1]));
    context->code+=3;
    context->eip+=3;
  } else {
    GETREG(context, context->code[0] - 0xB8 ) = *((uint32_t *)(&context->code[1]));
    context->code+=5;
    context->eip+=5;
  }
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(mov_r32_imm32_handler) {
  assert( context->code[0] >= 0xB8 || context->code[0] <= 0xBF );

  GETREG(context, context->code[0] - 0xB8 ) = *((uint32_t *)(&context->code[1]));

  context->code+=5;
  context->eip+=5;
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(mov_rm8_imm8_handler) {
  assert( context->code[0] == 0xC6 );

  uint32_t displacement = INT32_MAX;
  uint8_t *dest;

  switch( GETEXTOPCODE(context->code[1]) ) {
  case 0:
    {
      dest = get_rm8( &context->code[1], context->general_purpose_registers, &displacement, table);
      *dest = context->code[1+displacement];
      context->eip +=displacement+2;
      context->code += displacement+2;
    }
    break;
  default:
    fprintf( stderr, "ERROR: Undefined extended opcode for MOV (MOV rm8, imm8\n");
    assert(0);
    break;
  }
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(mov_rm1632_imm1632_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip);
  assert( context->code[0] == 0xC7);

  uint32_t displacement = INT32_MAX;
  uint32_t *dest;

  switch( GETEXTOPCODE(context->code[1]) ) {
  case 0:
    {
      dest = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);

      if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
        *((uint16_t *)dest) = *((uint16_t *)(&context->code[1+displacement]));
        context->eip +=displacement+3;
        context->code += displacement+3;
      } else {
        *dest = *((uint32_t *)(&context->code[1+displacement]));
        context->eip +=displacement+5;
        context->code += displacement+5;
      }
    }
    break;
  default:
    fprintf( stderr, "ERROR: Undefined extended opcode for MOV (MOV rm1632, imm1632)\n");
    assert(0);
    break;
  }
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(mov_rm32_imm32_handler) {
  assert( context->code[0] == 0x81 );

  uint32_t displacement = INT32_MAX;
  uint32_t *dest;

  switch( GETEXTOPCODE(context->code[1]) ) {
  case 0:
    {
      dest = (uint32_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);
      *dest = *((uint32_t *)(&context->code[1+displacement]));
      context->eip +=displacement+5;
      context->code += displacement+5;
    }
    break;
  default:
    fprintf( stderr, "ERROR: Undefined extended opcode for MOV (MOV rm32, imm32)\n");
    assert(0);
    break;
  }
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(mov_al_moffs8_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip );
  assert( context->code[0] == 0xA0 );

  uint8_t *to_move;
  if( prefixes & PREFIX_ADDRESS_SIZE_OVERRIDE )
    to_move = get_real_address( *((uint16_t *)(&context->code[1])), table, READ );
  else
    to_move = get_real_address( *((uint32_t *)(&context->code[1])), table, READ );

  (*(uint8_t *)(&context->eax)) = *to_move;
  context->eip +=2;
  context->code +=2;
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(mov_moffs8_al_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip );
  assert( context->code[0] == 0xA2 );

  uint8_t *to_move;
  if( prefixes & PREFIX_ADDRESS_SIZE_OVERRIDE )
    to_move = get_real_address( *((uint16_t *)(&context->code[1])), table, READ );
  else
    to_move = get_real_address( *((uint32_t *)(&context->code[1])), table, READ );

  *to_move = context->eax;
  context->eip +=2;
  context->code +=2;
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(mov_ar_moffs1632_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip );
  assert( context->code[0] == 0xA1 );

  unsigned char*to_move;
  if( prefixes & PREFIX_ADDRESS_SIZE_OVERRIDE ) {
    to_move = get_real_address( *((uint16_t *)(&context->code[1])), table, READ );
    context->eip +=3;
    context->code +=3;
  } else {
    to_move = get_real_address( *((uint32_t *)(&context->code[1])), table, READ );
    context->eip +=5;
    context->code +=5;
  }

  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE )
    (*(uint16_t *)(&context->eax)) = *((uint16_t *)(to_move));
  else
    context->eax = *((uint32_t *)(to_move));
}
HANDLER_DEF_END
HANDLER_DEF_BEGIN(mov_moffs1632_ar_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip );
  assert( context->code[0] == 0xA3 );

  unsigned char*to_move;
  if( prefixes & PREFIX_ADDRESS_SIZE_OVERRIDE )
    to_move = get_real_address( *((uint16_t *)(&context->code[1])), table, READ );
  else
    to_move = get_real_address( *((uint32_t *)(&context->code[1])), table, READ );

  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
    *((uint16_t *)(to_move)) = context->eax;
    context->eip +=3;
    context->code +=3;
  }
  else {
    *((uint32_t *)(to_move)) = context->eax;
    context->eip +=5;
    context->code +=5;
  }
}
HANDLER_DEF_END