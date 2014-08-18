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
cplusplus {{
        #include "NetwPkt_m.h"
}}

class NetwPkt;
class LAddress::L3Type extends void;

packet NdnNetPkt extends NetwPkt {
        LAddress::L3Type creatorAddr;
        unsigned int pitBloomFilterMap1;
        unsigned int pitBloomFilterMap2;
        unsigned int pitBloomFilterMap3;
        unsigned int pitBloomFilterMap4;
        
        unsigned int csBloomFilterMap1;
        unsigned int csBloomFilterMap2;
        unsigned int csBloomFilterMap3;
        unsigned int csBloomFilterMap4;
}