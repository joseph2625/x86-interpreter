#ifndef X86INTERPRETER_INTERPRETER_H
#define X86INTERPRETER_INTERPRETER_H

#include "runtime.h"
#include <assert.h>
#include "interpreter_macro_expansion.h"
#include "interpreter_util.h"
#include "interpreter_util_instructions.h"

int interpret( RuntimeEnvironment_t *const environment, ThreadNode_t *const thread );

#ifdef _MSC_VER
extern int (FASTCALL * opcode_dispatch_table[256]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * opcode_with_prefix_dispatch_table[256]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * opcode_with_escape_sequence_dispatch_table[256]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * opcode_with_prefix_and_escape_sequence_dispatch_table[256]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);

extern int (FASTCALL * addsubcmpandxoror_rm8_imm8_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * addsubcmpandxoror_rm1632_imm1632_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * addsubcmpandxoror_rm32_imm32_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * addsubcmpandxoror_rm1632_imm8_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * addsubcmpandxoror_rm32_imm8_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);

extern int (FASTCALL * testnotnegmulimul_rm8_optimm8_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * testnotnegmulimul_rm1632_optimm1632_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * testnotnegmulimul_rm32_optimm32_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);

extern int (FASTCALL * decinc_rm8__dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * callpushdecincjmp_rm32__dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * callpushdecincjmp_rm1632__dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);

extern int (FASTCALL * salshlsarshr_rm8_1_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * salshlsarshr_rm1632_1_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * salshlsarshr_rm32_1_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * salshlsarshr_rm8_cl_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * salshlsarshr_rm1632_cl_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * salshlsarshr_rm32_cl_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * salshlsarshr_rm8_imm8_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * salshlsarshr_rm1632_imm8_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * salshlsarshr_rm32_imm8_dispatch_table[8]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
#endif


int FASTCALL add_al_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL add_eax_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL add_ar1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL sub_al_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_eax_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_ar1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL cmp_al_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_eax_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_ar1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL and_al_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_eax_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_ar1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL xor_al_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_eax_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_ar1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL or_al_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_eax_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_ar1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL test_al_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL test_eax_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL test_ar1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL add_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsubcmpandxoror_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL test_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL testnotnegmulimul_rm8_optimm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL add_rm1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_rm1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_rm1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_rm1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_rm1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_rm1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsubcmpandxoror_rm1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL test_rm1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL testnotnegmulimul_rm1632_optimm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );


int FASTCALL add_rm32_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_rm32_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_rm32_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_rm32_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_rm32_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_rm32_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsubcmpandxoror_rm32_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL test_rm32_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL testnotnegmulimul_rm32_optimm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL add_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL addsubcmpandxoror_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL add_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsubcmpandxoror_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL add_rm8_r8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_rm8_r8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_rm8_r8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_rm8_r8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_rm8_r8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_rm8_r8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL test_rm8_r8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL add_rm1632_r1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_rm1632_r1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_rm1632_r1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_rm1632_r1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_rm1632_r1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_rm1632_r1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL test_rm1632_r1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL add_rm32_r32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_rm32_r32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_rm32_r32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_rm32_r32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_rm32_r32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_rm32_r32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL test_rm32_r32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL add_r8_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_r8_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_r8_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_r8_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_r8_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_r8_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL add_r1632_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_r1632_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_r1632_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_r1632_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_r1632_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_r1632_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL add_r32_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sub_r32_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL cmp_r32_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL and_r32_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL xor_r32_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL or_r32_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL sal_rm8_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shl_rm8_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sar_rm8_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shr_rm8_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL salshlsarshr_rm8_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL sal_rm1632_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shl_rm1632_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sar_rm1632_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shr_rm1632_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL salshlsarshr_rm1632_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL sal_rm32_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shl_rm32_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sar_rm32_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shr_rm32_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL salshlsarshr_rm32_1_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL sal_rm8_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shl_rm8_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sar_rm8_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shr_rm8_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL salshlsarshr_rm8_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL sal_rm1632_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shl_rm1632_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sar_rm1632_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shr_rm1632_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL salshlsarshr_rm1632_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL sal_rm32_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shl_rm32_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sar_rm32_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shr_rm32_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL salshlsarshr_rm32_cl_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL sal_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shl_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sar_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shr_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL salshlsarshr_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL sal_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shl_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sar_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shr_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL salshlsarshr_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL sal_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shl_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sar_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL shr_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL salshlsarshr_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL neg_rm8__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL neg_rm1632__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL neg_rm32__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL not_rm8__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL not_rm1632__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL not_rm32__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mul_rm8__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mul_rm1632__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mul_rm32__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL imul_rm8__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL imul_rm1632__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL imul_rm32__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL imul_r32_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL imul_r1632_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL imul_r32_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL imul_r1632_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL imul_r32_rm32_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL imul_r1632_rm1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL undefined_opcode_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL callpush_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL pop_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL push_r32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL push_r1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL pop_r32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL pop_r1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL call_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL push_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL push_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL mov_rm8_r8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_rm1632_r1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_rm32_r32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_r8_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_r1632_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_r32_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_rm16_sreg_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_r8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_r1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_r32_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_rm1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_rm32_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL mov_r32_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_r1632_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_al_moffs8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_moffs8_al_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_ar_moffs1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL mov_moffs1632_ar_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL movsx_r1632_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL movsx_r32_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL movsx_r32_rm16_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL movzx_r1632_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL movzx_r32_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL movzx_r32_rm16_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL dec_rm8__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL dec_rm1632__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL dec_rm32__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL dec_r1632__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL dec_r32__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL inc_rm8__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL inc_rm1632__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL inc_rm32__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL inc_r1632__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL inc_r32__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );


int FASTCALL decinc_rm8__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL callpushdecincjmp_rm1632__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL callpushdecincjmp_rm32__handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL ja_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jae_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jb_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jbe_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL je_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jne_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jg_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jge_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jl_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jle_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jno_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jnp_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jns_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jo_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jp_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL js_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jmp_rel8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL ja_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jae_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jb_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jbe_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL je_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jne_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jg_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jge_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jl_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jle_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jno_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jnp_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jns_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jo_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jp_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL js_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jmp_rel1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL ja_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jae_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jb_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jbe_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL je_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jne_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jg_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jge_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jl_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jle_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jno_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jnp_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jns_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jo_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jp_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL js_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jmp_rel32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL jmp_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL jmp_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL seta_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL setae_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL setb_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL setbe_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sete_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL setne_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL setg_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL setge_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL setl_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL setle_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL setno_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL setnp_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL setns_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL seto_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL setp_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL sets_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL nop_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL lea_r32_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL leave32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL interrupt_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL callpush_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL pop_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL push_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL lea_r1632_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL leave1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL ret_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL push_fgs_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL prefix_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL escape_sequence_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL prefix_with_escape_sequence_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

#endif //X86INTERPRETER_INTERPRETER_H