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

package org.mixim.modules.NDNx.Node;

import org.mixim.modules.NDNx.Cache.ICacheDaemon;
import org.mixim.modules.NDNx.Cache.IContentStore;
import org.mixim.modules.NDNx.Cache.IForwardingInfoBase;
import org.mixim.modules.NDNx.Cache.IPendingInterestTable;


module NdnCache
{
    parameters:
        string CacheDaemonType = default("CacheDaemon");
        string PendingInterestTableType = default("PendingInterestTable");
        string ContentStoreType = default("ContentStore");
        string ForwardingInfoBase = default("ForwardingInfoBase");

    submodules:
        cache: <CacheDaemonType> like ICacheDaemon {
            @display("p=51,51");
        }

        pit: <PendingInterestTableType> like IPendingInterestTable {
            @display("p=162,51");
        }

        cs: <ContentStoreType> like IContentStore;
        fib: <ForwardingInfoBase> like IForwardingInfoBase {
            @display("p=335,51");
        }
}
