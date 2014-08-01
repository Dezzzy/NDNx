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

#include "MurmurHash.h"

MurmurHash::MurmurHash() {


}

MurmurHash::~MurmurHash() {

}

uint32_t MurmurHash::MurmurHash3(const char* key, int len, uint32_t seed)
{
   const uint8_t * data = (const uint8_t*)key;
   const int nblocks = len / 4;

   uint32_t h1 = seed;

   const uint32_t c1 = 0xcc9e2d51;
   const uint32_t c2 = 0x1b873593;

   //----------
   // body

   const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

   for(int i = -nblocks; i; i++)
   {
       uint32_t k1 = getblock(blocks,i);

       k1 *= c1;
       k1 = ROTL(k1,15);
       k1 *= c2;

       h1 ^= k1;
       h1 = ROTL(h1,13);
       h1 = h1*5+0xe6546b64;
   }

   //----------
   // tail

   const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

   uint32_t k1 = 0;

   switch(len & 3)
   {
   case 3: k1 ^= tail[2] << 16;
   case 2: k1 ^= tail[1] << 8;
   case 1: k1 ^= tail[0];
   k1 *= c1; k1 = ROTL(k1,15); k1 *= c2; h1 ^= k1;
   };

   //----------
   // finalization

   h1 ^= len;

   h1 = fmix(h1);

   return h1;
}
