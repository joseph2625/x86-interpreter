#ifndef X86INTERPRETER_INTERPRETER_UTIL_ADDSUBCMP_H
#define X86INTERPRETER_INTERPRETER_UTIL_ADDSUBCMP_H
#include "interpreter_util.h"

inline uint32_t perform_32bit_add( const uint32_t i, const uint32_t j, uint32_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = i+j;
  CHECKCFADDITION;
  CHECKOF32BITADDITION;
  CHECKSF32BIT;
  CHECKZF;
  CHECKPF;
  CHECKAFADDITION;
  UNSETAF(eflags);
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint16_t perform_16bit_add( const uint32_t i, const uint32_t j, uint16_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = ( i & 0xFFFF ) + ( j & 0xFFFF );
  CHECKCFADDITION;
  CHECKOF16BITADDITION;
  CHECKSF16BIT;
  CHECKZF;
  CHECKPF;
  CHECKAFADDITION;
  UNSETAF(eflags);
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint8_t perform_8bit_add( const uint32_t i, const uint32_t j, uint8_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = ( i & 0xFF ) + ( j & 0xFF );
  CHECKCFADDITION;
  CHECKOF8BITADDITION;
  CHECKSF8BIT;
  CHECKZF;
  CHECKPF;
  CHECKAFADDITION;
  UNSETAF(eflags);
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint32_t perform_32bit_sub( const uint32_t i, const uint32_t j, uint32_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = i - j;
  CHECKCFSUBTRACTION;
  CHECKOF32BITSUBTRACTION;
  CHECKSF32BIT;
  CHECKZF;
  CHECKPF;
  CHECKAFSUBTRACTION;
  UNSETAF(eflags);
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint16_t perform_16bit_sub( const uint32_t i, const uint32_t j, uint16_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = ( i & 0xFFFF ) - ( j & 0xFFFF );
  CHECKCFSUBTRACTION;
  CHECKOF16BITSUBTRACTION;
  CHECKSF16BIT;
  CHECKZF;
  CHECKPF;
  CHECKAFSUBTRACTION;
  UNSETAF(eflags);
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint8_t perform_8bit_sub( const uint32_t i, const uint32_t j, uint8_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = ( i & 0xFF ) - ( j & 0xFF );
  CHECKCFSUBTRACTION;
  CHECKOF8BITSUBTRACTION;
  CHECKSF8BIT;
  CHECKZF;
  CHECKPF;
  CHECKAFSUBTRACTION;
  UNSETAF(eflags);
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline void perform_32bit_cmp( const uint32_t i, const uint32_t j, uint32_t *, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = i - j;
  CHECKCFSUBTRACTION;
  CHECKOF32BITSUBTRACTION;
  CHECKSF32BIT;
  CHECKZF;
  CHECKPF;
  CHECKAFSUBTRACTION;
  UNSETAF(eflags);
  *context_eflags = eflags;
}

inline void perform_16bit_cmp( const uint32_t i, const uint32_t j, uint16_t *, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = ( i & 0xFFFF ) - ( j & 0xFFFF );
  CHECKCFSUBTRACTION;
  CHECKOF16BITSUBTRACTION;
  CHECKSF16BIT;
  CHECKZF;
  CHECKPF;
  CHECKAFSUBTRACTION;
  UNSETAF(eflags);
  *context_eflags = eflags;
}

inline void perform_8bit_cmp( const uint32_t i, const uint32_t j, uint8_t *, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = ( i & 0xFF ) - ( j & 0xFF );
  CHECKCFSUBTRACTION;
  CHECKOF8BITSUBTRACTION;
  CHECKSF8BIT;
  CHECKZF;
  CHECKPF;
  CHECKAFSUBTRACTION;
  UNSETAF(eflags);
  *context_eflags = eflags;
}
#endif //X86INTERPRETER_INTERPRETER_UTIL_ADDSUBCMP_H