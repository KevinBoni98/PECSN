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

#include "BaseStation.h"
using namespace omnetpp;

namespace pecsn_project {

Define_Module(BaseStation);

BaseStation::BaseStation() {
    // TODO Auto-generated constructor stub

}

BaseStation::~BaseStation() {
    // TODO Auto-generated destructor stub
}

void BaseStation::initialize(){
    beep = new cMessage("beep");
    nUsers = getParentModule()->par("N_USERS");
    currentCQI = new int[nUsers];
    for (int i = 0; i < nUsers; i++){
        currentCQI[i] = 0;
    }
    //inizializzo array dei valori in byte dei CQI
    EV<<par("CQIArrayLength").intValue()<<endl;
    int len = par("CQIArrayLength").intValue();
    CQITable = new int[len];
    std::string toParse = par("CQIValues").str();
    std::stringstream ss;
    ss << toParse;
    std::string temp;
    int found;
    int i = 0;
    while (!ss.eof()) {
        ss >> temp;
        EV<<temp<<endl;
        if (std::stringstream(temp) >> found){
            CQITable[i] = found;
            i++;
            if (i >= len) break;
        }
        temp = "";
    }

    queues = new UserQueue*[nUsers];
    RRqueues = new cQueue("RoundRobinQueues");
    // default behavior of cQueue --> FIFO

    for (int i = 0; i < nUsers; i++) {
        std::string name = "User" + std::to_string(i);
        queues[i] = new UserQueue(name.c_str());
        queues[i]->RBsize = 20;
        RRqueues->insert(queues[i]);
    }

    //scheduling timer for frame
    scheduleSelfMessage();
}

void BaseStation::scheduleSelfMessage(){
    double time = par("timeSlot").doubleValueInUnit("s");
    scheduleAt(simTime() + time, beep);
}

void BaseStation::clearFrame(){
    frame->setRBslotsUsed(0);
    std::vector<Packet*> packets = frame->getPacketList();
    while(packets.size()) {
        Packet* p = packets.back();
        packets.pop_back();
        delete(p);
    }
    frame->setPacketList(packets);
}

bool BaseStation::insertIntoFrame(Frame *frame, UserQueue *queue){
    int occupiedSlots = frame->getRBslotsUsed();
    std::vector<Packet*> packets = frame->getPacketList();
    int RBsize = queue->RBsize;

    int emptySlots;
    int freeSpace;
    int freeBytesFromLastRB = 0;
    int packetSize;

    Packet* currentPacket;
    while(!queue->isEmpty()){
        // each frame is composed by 25 RBs
        emptySlots = 25 - occupiedSlots;
        freeSpace = emptySlots * RBsize;

        currentPacket = check_and_cast<Packet*>(queue->get(0));
        packetSize = currentPacket->getLength();

        if(packetSize > freeSpace + freeBytesFromLastRB){
            // not enough space to fit the packet into the frame
            if(!emptySlots){
                // no more empty spots --> frame is full and ready
                frame->setPacketList(packets);
                return true;
            }
            break;
        }
        // #RBs occupied by current packet (packets from the same user can share the same RB)
        float RBoccupiedByPacket = ((float)(packetSize - freeBytesFromLastRB)) / RBsize;

        std::vector<int> RBs = currentPacket->getRBs();
        if (freeBytesFromLastRB > 0) {
            RBs.push_back(occupiedSlots);
        }
        if (RBoccupiedByPacket > 0) {
            // we start filling a new RB of the frame
            int RBoccupied = std::ceil(RBoccupiedByPacket);
            for(int i = 0; i < RBoccupied; ++i){
                // keep track of each RB (of the frame) occupied by the packet into the RBs vector of the packet
                // and increase the counter of the occupied slots into the frame
                RBs.push_back(++occupiedSlots);
            }
            // compute #freeBytes left from the last occupied RB
            freeBytesFromLastRB = (RBsize - ((packetSize - freeBytesFromLastRB) % RBsize));
        } else {
            // the new packet fits into the current RB of the frame
            freeBytesFromLastRB -= packetSize;
        }

        currentPacket->setRBs(RBs);
        frame->setRBslotsUsed(occupiedSlots);
        // insert current packet into the frame
        packets.push_back(currentPacket);
        // remove current packet from the queue
        queue->remove(currentPacket);
    }

    // update packets in the frame
    frame->setPacketList(packets);
    if (emptySlots == 0)
        // frame is ready
        return true;
    // otherwise the frame has still some empty spot to fill
    return false;
}

void BaseStation::assembleFrame(){
    clearFrame();
    std::vector<UserQueue*> servedUsers;
    bool readyToSend = false;
    for (int i = 0; i < nUsers && !readyToSend; i++){
        UserQueue *queue = check_and_cast<UserQueue*>(RRqueues->get(i));
        if(queue->isEmpty())
            // no packets to transmit
            continue;

        servedUsers.push_back(queue);
        readyToSend = insertIntoFrame(frame, queue);
    }
    for (int i = 0; i < servedUsers.size(); i++) {
        UserQueue *q = servedUsers[i];
        // we implement the Round Robin policy simply by considering a FIFO queue (using RRqueues)
        // backlogged users are served cyclically, in a fixed order
        RRqueues->remove(q);
        RRqueues->insert(q);
    }
    servedUsers.clear();
}

void BaseStation::sendFrame(){
    assembleFrame();
    int occupiedSlots = frame->getRBslotsUsed();

    emit(simFrame, occupiedSlots);

    for (int i = 0; i < nUsers; i++){
        Frame *f = new Frame(*frame);
        send(f, "out", i);
    }

    scheduleSelfMessage();
}

void BaseStation::storePacket(cMessage *msg){
    //store packet in queue
}

void BaseStation::updateCQI(int cqi, int id){
    currentCQI[id] = cqi;
}

void BaseStation::handleMessage(cMessage *msg){
    if (msg->isSelfMessage()){
        sendFrame();
    }

    if(strcmp(msg->getName(),"CQI") == 0){

        CQImsg *m = check_and_cast<CQImsg*>(msg);

        updateCQI(m->getNewCQI(), m->getArrivalGate()->getIndex());
    }
    if (strcmp(msg->getName(), "Packet") == 0){
        storePacket(msg);
    }
}
} /* namespace pecsn_project */
