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

import org.mixim.base.modules.IBaseApplLayer;
import org.mixim.base.modules.IBaseArp;
import org.mixim.base.modules.IBaseNetwLayer;
import org.mixim.base.modules.IMobility;
import org.mixim.modules.NDNx.Cache.ICacheDaemon;
import org.mixim.modules.NDNx.Cache.IContentStore;
import org.mixim.modules.NDNx.Cache.IForwardingInfoBase;
import org.mixim.modules.NDNx.Cache.IPendingInterestTable;
import org.mixim.modules.nic.IWirelessNicUpperCtrl;


module NdnNode
{
    parameters:
        string cacheDaemonType = default("CacheDaemon");
        string PendingInterestType = default("PendingInterestTable");
        string ContentStoreType = default("ContentStore");
        string ForwardingInfoBaseType = default("ForwardingInfoBase");
        string arpType = default("BaseArp");
        string applicationType = default("NdnApplLayer");
        string networktype = default("NdnNetwLayer");
        string mobilityType = default("ConstSpeedMobility");
        string nicType;

    gates:
        input radioIn; // gate for sendDirect

    submodules:
        arp: <arpType> like IBaseArp {
            @display("p=162,331;i=block/network2");
        }
        mobility: <mobilityType> like IMobility {
            parameters:
                @display("p=162,50;i=block/cogwheel");
        }
        nic: <nicType> like IWirelessNicUpperCtrl {
            parameters:
                @display("p=70,331;i=block/wrxtx,#008000");
        }
        // OSI network layer
        netwl: <networkType> like IBaseNetwLayer {
            parameters:
                @display("p=70,196;i=block/layer");
        }

        appl: <applicationType> like IBaseApplLayer {
            @display("p=70,50");
        }

        cache: <cacheDaemonType> like ICacheDaemon {

            @display("p=162,128");
        }

        pit: <PendingInterestTableType> like IPendingInterestTable {
            @display("p=162,196");
        }

        cs: <ContentStoreType> like IContentStore {
            @display("p=314,196");
        }
        fib: <ForwardingInfoBase> like IForwardingInfoBase {
            @display("p=239,196");
        }
    connections allowunconnected:
        netwl.upperLayerOut --> appl.lowerLayerIn;
        netwl.upperLayerIn <-- appl.lowerLayerOut;
        netwl.upperControlOut --> { @display("ls=red;m=m,70,0,70,0"); } --> appl.lowerControlIn;
        netwl.upperControlIn <-- { @display("ls=red;m=m,70,0,70,0"); } <-- appl.lowerControlOut;

        nic.upperLayerOut --> netwl.lowerLayerIn;
        nic.upperLayerIn <-- netwl.lowerLayerOut;
        nic.upperControlOut --> { @display("ls=red;m=m,70,0,70,0"); } --> netwl.lowerControlIn;
        nic.upperControlIn <-- { @display("ls=red;m=m,70,0,70,0"); } <-- netwl.lowerControlOut;

        radioIn --> nic.radioIn;
}
