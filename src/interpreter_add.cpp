#ifdef _WIN32
#include "interpreter.h"
#endif


HANDLER_DEF_BEGIN(add_ar1632_imm1632_handler) {
  uint32_t prefixes = get_prefixes( &context->code, &context->eip, 0x05);
  assert(prefixes != PREFIX_INVALID );
  
  uint32_t i;
  uint32_t j;
  uint32_t k;

  if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
    i = *((uint16_t *)(&context->code[1]));
    j = context->eax & 0xFFFF;
    k = i + j;
    context->eflags = update_eflags_for_16bit_arithmetics( context->eflags, i, j, k );
    context->eax = (context->eax & 0xFFFF0000) |  (k & 0xFFFF);
    context->code+=3;
    context->eip+=3;
  } else {
    i = *((uint32_t *)(&context->code[1]));
    j = context->eax;
    k = i + j;
    context->eflags = update_eflags_for_32bit_arithmetics( context->eflags, i, j, k );
    context->eax = k;
    context->code+=5;
    context->eip+=5;
  }
}
HANDLER_DEF_END

HANDLER_DEF_BEGIN(add_al_imm8_handler) {

  assert( context->code[0] == 0x04 );
  register uint32_t i =context->eax & 0xFF;//AL
  register uint32_t j =(uint32_t)context->code[1];//imm8
  register uint32_t k = i + j;

  context->eflags = update_eflags_for_8bit_arithmetics( context->eflags, i, j, k );

  context->eax = ( context->eax & 0xFFFFFF00 ) | ( k & 0xFF );
  context->code+=2;
  context->eip+=2;
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(add_eax_imm32_handler) {
    assert( context->code[0] == 0x05 );
    uint32_t i =context->eax;//AX
    uint32_t j =*((uint32_t *)(&context->code[1]));//imm32
    uint32_t k = i + j;

    context->eflags = update_eflags_for_32bit_arithmetics( context->eflags, i, j, k );
    context->eax = k;
    context->code+=5;
    context->eip+=5;
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(add_rm8_imm8_handler) {
    uint32_t i;
    uint32_t j;
    uint32_t k;

    assert( context->code[0] == 0x80 );

    uint32_t displacement = INT32_MAX;
    uint8_t *dest;

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 0:
      {
        if( GETMOD(context->code[1]) == 3 ) {
          dest = (uint8_t *)&GETREG(context,GETRM(context->code[1]));
          displacement=1;

        }
        else {
          i = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
          dest = get_real_address( i, table, READ );
          if( dest == NULL ) {
            fprintf( stderr, "ERROR: Invalid destination address for ADD rm8, imm8\n");
            assert(0);
          }
        }

        i = *dest;
        j = context->code[1+displacement];
        k = i + j;

        context->eflags = update_eflags_for_8bit_arithmetics( context->eflags, i, j, k );

        *dest = k;
        context->eip +=displacement+2;
        context->code += displacement+2;
      }
      break;
    default:
      fprintf( stderr, "ERROR: Undefined extended opcode for CALL\n");
      assert(0);
      break;
    }
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(add_rm8_r8_handler) {
    uint32_t i;
    uint32_t j;
    uint32_t k;

    assert( context->code[0] == 0x00 );

    uint32_t displacement = INT32_MAX;
    uint8_t *dest;

    if( GETMOD(context->code[1]) == 3 ) {
      dest = (uint8_t *)&GETREG(context,GETRM(context->code[1]));
      displacement=1;
    }
    else {
      i = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
      dest = get_real_address( i, table, READ );
      if( dest == NULL ) {
        fprintf( stderr, "ERROR: Invalid destination address for ADD rm8, r8\n");
        assert(0);
      }
    }

    i = *dest;
    j = (uint8_t)GETREG(context, GETREGNUM(context->code[1]));
    k = i + j;

    context->eflags = update_eflags_for_8bit_arithmetics( context->eflags, i, j, k );

    *dest = k;
    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(add_r8_rm8_handler) {
    uint32_t i;
    uint32_t j;
    uint32_t k;

    assert( context->code[0] == 0x02 );

    uint32_t displacement = INT32_MAX;
    uint8_t *src;

    if( GETMOD(context->code[1]) == 3 ) {
      src = (uint8_t *)&GETREG(context,GETRM(context->code[1]));
      displacement=1;
    }
    else {
      i = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
      src = get_real_address( i, table, READ );
      if( src == NULL ) {
        fprintf( stderr, "ERROR: Invalid source address for ADD r8, rm8\n");
        assert(0);
      }
    }

    i = *src;
    j = (uint8_t)GETREG(context, GETREGNUM(context->code[1]));
    k = i + j;

    context->eflags = update_eflags_for_8bit_arithmetics( context->eflags, i, j, k );

    GETREG(context, GETREGNUM(context->code[1])) = (GETREG(context, GETREGNUM(context->code[1])) & 0xFFFFFF00) | k;
    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(add_rm1632_r1632_handler){
    uint32_t prefixes = get_prefixes( &context->code, &context->eip, 0x01 );
    assert(prefixes != PREFIX_INVALID );

    uint32_t i;
    uint32_t j;
    uint32_t k;

    uint32_t displacement = INT32_MAX;
    uint32_t *dest;

    if( GETMOD(context->code[1]) == 3 ) {
      dest = (uint32_t *)&GETREG(context,GETRM(context->code[1]));
      displacement=1;
    }
    else {
      i = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
      dest = (uint32_t *)get_real_address( i, table, READ );
      if( dest == NULL ) {
        fprintf( stderr, "ERROR: Invalid destination address for ADD rm1632, r1632\n");
        assert(0);
      }
    }

    if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
      i = *((uint16_t *)dest);
      j = GETREG(context, GETREGNUM(context->code[1])) & 0xFFFF;
      k = i + j;
      context->eflags = update_eflags_for_16bit_arithmetics( context->eflags, i, j, k );
      *dest = (( *dest & 0xFFFF0000 ) | ( k & 0xFFFF ));
    } else {
      i = *dest;
      j = GETREG(context, GETREGNUM(context->code[1]));
      k = i + j;
      context->eflags = update_eflags_for_32bit_arithmetics( context->eflags, i, j, k );
      *dest = k;
    }
    
    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(add_rm32_r32_handler) {
    uint32_t i;
    uint32_t j;
    uint32_t k;

    assert( context->code[0] == 0x01 );

    uint32_t displacement = INT32_MAX;
    uint32_t *dest;

    if( GETMOD(context->code[1]) == 3 ) {
      dest = (uint32_t *)&GETREG(context,GETRM(context->code[1]));
      displacement=1;
    }
    else {
      i = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
      dest = (uint32_t *)get_real_address( i, table, READ );
      if( dest == NULL ) {
        fprintf( stderr, "ERROR: Invalid destination address for ADD rm32, r32\n");
        assert(0);
      }
    }

    i = *dest;
    j = GETREG(context, GETREGNUM(context->code[1]));
    k = i + j;

    context->eflags = update_eflags_for_32bit_arithmetics( context->eflags, i, j, k );

    *dest = k;
    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(add_r1632_rm1632_handler) {

    uint32_t prefixes = get_prefixes( &context->code, &context->eip, 0x03 );
    assert( prefixes != PREFIX_INVALID );

    uint32_t i;
    uint32_t j;
    uint32_t k;

    uint32_t displacement = INT32_MAX;
    uint32_t *src;

    if( GETMOD(context->code[1]) == 3 ) {
      src = (uint32_t *)&GETREG(context,GETRM(context->code[1]));
      displacement=1;
    }
    else {
      i = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
      src = (uint32_t *)get_real_address( i, table, READ );
      if( src == NULL ) {
        fprintf( stderr, "ERROR: Invalid source address for ADD r32, rm32\n");
        assert(0);
      }
    }

    if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
      i = *((uint16_t *)src);
      j = GETREG(context, GETREGNUM(context->code[1])) & 0xFFFF;
      k = i + j;
      context->eflags = update_eflags_for_16bit_arithmetics( context->eflags, i, j, k );
      GETREG(context, GETREGNUM(context->code[1])) = (GETREG(context, GETREGNUM(context->code[1])) & 0xFFFF0000) | ( k & 0xFFFF );
    } else {
      i = *src;
      j = GETREG(context, GETREGNUM(context->code[1]));
      k = i + j;
      context->eflags = update_eflags_for_32bit_arithmetics( context->eflags, i, j, k );
      GETREG(context, GETREGNUM(context->code[1])) = k;
    }

    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(add_r32_rm32_handler) {
    uint32_t i;
    uint32_t j;
    uint32_t k;

    assert( context->code[0] == 0x03 );

    uint32_t displacement = INT32_MAX;
    uint32_t *src;

    if( GETMOD(context->code[1]) == 3 ) {
      src = (uint32_t *)&GETREG(context,GETRM(context->code[1]));
      displacement=1;
    }
    else {
      i = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
      src = (uint32_t *)get_real_address( i, table, READ );
      if( src == NULL ) {
        fprintf( stderr, "ERROR: Invalid source address for ADD r32, rm32\n");
        assert(0);
      }
    }

    i = *src;
    j = GETREG(context, GETREGNUM(context->code[1]));
    k = i + j;

    context->eflags = update_eflags_for_32bit_arithmetics( context->eflags, i, j, k );

    GETREG(context, GETREGNUM(context->code[1])) = k;
    context->eip +=displacement+1;
    context->code += displacement+1;
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(add_rm1632_imm8_handler) {
    uint32_t prefixes = get_prefixes( &context->code, &context->eip, 0x83);
    assert(prefixes != PREFIX_INVALID );

    uint32_t i;
    uint32_t j;
    uint32_t k;
    uint32_t displacement = INT32_MAX;
    uint32_t *dest;

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 0:
      {
        if( GETMOD(context->code[1]) == 3 ) {
          dest = (uint32_t *)&GETREG(context,GETRM(context->code[1]));
          displacement=1;

        }
        else {
          i = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
          dest = (uint32_t *)get_real_address( i, table, READ );
          if( dest == NULL ) {
            fprintf( stderr, "ERROR: Invalid destination address for ADD rm1632, imm8\n");
            assert(0);
          }
        }

        if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
          i = *((uint16_t *)dest);
          j = (int8_t)context->code[1+displacement];
          j &= 0xFFFF;
          k = i + j;
          context->eflags = update_eflags_for_16bit_arithmetics( context->eflags, i, j, k );
          *dest = (( *dest & 0xFFFF0000 ) | ( k & 0xFFFF ));
        }
        else {
          i = *dest;
          j = (int8_t)context->code[1+displacement];
          k = i + j;
          context->eflags = update_eflags_for_32bit_arithmetics( context->eflags, i, j, k );
          *dest = k;
          
        }
        context->eip +=displacement+2;
        context->code += displacement+2;
      }
      break;
    default:
      fprintf( stderr, "ERROR: Undefined extended opcode for ADD rm32, imm8\n");
      assert(0);
      break;
    }
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(add_rm1632_imm1632_handler) {
    uint32_t prefixes = get_prefixes( &context->code, &context->eip, 0x81);
    assert(prefixes != PREFIX_INVALID );

    uint32_t i;
    uint32_t j;
    uint32_t k;
    uint32_t displacement = INT32_MAX;
    uint32_t *dest;

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 0:
      {
        if( GETMOD(context->code[1]) == 3 ) {
          dest = (uint32_t *)&GETREG(context,GETRM(context->code[1]));
          displacement=1;

        }
        else {
          i = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
          dest = (uint32_t *)get_real_address( i, table, READ );
          if( dest == NULL ) {
            fprintf( stderr, "ERROR: Invalid destination address for ADD rm8, imm8\n");
            assert(0);
          }
        }

        if( prefixes & PREFIX_OPERAND_SIZE_OVERRIDE ) {
          i = *((uint16_t *)dest);
          j = *((uint16_t *)(&context->code[1+displacement]));
          k = i + j;
          context->eflags = update_eflags_for_16bit_arithmetics( context->eflags, i, j, k );
          *dest = (( *dest & 0xFFFF0000 ) | ( k & 0xFFFF ));
          context->eip +=displacement+3;
          context->code += displacement+3;
        } else {
          i = *dest;
          j = *((uint32_t *)(&context->code[1+displacement]));
          k = i + j;
          context->eflags = update_eflags_for_32bit_arithmetics( context->eflags, i, j, k );
          *dest = k;
          context->eip +=displacement+5;
          context->code += displacement+5;
        }
      }
      break;
    default:
      fprintf( stderr, "ERROR: Undefined extended opcode for ADD rm32, imm32\n");
      assert(0);
      break;
    }
}
HANDLER_DEF_END
  HANDLER_DEF_BEGIN(add_rm32_imm32_handler) {
    uint32_t i;
    uint32_t j;
    uint32_t k;

    assert( context->code[0] == 0x81 );

    uint32_t displacement = INT32_MAX;
    uint32_t *dest;

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 0:
      {
        if( GETMOD(context->code[1]) == 3 ) {
          dest = (uint32_t *)&GETREG(context,GETRM(context->code[1]));
          displacement=1;

        }
        else {
          i = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
          dest = (uint32_t *)get_real_address( i, table, READ );
          if( dest == NULL ) {
            fprintf( stderr, "ERROR: Invalid destination address for ADD rm8, imm8\n");
            assert(0);
          }
        }

        i = *dest;
        j = *((uint32_t *)(&context->code[1+displacement]));
        k = i + j;

        context->eflags = update_eflags_for_32bit_arithmetics( context->eflags, i, j, k );

        *dest = k;
        context->eip +=displacement+5;
        context->code += displacement+5;
      }
      break;
    default:
      fprintf( stderr, "ERROR: Undefined extended opcode for ADD rm32, imm32\n");
      assert(0);
      break;
    }
}
HANDLER_DEF_END

  HANDLER_DEF_BEGIN(add_rm32_imm8_handler) {
    uint32_t i;
    uint32_t j;
    uint32_t k;

    assert( context->code[0] == 0x83 );

    uint32_t displacement = INT32_MAX;
    uint32_t *dest;

    switch( GETEXTOPCODE(context->code[1]) ) {
    case 0:
      {
        if( GETMOD(context->code[1]) == 3 ) {
          dest = (uint32_t *)&GETREG(context,GETRM(context->code[1]));
          displacement=1;

        }
        else {
          i = resolve_rm(context->general_purpose_registers, &context->code[1], &displacement );
          dest = (uint32_t *)get_real_address( i, table, READ );
          if( dest == NULL ) {
            fprintf( stderr, "ERROR: Invalid destination address for ADD rm32, imm8\n");
            assert(0);
          }
        }

        i = *dest;
        j = (int8_t)context->code[1+displacement];
        k = i + j;

        context->eflags = update_eflags_for_32bit_arithmetics( context->eflags, i, j, k );

        *dest = k;
        context->eip +=displacement+2;
        context->code += displacement+2;
      }
      break;
    default:
      fprintf( stderr, "ERROR: Undefined extended opcode for ADD rm32, imm8\n");
      assert(0);
      break;
    }
}
HANDLER_DEF_END