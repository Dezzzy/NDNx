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

package org.mixim.modules.NDNx.Application;


moduleinterface INdnApplLayer
{
    parameters:
        int headerLength @unit("bit"); // Application Layer Header Length
    gates:
        input upperLayerIn; // from Daemon layer
        output upperLayerOut; // to Daemon layer
        input upperControlIn; // control from Daemon layer
        output upperControlOut; // control to Daemon layer
        input lowerLayerIn; // from Ndn
        output lowerLayerOut; // to NIC
        input lowerControlIn; // control from NIC
        output lowerControlOut; // control to NIC
}
