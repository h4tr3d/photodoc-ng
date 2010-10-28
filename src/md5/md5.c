/***************************************************************************
 *   Copyright (C) 2006-2010 by Alexander Drozdov                          *
 *   hatred@inbox.ru                                                       *
 *                                                                         *
 *   Distributed under GPLv2 terms                                         *
 *                                                                         *
 ***************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "md5.h"

static void MD5Transform(UINT64 state[4], BYTE block[64]);
static void MD5Encode(BYTE * input, UINT64 * output, UINT len);
static void MD5Decode(UINT64 * input, BYTE * output, UINT len);

static BYTE PADDING[64] = {0x80, 0};

#define DEBUG 1

// Constants for MD5Transform routine
#define S11 7
#define S12 12
#define S13 17
#define S14 22

#define S21 5
#define S22 9
#define S23 14
#define S24 20

#define S31 4
#define S32 11
#define S33 16
#define S34 23

#define S41 6
#define S42 10
#define S43 15
#define S44 21

// F, G, H и I - основные MD5 функции
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

// ROTATE_LEFT сдвигает x влево на n бит (для 32-bit числа).
#define ROTATE_LEFT(x, n) (((x) << (n)) | ( ((x) & 0xffffffff) >> (32-(n))))

// FF, GG, HH, и II - трансформации для проходов 1, 2, 3 и 4.
#define FF(a, b, c, d, x, s, ac) { \
    (a) += F ((b), (c), (d)) + (x) + (UINT64)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) { \
    (a) += G ((b), (c), (d)) + (x) + (UINT64)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
    (a) += H ((b), (c), (d)) + (x) + (UINT64)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
    (a) += I ((b), (c), (d)) + (x) + (UINT64)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
  }

/*
 * Считает md5-хеш для данных в ptr, длинной len, сохраняет значение в hash
 * возвращает 0 в случае неудачи, 1 при успехе;
 */
int md5hash(uint8_t *ptr, uint32_t len, uint8_t hash[16])
{
    MD5CTX *ctx;

    memset(hash, 0, sizeof(hash));

    ctx = MD5Init();
    if(ctx == NULL)
    {
        return 0;
    }

    MD5Update(ctx, ptr, len);
    MD5Final(ctx, hash);

    free(ctx);

    return 1;
}

// MD5 инициализация работы алгоритма MD5, инициализация нового контента
MD5CTX *MD5Init()
{
    MD5CTX *md5_context = (MD5CTX*)calloc(1, sizeof(MD5CTX));

    if(md5_context == NULL)
    {
        return NULL;
    }

    md5_context->count[0] = md5_context->count[1] = 0;

    // Записываем магические инициализационые переменные
    md5_context->state[0] = 0x67452301L;
    md5_context->state[1] = 0xefcdab89L;
    md5_context->state[2] = 0x98badcfeL;
    md5_context->state[3] = 0x10325476L;

    return md5_context;
}

// Операци обновления MD5 блока.
void MD5Update(MD5CTX *md5_context, BYTE * input, UINT inputLen)
{
    UINT i, index, partLen;

    /* Compute number of bytes mod 64 */
    index = (UINT64)((md5_context->count[0] >> 3) & 0x3F);

    /* Update number of bits */
    if ((md5_context->count[0] += ((UINT64)inputLen << 3)) < ((UINT64)inputLen << 3))
        md5_context->count[1]++;

    md5_context->count[1] += ((UINT64)inputLen >> 29);
    partLen = 64 - index;

    /* Transform as many times as possible.*/
    if (inputLen >= partLen)
    {
        memcpy ((POINTER)&md5_context->buffer[index], (POINTER)input, partLen);
        MD5Transform (md5_context->state, md5_context->buffer);

        for (i = partLen; i + 63 < inputLen; i += 64)
            MD5Transform (md5_context->state, &input[i]);

        index = 0;
    }
    else
        i = 0;

    /* Buffer remaining input */
    memcpy((POINTER)&md5_context->buffer[index], (POINTER)&input[i], inputLen-i);
}

// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.
void MD5Final(MD5CTX *md5_context, BYTE digest[16])
{
    BYTE bits[8];
    UINT index, padLen;

    /* Save number of bits */
    MD5Encode (bits, md5_context->count, 8);

    /* Pad out to 56 mod 64. */
    index = (UINT64)((md5_context->count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    MD5Update(md5_context, PADDING, padLen);

    /* Append length (before padding) */
    MD5Update(md5_context, bits, 8);

    /* Store state in digest */
    MD5Encode (digest, md5_context->state, 16);

    /* Zeroize sensitive information. */
    memset ((POINTER)md5_context, 0, sizeof (*md5_context));
}

static void MD5Transform( UINT64 state[4], BYTE block[64])
{
    UINT64 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

    MD5Decode (x, block, 64);

    /* Round 1 */
    FF ( a, b, c, d, x[ 0], S11, 0xd76aa478L); /* 1 */
    FF ( d, a, b, c, x[ 1], S12, 0xe8c7b756L); /* 2 */
    FF ( c, d, a, b, x[ 2], S13, 0x242070dbL); /* 3 */
    FF ( b, c, d, a, x[ 3], S14, 0xc1bdceeeL); /* 4 */
    FF ( a, b, c, d, x[ 4], S11, 0xf57c0fafL); /* 5 */
    FF ( d, a, b, c, x[ 5], S12, 0x4787c62aL); /* 6 */
    FF ( c, d, a, b, x[ 6], S13, 0xa8304613L); /* 7 */
    FF ( b, c, d, a, x[ 7], S14, 0xfd469501L); /* 8 */
    FF ( a, b, c, d, x[ 8], S11, 0x698098d8L); /* 9 */
    FF ( d, a, b, c, x[ 9], S12, 0x8b44f7afL); /* 10 */
    FF ( c, d, a, b, x[10], S13, 0xffff5bb1L); /* 11 */
    FF ( b, c, d, a, x[11], S14, 0x895cd7beL); /* 12 */
    FF ( a, b, c, d, x[12], S11, 0x6b901122L); /* 13 */
    FF ( d, a, b, c, x[13], S12, 0xfd987193L); /* 14 */
    FF ( c, d, a, b, x[14], S13, 0xa679438eL); /* 15 */
    FF ( b, c, d, a, x[15], S14, 0x49b40821L); /* 16 */

    /* Round 2 */
    GG ( a, b, c, d, x[ 1], S21, 0xf61e2562L); /* 17 */
    GG ( d, a, b, c, x[ 6], S22, 0xc040b340L); /* 18 */
    GG ( c, d, a, b, x[11], S23, 0x265e5a51L); /* 19 */
    GG ( b, c, d, a, x[ 0], S24, 0xe9b6c7aaL); /* 20 */
    GG ( a, b, c, d, x[ 5], S21, 0xd62f105dL); /* 21 */
    GG ( d, a, b, c, x[10], S22,  0x2441453L); /* 22 */
    GG ( c, d, a, b, x[15], S23, 0xd8a1e681L); /* 23 */
    GG ( b, c, d, a, x[ 4], S24, 0xe7d3fbc8L); /* 24 */
    GG ( a, b, c, d, x[ 9], S21, 0x21e1cde6L); /* 25 */
    GG ( d, a, b, c, x[14], S22, 0xc33707d6L); /* 26 */
    GG ( c, d, a, b, x[ 3], S23, 0xf4d50d87L); /* 27 */
    GG ( b, c, d, a, x[ 8], S24, 0x455a14edL); /* 28 */
    GG ( a, b, c, d, x[13], S21, 0xa9e3e905L); /* 29 */
    GG ( d, a, b, c, x[ 2], S22, 0xfcefa3f8L); /* 30 */
    GG ( c, d, a, b, x[ 7], S23, 0x676f02d9L); /* 31 */
    GG ( b, c, d, a, x[12], S24, 0x8d2a4c8aL); /* 32 */

    /* Round 3 */
    HH ( a, b, c, d, x[ 5], S31, 0xfffa3942L); /* 33 */
    HH ( d, a, b, c, x[ 8], S32, 0x8771f681L); /* 34 */
    HH ( c, d, a, b, x[11], S33, 0x6d9d6122L); /* 35 */
    HH ( b, c, d, a, x[14], S34, 0xfde5380cL); /* 36 */
    HH ( a, b, c, d, x[ 1], S31, 0xa4beea44L); /* 37 */
    HH ( d, a, b, c, x[ 4], S32, 0x4bdecfa9L); /* 38 */
    HH ( c, d, a, b, x[ 7], S33, 0xf6bb4b60L); /* 39 */
    HH ( b, c, d, a, x[10], S34, 0xbebfbc70L); /* 40 */
    HH ( a, b, c, d, x[13], S31, 0x289b7ec6L); /* 41 */
    HH ( d, a, b, c, x[ 0], S32, 0xeaa127faL); /* 42 */
    HH ( c, d, a, b, x[ 3], S33, 0xd4ef3085L); /* 43 */
    HH ( b, c, d, a, x[ 6], S34,  0x4881d05L); /* 44 */
    HH ( a, b, c, d, x[ 9], S31, 0xd9d4d039L); /* 45 */
    HH ( d, a, b, c, x[12], S32, 0xe6db99e5L); /* 46 */
    HH ( c, d, a, b, x[15], S33, 0x1fa27cf8L); /* 47 */
    HH ( b, c, d, a, x[ 2], S34, 0xc4ac5665L); /* 48 */

    /* Round 4 */
    II ( a, b, c, d, x[ 0], S41, 0xf4292244L); /* 49 */
    II ( d, a, b, c, x[ 7], S42, 0x432aff97L); /* 50 */
    II ( c, d, a, b, x[14], S43, 0xab9423a7L); /* 51 */
    II ( b, c, d, a, x[ 5], S44, 0xfc93a039L); /* 52 */
    II ( a, b, c, d, x[12], S41, 0x655b59c3L); /* 53 */
    II ( d, a, b, c, x[ 3], S42, 0x8f0ccc92L); /* 54 */
    II ( c, d, a, b, x[10], S43, 0xffeff47dL); /* 55 */
    II ( b, c, d, a, x[ 1], S44, 0x85845dd1L); /* 56 */
    II ( a, b, c, d, x[ 8], S41, 0x6fa87e4fL); /* 57 */
    II ( d, a, b, c, x[15], S42, 0xfe2ce6e0L); /* 58 */
    II ( c, d, a, b, x[ 6], S43, 0xa3014314L); /* 59 */
    II ( b, c, d, a, x[13], S44, 0x4e0811a1L); /* 60 */
    II ( a, b, c, d, x[ 4], S41, 0xf7537e82L); /* 61 */
    II ( d, a, b, c, x[11], S42, 0xbd3af235L); /* 62 */
    II ( c, d, a, b, x[ 2], S43, 0x2ad7d2bbL); /* 63 */
    II ( b, c, d, a, x[ 9], S44, 0xeb86d391L); /* 64 */

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    /* Zeroize sensitive information */
    memset ((POINTER)x, 0, sizeof (x));
}

static void MD5Encode( BYTE * output, UINT64 * input, UINT len)
{
    unsigned int i, j;

    for (i = 0, j = 0; j < len; i++, j += 4) {
        output[j]   = (BYTE)(input[i] & 0xff);
        output[j+1] = (BYTE)((input[i] >> 8) & 0xff);
        output[j+2] = (BYTE)((input[i] >> 16) & 0xff);
        output[j+3] = (BYTE)((input[i] >> 24) & 0xff);
    }
}

static void MD5Decode( UINT64 * output, BYTE * input, UINT len)
{
    UINT i, j;

    for (i = 0, j = 0; j < len; i++, j += 4)
        output[i] = ((UINT64)input[j]) | (((UINT64)input[j+1]) << 8) |
            (((UINT64)input[j+2]) << 16) | (((UINT64)input[j+3]) << 24);
}
