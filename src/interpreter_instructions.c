#ifdef _MSC_VER
#include "interpreter.h"
#endif

HANDLER_DEF_AR1632_IMM1632( add );
HANDLER_DEF_AR1632_IMM1632( sub );
HANDLER_DEF_AR1632_IMM1632( cmp );
HANDLER_DEF_AR1632_IMM1632( and );
HANDLER_DEF_AR1632_IMM1632( xor );
HANDLER_DEF_AR1632_IMM1632( or );
HANDLER_DEF_AR1632_IMM1632( test );

HANDLER_DEF_AL_IMM8( add );
HANDLER_DEF_AL_IMM8( sub );
HANDLER_DEF_AL_IMM8( cmp );
HANDLER_DEF_AL_IMM8( and );
HANDLER_DEF_AL_IMM8( xor );
HANDLER_DEF_AL_IMM8( or );
HANDLER_DEF_AL_IMM8( test );

HANDLER_DEF_EAX_IMM32( add );
HANDLER_DEF_EAX_IMM32( sub );
HANDLER_DEF_EAX_IMM32( cmp );
HANDLER_DEF_EAX_IMM32( and );
HANDLER_DEF_EAX_IMM32( xor );
HANDLER_DEF_EAX_IMM32( or );
HANDLER_DEF_EAX_IMM32( test );

HANDLER_DEF_RM8_IMM8( add );
HANDLER_DEF_RM8_IMM8( sub );
HANDLER_DEF_RM8_IMM8( cmp );
HANDLER_DEF_RM8_IMM8( and );
HANDLER_DEF_RM8_IMM8( xor );
HANDLER_DEF_RM8_IMM8( or );
HANDLER_DEF_RM8_IMM8( test );

HANDLER_EXTOPCODE_DISPATCH( addsubcmpandxoror, rm8, imm8 );
HANDLER_EXTOPCODE_DISPATCH( testnotneg, rm8, optimm8 );

HANDLER_DEF_RM1632_IMM1632( add );
HANDLER_DEF_RM1632_IMM1632( sub );
HANDLER_DEF_RM1632_IMM1632( cmp );
HANDLER_DEF_RM1632_IMM1632( and );
HANDLER_DEF_RM1632_IMM1632( xor );
HANDLER_DEF_RM1632_IMM1632( or );
HANDLER_DEF_RM1632_IMM1632( test );

HANDLER_EXTOPCODE_WITH_PREFIX_DISPATCH( addsubcmpandxoror, rm1632, imm1632, 0x81 );
HANDLER_EXTOPCODE_WITH_PREFIX_DISPATCH( testnotneg, rm1632, optimm1632, 0xF7 );

HANDLER_DEF_RM32_IMM32( add );
HANDLER_DEF_RM32_IMM32( sub );
HANDLER_DEF_RM32_IMM32( cmp );
HANDLER_DEF_RM32_IMM32( and );
HANDLER_DEF_RM32_IMM32( xor );
HANDLER_DEF_RM32_IMM32( or );
HANDLER_DEF_RM32_IMM32( test );

HANDLER_EXTOPCODE_DISPATCH( addsubcmpandxoror, rm32, imm32 );
HANDLER_EXTOPCODE_DISPATCH( testnotneg, rm32, optimm32 );

HANDLER_DEF_RM1632_IMM8( add );
HANDLER_DEF_RM1632_IMM8( sub );
HANDLER_DEF_RM1632_IMM8( cmp );
HANDLER_DEF_RM1632_IMM8( and );
HANDLER_DEF_RM1632_IMM8( xor );
HANDLER_DEF_RM1632_IMM8( or );

HANDLER_EXTOPCODE_WITH_PREFIX_DISPATCH( addsubcmpandxoror, rm1632, imm8, 0x83 );

HANDLER_DEF_RM32_IMM8( add );
HANDLER_DEF_RM32_IMM8( sub );
HANDLER_DEF_RM32_IMM8( cmp );
HANDLER_DEF_RM32_IMM8( and );
HANDLER_DEF_RM32_IMM8( xor );
HANDLER_DEF_RM32_IMM8( or );

HANDLER_EXTOPCODE_DISPATCH( addsubcmpandxoror, rm32, imm8 );

HANDLER_DEF_RM8_R8( add );
HANDLER_DEF_RM8_R8( sub );
HANDLER_DEF_RM8_R8( cmp );
HANDLER_DEF_RM8_R8( and );
HANDLER_DEF_RM8_R8( xor );
HANDLER_DEF_RM8_R8( or );
HANDLER_DEF_RM8_R8( test );

HANDLER_DEF_RM1632_R1632( add );
HANDLER_DEF_RM1632_R1632( sub );
HANDLER_DEF_RM1632_R1632( cmp );
HANDLER_DEF_RM1632_R1632( and );
HANDLER_DEF_RM1632_R1632( xor );
HANDLER_DEF_RM1632_R1632( or );
HANDLER_DEF_RM1632_R1632( test );

HANDLER_DEF_RM32_R32( add );
HANDLER_DEF_RM32_R32( sub );
HANDLER_DEF_RM32_R32( cmp );
HANDLER_DEF_RM32_R32( and );
HANDLER_DEF_RM32_R32( xor );
HANDLER_DEF_RM32_R32( or );
HANDLER_DEF_RM32_R32( test );

HANDLER_DEF_R8_RM8( add );
HANDLER_DEF_R8_RM8( sub );
HANDLER_DEF_R8_RM8( cmp );
HANDLER_DEF_R8_RM8( and );
HANDLER_DEF_R8_RM8( xor );
HANDLER_DEF_R8_RM8( or );

HANDLER_DEF_R1632_RM1632( add );
HANDLER_DEF_R1632_RM1632( sub );
HANDLER_DEF_R1632_RM1632( cmp );
HANDLER_DEF_R1632_RM1632( and );
HANDLER_DEF_R1632_RM1632( xor );
HANDLER_DEF_R1632_RM1632( or );

HANDLER_DEF_R32_RM32( add );
HANDLER_DEF_R32_RM32( sub );
HANDLER_DEF_R32_RM32( cmp );
HANDLER_DEF_R32_RM32( and );
HANDLER_DEF_R32_RM32( xor );
HANDLER_DEF_R32_RM32( or );

HANDLER_DEF_RM8( dec );
HANDLER_DEF_RM8( inc );
HANDLER_DEF_RM8( not );
HANDLER_DEF_RM8( neg );

HANDLER_EXTOPCODE_DISPATCH( decinc, rm8,  );

HANDLER_DEF_RM1632( dec );
HANDLER_DEF_RM1632( inc );
HANDLER_DEF_RM1632( not );
HANDLER_DEF_RM1632( neg );

HANDLER_EXTOPCODE_WITH_PREFIX_DISPATCH( callpushdecincjmp, rm1632, , 0xFF );

HANDLER_DEF_RM32( dec );
HANDLER_DEF_RM32( inc );
HANDLER_DEF_RM32( not );
HANDLER_DEF_RM32( neg );

HANDLER_EXTOPCODE_DISPATCH( callpushdecincjmp, rm32,  );

HANDLER_DEF_R1632( dec, 0x48 );
HANDLER_DEF_R1632( inc, 0x40 );

HANDLER_DEF_R32( dec, 0x48 );
HANDLER_DEF_R32( inc, 0x40 );

HANDLER_DEF_RM8_1( sal );
HANDLER_DEF_RM8_1( shl );
HANDLER_DEF_RM8_1( sar );
HANDLER_DEF_RM8_1( shr );

HANDLER_EXTOPCODE_DISPATCH( salshlsarshr, rm8, 1 );

HANDLER_DEF_RM8_CL( sal );
HANDLER_DEF_RM8_CL( shl );
HANDLER_DEF_RM8_CL( sar );
HANDLER_DEF_RM8_CL( shr );

HANDLER_EXTOPCODE_DISPATCH( salshlsarshr, rm8, cl );

HANDLER_DEF_RM8_IMM8( sal );
HANDLER_DEF_RM8_IMM8( shl );
HANDLER_DEF_RM8_IMM8( sar );
HANDLER_DEF_RM8_IMM8( shr );

HANDLER_EXTOPCODE_DISPATCH( salshlsarshr, rm8, imm8 );

HANDLER_DEF_RM1632_1( sal );
HANDLER_DEF_RM1632_1( shl );
HANDLER_DEF_RM1632_1( sar );
HANDLER_DEF_RM1632_1( shr );

HANDLER_EXTOPCODE_WITH_PREFIX_DISPATCH( salshlsarshr, rm1632, 1, 0xD1 );

HANDLER_DEF_RM1632_CL( sal );
HANDLER_DEF_RM1632_CL( shl );
HANDLER_DEF_RM1632_CL( sar );
HANDLER_DEF_RM1632_CL( shr );

HANDLER_EXTOPCODE_WITH_PREFIX_DISPATCH( salshlsarshr, rm1632, cl, 0xD3 );

HANDLER_DEF_RM1632_IMM8( sal );
HANDLER_DEF_RM1632_IMM8( shl );
HANDLER_DEF_RM1632_IMM8( sar );
HANDLER_DEF_RM1632_IMM8( shr );

HANDLER_EXTOPCODE_WITH_PREFIX_DISPATCH( salshlsarshr, rm1632, imm8, 0xC1 );

HANDLER_DEF_RM32_1( sal );
HANDLER_DEF_RM32_1( shl );
HANDLER_DEF_RM32_1( sar );
HANDLER_DEF_RM32_1( shr );

HANDLER_EXTOPCODE_DISPATCH( salshlsarshr, rm32, 1 );

HANDLER_DEF_RM32_CL( sal );
HANDLER_DEF_RM32_CL( shl );
HANDLER_DEF_RM32_CL( sar );
HANDLER_DEF_RM32_CL( shr );

HANDLER_EXTOPCODE_DISPATCH( salshlsarshr, rm32, cl );

HANDLER_DEF_RM32_IMM8( sal );
HANDLER_DEF_RM32_IMM8( shl );
HANDLER_DEF_RM32_IMM8( sar );
HANDLER_DEF_RM32_IMM8( shr );

HANDLER_EXTOPCODE_DISPATCH( salshlsarshr, rm32, imm8 );

HANDLER_DEF_JCC_REL8( ja, !ISSETCF(context->eflags) && !ISSETZF(context->eflags) );
HANDLER_DEF_JCC_REL8( jae, !ISSETCF(context->eflags) );
HANDLER_DEF_JCC_REL8( jb, ISSETCF(context->eflags) );
HANDLER_DEF_JCC_REL8( jbe, ISSETCF(context->eflags) || ISSETZF(context->eflags));
HANDLER_DEF_JCC_REL8( je, ISSETZF(context->eflags));
HANDLER_DEF_JCC_REL8( jne, !ISSETZF(context->eflags));
HANDLER_DEF_JCC_REL8( jg, !ISSETZF(context->eflags) && ISSETSF(context->eflags) == ISSETOF(context->eflags));
HANDLER_DEF_JCC_REL8( jge, ISSETSF(context->eflags) == ISSETOF(context->eflags));
HANDLER_DEF_JCC_REL8( jl, ISSETSF(context->eflags) != ISSETOF(context->eflags));
HANDLER_DEF_JCC_REL8( jle, ISSETZF(context->eflags) || ISSETSF(context->eflags) != ISSETOF(context->eflags));
HANDLER_DEF_JCC_REL8( jno, !ISSETOF(context->eflags) );
HANDLER_DEF_JCC_REL8( jnp, !ISSETPF(context->eflags) );
HANDLER_DEF_JCC_REL8( jns, !ISSETSF(context->eflags) );
HANDLER_DEF_JCC_REL8( jo, ISSETOF(context->eflags) );
HANDLER_DEF_JCC_REL8( jp, ISSETPF(context->eflags) );
HANDLER_DEF_JCC_REL8( js, ISSETSF(context->eflags) );
HANDLER_DEF_JCC_REL8( jmp, true );

HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( ja, !ISSETCF(context->eflags) && !ISSETZF(context->eflags) );
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jae, !ISSETCF(context->eflags) );
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jb, ISSETCF(context->eflags) );
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jbe, ISSETCF(context->eflags) || ISSETZF(context->eflags));
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( je, ISSETZF(context->eflags));
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jne, !ISSETZF(context->eflags));
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jg, !ISSETZF(context->eflags) && ISSETSF(context->eflags) == ISSETOF(context->eflags));
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jge, ISSETSF(context->eflags) == ISSETOF(context->eflags));
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jl, ISSETSF(context->eflags) != ISSETOF(context->eflags));
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jle, ISSETZF(context->eflags) || ISSETSF(context->eflags) != ISSETOF(context->eflags));
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jno, !ISSETOF(context->eflags) );
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jnp, !ISSETPF(context->eflags) );
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jns, !ISSETSF(context->eflags) );
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jo, ISSETOF(context->eflags) );
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jp, ISSETPF(context->eflags) );
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( js, ISSETSF(context->eflags) );
HANDLER_DEF_JCC_REL1632_WITH_ESCAPE_SEQUENCE( jmp, true );

HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( ja, !ISSETCF(context->eflags) && !ISSETZF(context->eflags) );
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jae, !ISSETCF(context->eflags) );
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jb, ISSETCF(context->eflags) );
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jbe, ISSETCF(context->eflags) || ISSETZF(context->eflags));
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( je, ISSETZF(context->eflags));
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jne, !ISSETZF(context->eflags));
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jg, !ISSETZF(context->eflags) && ISSETSF(context->eflags) == ISSETOF(context->eflags));
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jge, ISSETSF(context->eflags) == ISSETOF(context->eflags));
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jl, ISSETSF(context->eflags) != ISSETOF(context->eflags));
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jle, ISSETZF(context->eflags) || ISSETSF(context->eflags) != ISSETOF(context->eflags));
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jno, !ISSETOF(context->eflags) );
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jnp, !ISSETPF(context->eflags) );
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jns, !ISSETSF(context->eflags) );
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jo, ISSETOF(context->eflags) );
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jp, ISSETPF(context->eflags) );
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( js, ISSETSF(context->eflags) );
HANDLER_DEF_JCC_REL32_WITH_ESCAPE_SEQUENCE( jmp, true );

HANDLER_DEF_SETCC_RM8( seta, !ISSETCF(context->eflags) && !ISSETZF(context->eflags) );
HANDLER_DEF_SETCC_RM8( setae, !ISSETCF(context->eflags) );
HANDLER_DEF_SETCC_RM8( setb, ISSETCF(context->eflags) );
HANDLER_DEF_SETCC_RM8( setbe, ISSETCF(context->eflags) || ISSETZF(context->eflags));
HANDLER_DEF_SETCC_RM8( sete, ISSETZF(context->eflags));
HANDLER_DEF_SETCC_RM8( setne, !ISSETZF(context->eflags));
HANDLER_DEF_SETCC_RM8( setg, !ISSETZF(context->eflags) && ISSETSF(context->eflags) == ISSETOF(context->eflags));
HANDLER_DEF_SETCC_RM8( setge, ISSETSF(context->eflags) == ISSETOF(context->eflags));
HANDLER_DEF_SETCC_RM8( setl, ISSETSF(context->eflags) != ISSETOF(context->eflags));
HANDLER_DEF_SETCC_RM8( setle, ISSETZF(context->eflags) || ISSETSF(context->eflags) != ISSETOF(context->eflags));
HANDLER_DEF_SETCC_RM8( setno, !ISSETOF(context->eflags) );
HANDLER_DEF_SETCC_RM8( setnp, !ISSETPF(context->eflags) );
HANDLER_DEF_SETCC_RM8( setns, !ISSETSF(context->eflags) );
HANDLER_DEF_SETCC_RM8( seto, ISSETOF(context->eflags) );
HANDLER_DEF_SETCC_RM8( setp, ISSETPF(context->eflags) );
HANDLER_DEF_SETCC_RM8( sets, ISSETSF(context->eflags) );

HANDLER_DEF_RM8_R8( mov );
HANDLER_DEF_RM1632_R1632( mov );
HANDLER_DEF_RM32_R32( mov );
HANDLER_DEF_R8_RM8( mov );
HANDLER_DEF_R1632_RM1632( mov );
HANDLER_DEF_R32_RM32( mov ); //uses extended opcode, but there's only one mapping
HANDLER_DEF_RM8_IMM8( mov ); //uses extended opcode, but there's only one mapping
HANDLER_DEF_RM1632_IMM1632( mov ); //uses extended opcode, but there's only one mapping
HANDLER_DEF_RM32_IMM32( mov ); //uses extended opcode, but there's only one mapping

HANDLER_DEF_R8_IMM8( mov, 0xB0 );
HANDLER_DEF_R1632_IMM1632( mov, 0xB8 );
HANDLER_DEF_R32_IMM32( mov, 0xB8 );

HANDLER_DEF_R1632_RM8_SIGN_EXT_WITH_ESCAPE_SEQUENCE( movsx );
HANDLER_DEF_R32_RM8_SIGN_EXT_WITH_ESCAPE_SEQUENCE( movsx );
HANDLER_DEF_R32_RM16_SIGN_EXT_WITH_ESCAPE_SEQUENCE( movsx );

HANDLER_DEF_R1632_RM8_WITH_ESCAPE_SEQUENCE( movzx );
HANDLER_DEF_R32_RM8_WITH_ESCAPE_SEQUENCE( movzx );
HANDLER_DEF_R32_RM16_WITH_ESCAPE_SEQUENCE( movzx );

