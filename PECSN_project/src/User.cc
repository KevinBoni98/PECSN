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

#include "User.h"
using namespace omnetpp;




namespace pecsn_project {

Define_Module(User);

User::User() {
    // TODO Auto-generated constructor stub

}

User::~User() {
    // TODO Auto-generated destructor stub
}
void User::initialize()
{
    warmup = getParentModule()->par("warmup").doubleValueInUnit("s");
    packetDelay = registerSignal("packetDelay");
    throughput = registerSignal("throughput");
    packetsReceived = registerSignal("packetsReceived");
    frameReceived = registerSignal("frameReceived");
    int multiplier = getParentModule()->par("CQIRngMultiplier");
    nUsers = getParentModule()->par("NUM_USER");
    id = getIndex();
    rngIndex = id + multiplier*nUsers;
    distribution = par("CQIdistribution").stringValue();
    //EV<<"distribution"<<distribution<<endl;
    p = (double)(id+1)/(double)nUsers;
    //EV<<"p = "<<p<<endl;
    sendCQI();
}

void User::handleMessage(cMessage *msg){
    Frame *frame = check_and_cast<Frame*>(msg);
    std::vector<Packet*> pl = frame->getPacketList();
    //EV<<"total size:\t"<<pl.size()<<endl;
    int bytesReceived = 0;
    int pR = 0;
    int fR = 0;
    while(pl.size() != 0){
        Packet * p = pl.back();
        pl.pop_back();
        if (p->getDestination() == id){
            if (fR == 0) fR++;
            pR++;
            if (simTime() >= warmup){
                simtime_t elapsed = simTime() - p->getArrivalTime();//arrival time = generation time = arrival at base station
                emit(packetDelay, elapsed.dbl());
            }

            bytesReceived += p->getLength();
            //EV<<"packet size: "<<p->getLength()<<endl;
        }
    }
   // EV<<"packets received by id = "<<id<<": "<<pR<<endl;
    if (simTime() >= warmup){
        emit(packetsReceived, pR);
        emit(frameReceived, fR);
        emit(throughput, bytesReceived);
    }
    delete(msg);
    sendCQI();

}

void User::sendCQI(){

    if (distribution.compare("uniform") == 0){
        cqi = intuniform(1, 15, rngIndex);
    }
    else if (distribution.compare("binomial") == 0){
        cqi = binomial(14, p, rngIndex);
        cqi +=1;
    }
    else if (distribution.compare("constant") == 0){
        cqi = id+1;
        // cqi = 1;
    } 

    CQImsg * msg = new CQImsg("CQI");
    msg->setNewCQI(cqi);
    //EV<<"packet name: "<<msg->getName()<<endl;
    send(msg,"CQI_out");
    //EV<<"sending cqi = "<<msg->getNewCQI()<<endl;

}

} /* namespace pecsn_project */


