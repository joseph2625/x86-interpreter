#ifndef X86INTERPRETER_INTERPRETER_UTIL_INSTRUCTIONS_H
#define X86INTERPRETER_INTERPRETER_UTIL_INSTRUCTIONS_H
#include "interpreter_util.h"

inline uint32_t perform_32bit_add( const uint32_t i, const uint32_t j, uint32_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = i+j;
  CHECKCFADDITION;
  CHECKOF32BITADDITION;
  CHECKSF32BIT;
  CHECKZF32BIT;
  CHECKPF;
  CHECKAFADDITION;
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
  CHECKZF16BIT;
  CHECKPF;
  CHECKAFADDITION;
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
  CHECKZF8BIT;
  CHECKPF;
  CHECKAFADDITION;
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
  CHECKZF32BIT;
  CHECKPF;
  CHECKAFSUBTRACTION;
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
  CHECKZF16BIT;
  CHECKPF;
  CHECKAFSUBTRACTION;
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
  CHECKZF8BIT;
  CHECKPF;
  CHECKAFSUBTRACTION;
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
  CHECKZF32BIT;
  CHECKPF;
  CHECKAFSUBTRACTION;
  *context_eflags = eflags;
}

inline void perform_16bit_cmp( const uint32_t i, const uint32_t j, uint16_t *, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = ( i & 0xFFFF ) - ( j & 0xFFFF );
  CHECKCFSUBTRACTION;
  CHECKOF16BITSUBTRACTION;
  CHECKSF16BIT;
  CHECKZF16BIT;
  CHECKPF;
  CHECKAFSUBTRACTION;
  *context_eflags = eflags;
}

inline void perform_8bit_cmp( const uint32_t i, const uint32_t j, uint8_t *, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = ( i & 0xFF ) - ( j & 0xFF );
  CHECKCFSUBTRACTION;
  CHECKOF8BITSUBTRACTION;
  CHECKSF8BIT;
  CHECKZF8BIT;
  CHECKPF;
  CHECKAFSUBTRACTION;
  *context_eflags = eflags;
}

inline uint32_t perform_32bit_mov( const uint32_t, const uint32_t j, uint32_t * dest, uint32_t *const ){
  *dest = j;
  return j;
}

inline uint32_t perform_32bit_movsx( const uint32_t i, const uint32_t j, uint32_t * dest, uint32_t *const context_eflags){
  return perform_32bit_mov( i, j, dest, context_eflags );
}

inline uint32_t perform_32bit_movzx( const uint32_t i, const uint32_t j, uint32_t * dest, uint32_t *const context_eflags){
  return perform_32bit_mov( i, j, dest, context_eflags );
}

inline uint16_t perform_16bit_mov( const uint32_t, const uint32_t j, uint16_t * dest, uint32_t *const ){
  *dest = j;
  return j;
}

inline uint16_t perform_16bit_movsx( const uint32_t i, const uint32_t j, uint16_t * dest, uint32_t *const context_eflags){
  return perform_16bit_mov( i, j, dest, context_eflags);
}

inline uint16_t perform_16bit_movzx( const uint32_t i, const uint32_t j, uint16_t * dest, uint32_t *const context_eflags){
  return perform_16bit_mov( i, j, dest, context_eflags);
}

inline uint8_t perform_8bit_mov( const uint32_t, const uint32_t j, uint8_t * dest, uint32_t *const ){
  *dest = j;
  return j;
}

inline uint32_t perform_32bit_and( const uint32_t i, const uint32_t j, uint32_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = i & j;
  UNSETOF(eflags);
  UNSETCF(eflags);
  CHECKSF32BIT;
  CHECKZF32BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}
inline uint16_t perform_16bit_and( const uint32_t i, const uint32_t j, uint16_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = i & j;
  UNSETOF(eflags);
  UNSETCF(eflags);
  CHECKSF16BIT;
  CHECKZF16BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint8_t perform_8bit_and( const uint32_t i, const uint32_t j, uint8_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = i & j;
  UNSETOF(eflags);
  UNSETCF(eflags);
  CHECKSF8BIT;
  CHECKZF8BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint32_t perform_32bit_dec( const uint32_t i, const uint32_t, uint32_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t j = 1;
  uint32_t k = i - j;

  CHECKOF32BITSUBTRACTION;
  CHECKSF32BIT;
  CHECKZF32BIT;
  CHECKPF;
  CHECKAFSUBTRACTION;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint16_t perform_16bit_dec( const uint32_t i, const uint32_t, uint16_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t j = 1;
  uint32_t k = i - j;

  CHECKOF16BITSUBTRACTION;
  CHECKSF16BIT;
  CHECKZF16BIT;
  CHECKPF;
  CHECKAFSUBTRACTION;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint8_t perform_8bit_dec( const uint32_t i, const uint32_t, uint8_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t j = 1;
  uint32_t k = i - j;

  CHECKOF8BITSUBTRACTION;
  CHECKSF8BIT;
  CHECKZF8BIT;
  CHECKPF;
  CHECKAFSUBTRACTION;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint32_t perform_32bit_inc( const uint32_t i, const uint32_t, uint32_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t j = 1;
  uint32_t k = i + j;

  CHECKOF32BITADDITION;
  CHECKSF32BIT;
  CHECKZF32BIT;
  CHECKPF;
  CHECKAFADDITION;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint16_t perform_16bit_inc( const uint32_t i, const uint32_t, uint16_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t j = 1;
  uint32_t k = i - j;

  CHECKOF16BITADDITION;
  CHECKSF16BIT;
  CHECKZF16BIT;
  CHECKPF;
  CHECKAFADDITION;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint8_t perform_8bit_inc( const uint32_t i, const uint32_t, uint8_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t j = 1;
  uint32_t k = i - j;

  CHECKOF8BITADDITION;
  CHECKSF8BIT;
  CHECKZF8BIT;
  CHECKPF;
  CHECKAFADDITION;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint32_t perform_32bit_sal( const uint32_t i, const uint8_t j, uint32_t * dest, uint32_t *const context_eflags ){
  if( j == 0 ) return i;

  uint32_t eflags = *context_eflags;
  uint32_t k = i << j;

  if (((i << (j - 1)) & 0x80000000 ) > 0) {
    if( j == 1 ) {
      if( (k & 0x80000000) > 0 )
        UNSETOF(eflags);
      else
        SETOF(eflags);
    }

    SETCF(eflags);
  }
  else {
    if( j == 1 ) {
      if( (k & 0x80000000) > 0 )
        SETOF(eflags);
      else
        UNSETOF(eflags);
    }

    UNSETCF(eflags);
  }

  CHECKSF32BIT;
  CHECKZF32BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint16_t perform_16bit_sal( const uint16_t i, const uint8_t j, uint16_t * dest, uint32_t *const context_eflags ){
  if( j == 0 ) return i;

  uint32_t eflags = *context_eflags;
  uint16_t k = i << j;

  if (((i << (j - 1)) & 0x8000 ) > 0) {
    if( j == 1 ) {
      if( (k & 0x8000) > 0 )
        UNSETOF(eflags);
      else
        SETOF(eflags);
    }

    SETCF(eflags);
  }
  else {
    if( j == 0 ) {
      if( (k & 0x8000) > 0 )
        SETOF(eflags);
      else
        UNSETOF(eflags);
    }
    UNSETCF(eflags);
  }

  CHECKSF16BIT;
  CHECKZF16BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint8_t perform_8bit_sal( const uint8_t i, const uint8_t j, uint8_t * dest, uint32_t *const context_eflags ){
  if( j == 0 ) return i;

  uint32_t eflags = *context_eflags;
  uint8_t k = i << j;

  if (((i << (j - 1)) & 0x80 ) > 0) {
    if( j == 1 ) {
      if( (k & 0x80) > 0 )
        UNSETOF(eflags);
      else
        SETOF(eflags);
    }

    SETCF(eflags);
  }
  else {
    if( j == 1 ) {
      if( (k & 0x80) > 0 )
        SETOF(eflags);
      else
        UNSETOF(eflags);
    }
    UNSETCF(eflags);
  }

  CHECKSF8BIT;
  CHECKZF8BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}
inline uint32_t perform_32bit_shl( const uint32_t i, const uint8_t j, uint32_t * dest, uint32_t *const context_eflags ){
  return perform_32bit_sal( i, j, dest, context_eflags );
}
inline uint16_t perform_16bit_shl( const uint16_t i, const uint8_t j, uint16_t * dest, uint32_t *const context_eflags ){
  return perform_16bit_sal( i, j, dest, context_eflags );
}
inline uint8_t perform_8bit_shl( const uint8_t i, const uint8_t j, uint8_t * dest, uint32_t *const context_eflags ){
  return perform_8bit_sal( i, j, dest, context_eflags );
}

inline uint32_t perform_32bit_sar( const uint32_t i, const uint8_t j, uint32_t * dest, uint32_t *const context_eflags ){
  if( j == 0 ) return i;

  uint32_t eflags = *context_eflags;
  uint32_t k = (i >> j) | ( 0xFFFFFFFF << (32 - j));

  if( j == 1 )
    UNSETOF(eflags);

  if (((i >> (j - 1)) & 0x1 ) > 0) {
    SETCF(eflags);
  }
  else {
    UNSETCF(eflags);
  }

  CHECKSF32BIT;
  CHECKZF32BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint16_t perform_16bit_sar( const uint16_t i, const uint8_t j, uint16_t * dest, uint32_t *const context_eflags ){
  if( j == 0 ) return i;

  uint32_t eflags = *context_eflags;
  uint16_t k = (i >> j) | ( 0xFFFF << (16 - j));

  if( j == 1 )
    UNSETOF(eflags);

  if (((i >> (j - 1)) & 0x1 ) > 0) {
    SETCF(eflags);
  }
  else {
    UNSETCF(eflags);
  }

  CHECKSF16BIT;
  CHECKZF16BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}


inline uint8_t perform_8bit_sar( const uint8_t i, const uint8_t j, uint8_t * dest, uint32_t *const context_eflags ){
  if( j == 0 ) return i;

  uint32_t eflags = *context_eflags;
  uint8_t k = (i >> j) | ( 0xFF << (8 - j));

  if( j == 1 )
    UNSETOF(eflags);

  if (((i >> (j - 1)) & 0x1 ) > 0) {
    SETCF(eflags);
  }
  else {
    UNSETCF(eflags);
  }

  CHECKSF8BIT;
  CHECKZF8BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint32_t perform_32bit_shr( const uint32_t i, const uint8_t j, uint32_t * dest, uint32_t *const context_eflags ){
  if( j == 0 ) return i;

  uint32_t eflags = *context_eflags;
  uint32_t k = (i >> j);

  if( j == 1 ) {
    if( ( i & 0x80000000 ) > 0)
      SETOF(eflags);
    else
      UNSETOF(eflags);
  }

  if (((i >> (j - 1)) & 0x1 ) > 0) {
    SETCF(eflags);
  }
  else {
    UNSETCF(eflags);
  }

  CHECKSF32BIT;
  CHECKZF32BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint16_t perform_16bit_shr( const uint16_t i, const uint8_t j, uint16_t * dest, uint32_t *const context_eflags ){
  if( j == 0 ) return i;

  uint32_t eflags = *context_eflags;
  uint16_t k = (i >> j);

  if( j == 1 ) {
    if( ( i & 0x8000 ) > 0)
      SETOF(eflags);
    else
      UNSETOF(eflags);
  }

  if (((i >> (j - 1)) & 0x1 ) > 0) {
    SETCF(eflags);
  }
  else {
    UNSETCF(eflags);
  }

  CHECKSF16BIT;
  CHECKZF16BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}


inline uint8_t perform_8bit_shr( const uint8_t i, const uint8_t j, uint8_t * dest, uint32_t *const context_eflags ){
  if( j == 0 ) return i;

  uint32_t eflags = *context_eflags;
  uint8_t k = (i >> j);

  if( j == 1 ) {
    if( ( i & 0x80 ) > 0)
      SETOF(eflags);
    else
      UNSETOF(eflags);
  }

  if (((i >> (j - 1)) & 0x1 ) > 0) {
    SETCF(eflags);
  }
  else {
    UNSETCF(eflags);
  }

  CHECKSF8BIT;
  CHECKZF8BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint32_t perform_32bit_xor( const uint32_t i, const uint32_t j, uint32_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = i ^ j;
  UNSETOF(eflags);
  UNSETCF(eflags);
  CHECKSF32BIT;
  CHECKZF32BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}
inline uint16_t perform_16bit_xor( const uint32_t i, const uint32_t j, uint16_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = i ^ j;
  UNSETOF(eflags);
  UNSETCF(eflags);
  CHECKSF16BIT;
  CHECKZF16BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

inline uint8_t perform_8bit_xor( const uint32_t i, const uint32_t j, uint8_t * dest, uint32_t *const context_eflags ){
  uint32_t eflags = *context_eflags;
  uint32_t k = i ^ j;
  UNSETOF(eflags);
  UNSETCF(eflags);
  CHECKSF8BIT;
  CHECKZF8BIT;
  CHECKPF;
  *context_eflags = eflags;
  *dest = k;
  return k;
}

#endif //X86INTERPRETER_INTERPRETER_UTIL_INSTRUCTIONS_H