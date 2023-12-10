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
#include "Packet_m.h"
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
    // get user of the system
    int numUser = par("NUM_USER").intValue();
    indexArrivalTimeGen = getIndex()+numUser*2;
    indexPkgSizeGen = getIndex()+numUser*3;
    lambda =  par("LAMBDA").doubleValue();
    simtime_t time = exponential(1/lambda,indexArrivalTimeGen);
    scheduleAt(simTime()+time,beep);
}

void PacketGenerator::handleMessage(cMessage *msg){
    if (msg->isSelfMessage()){
        // Generate packet
        Packet *packet =  new Packet("Packet");
        // destination
        packet->setDestination(getIndex());
        // arrivaltime
        simtime_t arrivalTime = exponential(1/lambda,indexArrivalTimeGen);        
        packet->setArrivalTime(arrivalTime);
        
        // size
        int size = intuniform(1/* min size */, 15/* max size */, indexPkgSizeGen);
        packet->setLength(size);
        EV<<"size: "<<size<<endl;
        // Send packet
        send(packet, "packet_out");
        // Schedule the next self-message
        scheduleAt(simTime() + arrivalTime, beep);
    }
}

} /* namespace pecsn_project */
