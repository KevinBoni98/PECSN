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
    simFrame = registerSignal("simFrame");
    beep = new cMessage("beep");
    nUsers = getParentModule()->par("NUM_USER");
    currentCQI = new int[nUsers];
    for (int i = 0; i < nUsers; i++){
        currentCQI[i] = 0;
    }
    //inizializzo array dei valori in byte dei CQI
    EV<<"CQIArrayLength:\t"<<par("CQIArrayLength").intValue()<<endl;
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
        EV<<"temp:\t"<<temp<<endl;
        if (std::stringstream(temp) >> found){
            CQITable[i] = found;
            i++;
            if (i >= len) break;
        }
        temp = "";
    }
    frame = new Frame;
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
    EV<<par("timeSlot")<<endl;
    double time = par("timeSlot").doubleValue();
    scheduleAt(simTime() + time, beep);
}

void BaseStation::clearFrame(){
    EV<<"clearFrame..."<<endl;
    frame->setRBslotsUsed(0);
    std::vector<Packet*> packets = frame->getPacketList();
    while(packets.size()) {
        Packet* p = packets.back();
        packets.pop_back();
        delete(p);
    }
    frame->setPacketList(packets);
    EV<<"end clearFrame..."<<endl;
}

// insertIntoFrame insert packet of the user queue into the frame and 
// return true if there is space of the frame  
bool BaseStation::insertIntoFrame(Frame *frame, UserQueue *queue){
    int occupiedRBs = frame->getRBslotsUsed();
    std::vector<Packet*> packets = frame->getPacketList();
    int RBsize = queue->RBsize;
    EV<<"l'RBsize di questa queue e' "<<RBsize<<endl;
    int emptySlots;
    int freeSpace;
    // freeBytesFromLastRB indicates the free space of the last RB used by the same user
    // an RB could be split by two different packages from the same user
    int freeBytesFromLastRB = 0;
    int currentPacketSize;

    Packet* currentPacket;
    // i try to fill the frame with the packets in the current user's queue
    while(!queue->isEmpty()){
        // calculate free space on frame
        // each frame is composed by 25 RBs
        emptySlots = 25 - occupiedRBs;
        freeSpace = emptySlots * RBsize;
        // get first packet on queue
        currentPacket = check_and_cast<Packet*>(queue->get(0));
        currentPacketSize = currentPacket->getLength();
        // check if packet could be inserted on the queue 
        // i can use also byte free from last rb
        if(currentPacketSize > freeSpace + freeBytesFromLastRB){
            break;
        }
        // check if packet could be inserted on the empty space from last rb
        if (currentPacketSize < freeBytesFromLastRB)
        {
            freeBytesFromLastRB -= currentPacketSize;
            // insert current packet into the frame
            packets.push_back(currentPacket);
            // remove current packet from the queue
            queue->remove(currentPacket);
            continue;
        }

        EV<<"dimensione pacchetto:"<<currentPacketSize<<endl;
        EV<<"dimensione libera rb:"<<freeBytesFromLastRB<<endl;
        EV<<"rb occupati fino ad ora:"<<occupiedRBs<<endl;
        // get number of RB occupied by the current packet
        float rbsOccupiedByCurrentPkt = ((float)(currentPacketSize - freeBytesFromLastRB)) / RBsize;
        // update occupiedRBs
        occupiedRBs += std::floor(rbsOccupiedByCurrentPkt); 
        // update free space remain
        freeBytesFromLastRB = rbsOccupiedByCurrentPkt-occupiedRBs;
        EV<<"dimensione libera rb:"<<freeBytesFromLastRB<<endl;
        EV<<"rb occupati aggiornata:"<<occupiedRBs<<endl;
    }

    // update packets in the frame
    frame->setPacketList(packets);
    if (emptySlots == 0){
        // frame is ready
        EV<<"frame is ready"<<endl;
        return true;
    }
    EV<<"still some spots to fill"<<endl;
    // otherwise the frame has still some empty spot to fill
    return false;
}

void BaseStation::assembleFrame(){
    clearFrame();
    bool readyToSend = false;
    int fullLoop = 0; //exit loop if i have checked all users and they have nothing to send
    EV<<"entro nel loop"<<endl;
    while (!readyToSend && fullLoop < nUsers){
        UserQueue *queue = check_and_cast<UserQueue*>(RRqueues->get(toServe));
        if(queue->isEmpty()){
            EV<<"niente da trasmettere"<<endl;
            fullLoop++;
            continue;
        }
        // no packets to transmit
        EV<<"qualcosa vedo"<<endl;
        readyToSend = insertIntoFrame(frame, queue);
        EV<<"ho servito l'utente con id "<<toServe<<endl;
        if (toServe == nUsers-1) 
            toServe = 0;
        else 
            toServe++; //toServe tracks the next user that needs serving
        fullLoop++;
    }
}

void BaseStation::sendFrame(){
    EV<<"sendFrame..."<<endl;
    assembleFrame();
    int occupiedSlots = frame->getRBslotsUsed();
    EV<<"occupiedSlots:\t"<<occupiedSlots<<endl;
    emit(simFrame, occupiedSlots);

    for (int i = 0; i < nUsers; i++){
        Frame *f = new Frame(*frame);
        send(f, "frame_out", i);
    }

    scheduleSelfMessage();
}

void BaseStation::storePacket(cMessage *msg){
    Packet *packet = check_and_cast<Packet*>(msg);
    packet->setArrivalTime(simTime());
    queues[packet->getDestination()]->insert(packet);
}


void BaseStation::updateCQI(int cqi, int id){
    UserQueue *uq = queues[id];
    uq->RBsize = CQITable[cqi-1];
    EV<<"ora l'utente con id "<<id<<" ha cqi = "<<cqi<<", che corrisponde a "<<CQITable[cqi-1]<<" RBsize"<<endl;
}

void BaseStation::handleMessage(cMessage *msg){
    if (msg->isSelfMessage()){
        sendFrame();
    }

    if(strcmp(msg->getName(),"CQI") == 0){

        CQImsg *m = check_and_cast<CQImsg*>(msg);

        updateCQI(m->getNewCQI(), m->getArrivalGate()->getIndex());
        delete(msg);
    }
    if (strcmp(msg->getName(), "Packet") == 0){
        storePacket(msg);
    }
}
} /* namespace pecsn_project */
