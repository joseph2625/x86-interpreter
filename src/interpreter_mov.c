#ifdef _MSC_VER
#include "interpreter.h"
#endif

HANDLER_DEF_BEGIN(mov_al_moffs8_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip );
  assert( context->code[0] == 0xA0 );

  uint8_t *to_move;
  if( prefixes & PREFIX_ADDRESS_SIZE_OVERRIDE ) {
    to_move = get_real_address( *((uint16_t *)(&context->code[1])), table, READ, false );
    context->eip +=3;
    context->code +=3;
  }
  else {
    to_move = get_real_address( *((uint32_t *)(&context->code[1])), table, READ, false );
    context->eip +=5;
    context->code +=5;
  }

  (*(uint8_t *)(&context->eax)) = *to_move;
  
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(mov_moffs8_al_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip );
  assert( context->code[0] == 0xA2 );

  uint8_t *to_move;
  if( prefixes & PREFIX_ADDRESS_SIZE_OVERRIDE )
    to_move = get_real_address( *((uint16_t *)(&context->code[1])), table, READ, false );
  else
    to_move = get_real_address( *((uint32_t *)(&context->code[1])), table, READ, false );

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
    to_move = get_real_address( *((uint16_t *)(&context->code[1])), table, READ, false );
    context->eip +=3;
    context->code +=3;
  } else {
    to_move = get_real_address( *((uint32_t *)(&context->code[1])), table, READ, false );
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
    to_move = get_real_address( *((uint16_t *)(&context->code[1])), table, READ, false );
  else
    to_move = get_real_address( *((uint32_t *)(&context->code[1])), table, READ, false );

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

  HANDLER_DEF_BEGIN(mov_rm16_sreg_handler) {
    assert( context->code[0] == 0x8C || context->code[0] == 0x8E);

    uint32_t displacement = INT32_MAX;
    uint16_t *srcdest;

    srcdest = (uint16_t *)get_rm( &context->code[1], context->general_purpose_registers, &displacement, table);
    if( GETREGNUM(context->code[1]) > 5 ){
      log_message( ERROR, "Invalid segment register" );
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

