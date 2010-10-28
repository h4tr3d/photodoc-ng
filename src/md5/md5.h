/***************************************************************************
 *   Copyright (C) 2006-2010 by Alexander Drozdov                          *
 *   hatred@inbox.ru                                                       *
 *                                                                         *
 *   Distributed under GPLv2 terms                                         *
 *                                                                         *
 ***************************************************************************/
#ifndef MD5_H
#define MD5_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Для совместимости с моей первой реализацией */
typedef uint8_t            BYTE;
typedef uint16_t           UINT16;
typedef uint32_t           UINT32;
typedef uint64_t           UINT64;
typedef unsigned int       UINT; /* Имет размер 16, 32 или 64 бита в 
                                    зависимости от платформы*/
typedef uint8_t           *POINTER;

/* MD5 контекст */
typedef struct {
  UINT64 state[4];    /* state (ABCD) */
  UINT64 count[2];    /* number of bits, modulo 2^64 (lsb first) */
  BYTE   buffer[64];  /* input buffer */
} MD5CTX;


// Функциональность MD5
int md5hash(uint8_t *ptr, uint32_t len, uint8_t hash[16]);

MD5CTX *MD5Init();
void MD5Update(MD5CTX *, BYTE *, UINT);
void MD5Final(MD5CTX *md5_context, BYTE digest[16]);

#ifdef __cplusplus
}
#endif

#endif /* MD5_H */
