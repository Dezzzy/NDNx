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

package org.mixim.modules.NDNx.Cache;

import org.mixim.base.modules.BaseLayer;


moduleinterface ICacheDaemon
{
    gates:

        input lowerLayerIn; // from lower layer
        output lowerLayerOut; // to lower layer
        input lowerControlIn; // control from lower layer
        output lowerControlOut; // control to lower layer
}
simple CacheDaemon extends BaseLayer like ICacheDaemon
{
    parameters:
        int CacheSize = default(128);
        int WordSize = default(100);
        int TestingVariable = default(0);
        double timeToLive @unit("seconds") = default(600s);
        double startDelay @unit("seconds") = default(10s);

    
}
