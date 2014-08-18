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
        #include "ApplPkt_m.h"
}}

class ApplPkt;
class LAddress::L3Type extends void;

packet NdnApplPkt extends ApplPkt {
    
    	int maxNbHops;
    	int nbHops;
    	int msgId;
    	int isExtendedPkt;
        LAddress::L3Type creatorAddr;
        
        
        // 768 bit structure
        // self generated BloomFilter Maps
        unsigned int selfPitBFM1;
        unsigned int selfPitBFM2;
        unsigned int selfPitBFM3;
        unsigned int selfPitBFM4;
        
        unsigned int selfCsBFM1;
        unsigned int selfCsBFM2;
        unsigned int selfCsBFM3;
        unsigned int selfCsBFM4;
        
        // one hop neighbour Bloomfilter Map
        unsigned int ohPitBFM1;
        unsigned int ohPitBFM2;
        unsigned int ohPitBFM3;
        unsigned int ohPitBFM4;
        
        unsigned int ohCsBFM1;
        unsigned int ohCsBFM2;
        unsigned int ohCsBFM3;
        unsigned int ohCsBFM4;
        
        // two hop neighbour BloomFilter Map
        unsigned int thPitBFM1;
        unsigned int thPitBFM2;
        unsigned int thPitBFM3;
        unsigned int thPitBFM4;
        
        unsigned int thCsBFM1;
        unsigned int thCsBFM2;
        unsigned int thCsBFM3;
        unsigned int thCsBFM4;
        
        
}