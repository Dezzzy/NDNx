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

/*
* MurmurHash3, created by Austin Appleby for use in non-cryptographic hashing, is a uniformly distributed and independent function
* which good performance
*/
#ifndef MURMURHASH_H_
#define MURMURHASH_H_

#include "stdint.h"


#define ALLOW_UNALIGNED_READS 1

class MurmurHash {
public:
   MurmurHash();
   virtual ~MurmurHash();
   uint32_t MurmurHash3(const char* key, int len, uint32_t seed);

protected:
   /*
    * ROTL, this function is used to rotate the hashed before final hash is complete
    */
   inline uint32_t ROTL ( uint32_t x, int8_t r )
   {
       return (x << r) | (x >> (32 - r));
   }

   /*
    * fmix, final mixing of bytes for releasing hash
    */
   inline uint32_t fmix ( uint32_t h )
   {
       h ^= h >> 16;
       h *= 0x85ebca6b;
       h ^= h >> 13;
       h *= 0xc2b2ae35;
       h ^= h >> 16;

       return h;
   }

   /*
    * GetBlock, used as a byte block read for MurmurHash3, used to seperate key, allowing easy folding of data when used in hashing
    */
   inline uint32_t getblock ( const uint32_t * p, int i )
   {
       return p[i];
   }

   // end of inline functions

};

#endif /* MURMURHASH_H_ */
