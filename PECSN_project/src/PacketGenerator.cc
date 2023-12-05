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

#include "PacketGenerator.h"
using namespace omnetpp;



namespace pecsn_project {

Define_Module(PacketGenerator);


PacketGenerator::PacketGenerator() {
    // TODO Auto-generated constructor stub

}

PacketGenerator::~PacketGenerator() {
    // TODO Auto-generated destructor stub
}
void PacketGenerator::initialize(){
    beep = new cMessage("beep");
    //double time = exponential(parameters)
    //scheduleAt(simTime()+time,beep);
    //setta altri parametri?
}

void PacketGenerator::handleMessage(cMessage *msg){
    if (msg->isSelfMessage()){
        //generate packet, assign index of this PG
        //send(packet)
        //double time = exponential(parameters)
        //scheduleAt(simTime()+time,beep);
    }
}

} /* namespace pecsn_project */
