#ifndef X86INTERPRETER_INTERPRETER_H
#define X86INTERPRETER_INTERPRETER_H

#include "runtime.h"
#include <assert.h>
#include "interpreter_util.h"


int interpret( RuntimeEnvironment_t *const environment, ThreadNode_t *const thread );

#ifdef _WIN32
extern int (FASTCALL * opcode_dispatch_table[256]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * opcode_with_prefix_dispatch_table[256]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * opcode_with_escape_sequence_dispatch_table[256]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
extern int (FASTCALL * opcode_with_prefix_and_escape_sequence_dispatch_table[256]) (ThreadContext_t *, VirtualDirectoryLookupTable_t *);
#endif

int FASTCALL undefined_opcode_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_al_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_eax_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_rm8_r8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_rm32_imm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_rm32_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_rm8_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_rm32_r32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_r8_rm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_r32_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
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

int FASTCALL lea_r32_rm32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL leave32_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL interrupt_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
//OPCODE WITH PREFIXES
int FASTCALL addsub_ar1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_rm1632_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_rm1632_imm8_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_rm1632_r1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL addsub_r1632_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL callpush_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL pop_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL push_imm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL lea_r1632_rm1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL leave1632_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL ret_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );



//OPCODE WITH ESCAPE SEQUENCE
int FASTCALL push_fgs_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

int FASTCALL prefix_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL escape_sequence_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );
int FASTCALL prefix_with_escape_sequence_handler( ThreadContext_t *const, VirtualDirectoryLookupTable_t *const );

#endif //X86INTERPRETER_INTERPRETER_H