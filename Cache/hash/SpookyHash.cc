//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "SpookyHash.h"



SpookyHash::SpookyHash() {

}

SpookyHash::~SpookyHash() {

}

uint32_t SpookyHash::SpookHash(const char* message, int len, uint32_t seed)
{
   uint64_t hash1 = seed, hash2 = seed;
   SpookyHash128(message, len, &hash1, &hash2);
   return (uint32_t)hash1;
}

void SpookyHash::SpookyHash128(const char* message ,size_t length, uint64_t* hash1, uint64_t* hash2)
{
   if (length < sc_bufSize)
   {
       SpookyShortHash(message, length, hash1, hash2);
       return;
   }

   uint64_t h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11;
   uint64_t buf[sc_numVars];
   uint64_t *end;
   union
   {
       const uint8_t *p8;
       uint64_t *p64;
       size_t i;
   } u;
   size_t remainder;

   h0=h3=h6=h9  = *hash1;
   h1=h4=h7=h10 = *hash2;
   h2=h5=h8=h11 = sc_const;

   u.p8 = (const uint8_t *)message;
   end = u.p64 + (length/sc_blockSize)*sc_numVars;

   // handle all whole sc_blockSize blocks of bytes
   if (ALLOW_UNALIGNED_READS || ((u.i & 0x7) == 0))
   {
       while (u.p64 < end)
       {
           Mix(u.p64, h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
           u.p64 += sc_numVars;
       }
   }
   else
   {
       while (u.p64 < end)
       {
           memcpy(buf, u.p64, sc_blockSize);
           Mix(buf, h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
           u.p64 += sc_numVars;
       }
   }

   // handle the last partial block of sc_blockSize bytes
   remainder = (length - ((const uint8_t *)end-(const uint8_t *)message));
   memcpy(buf, end, remainder);
   memset(((uint8_t *)buf)+remainder, 0, sc_blockSize-remainder);
   ((uint8_t *)buf)[sc_blockSize-1] = remainder;
   Mix(buf, h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);

   // do some final mixing
   End(h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
   *hash1 = h0;
   *hash2 = h1;
}

void SpookyHash::SpookyShortHash(const char* message, size_t length, uint64_t* hash1, uint64_t* hash2)
{
   uint64_t buf[sc_numVars];
   union
   {
       const uint8_t *p8;
       uint32_t *p32;
       uint64_t *p64;
       size_t i;
   } u;

   u.p8 = (const uint8_t *)message;

   if (!ALLOW_UNALIGNED_READS && (u.i & 0x7))
   {
       memcpy(buf, message, length);
       u.p64 = buf;
   }

   size_t remainder = length%32;
   uint64_t a=*hash1;
   uint64_t b=*hash2;
   uint64_t c=sc_const;
   uint64_t d=sc_const;

   if (length > 15)
   {
       const uint64_t *end = u.p64 + (length/32)*4;

       // handle all complete sets of 32 bytes
       for (; u.p64 < end; u.p64 += 4)
       {
           c += u.p64[0];
           d += u.p64[1];
           ShortMix(a,b,c,d);
           a += u.p64[2];
           b += u.p64[3];
       }

       //Handle the case of 16+ remaining bytes.
       if (remainder >= 16)
       {
           c += u.p64[0];
           d += u.p64[1];
           ShortMix(a,b,c,d);
           u.p64 += 2;
           remainder -= 16;
       }
   }

   // Handle the last 0..15 bytes, and its length
   d = ((uint64_t)length) << 56;
   switch (remainder)
   {
   case 15:
       d += ((uint64_t)u.p8[14]) << 48;
   case 14:
       d += ((uint64_t)u.p8[13]) << 40;
   case 13:
       d += ((uint64_t)u.p8[12]) << 32;
   case 12:
       d += u.p32[2];
       c += u.p64[0];
       break;
   case 11:
       d += ((uint64_t)u.p8[10]) << 16;
   case 10:
       d += ((uint64_t)u.p8[9]) << 8;
   case 9:
       d += (uint64_t)u.p8[8];
   case 8:
       c += u.p64[0];
       break;
   case 7:
       c += ((uint64_t)u.p8[6]) << 48;
   case 6:
       c += ((uint64_t)u.p8[5]) << 40;
   case 5:
       c += ((uint64_t)u.p8[4]) << 32;
   case 4:
       c += u.p32[0];
       break;
   case 3:
       c += ((uint64_t)u.p8[2]) << 16;
   case 2:
       c += ((uint64_t)u.p8[1]) << 8;
   case 1:
       c += (uint64_t)u.p8[0];
       break;
   case 0:
       c += sc_const;
       d += sc_const;
   }
   ShortEnd(a,b,c,d);
   *hash1 = a;
   *hash2 = b;
}

