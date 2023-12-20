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
    toServe = 0;
    simFrame = registerSignal("simFrame");
    packetsInQueue = registerSignal("packetsInQueue");
    beep = new cMessage("beep");
    nUsers = getParentModule()->par("NUM_USER");
    packetsInSpecificQueue = new simsignal_t[nUsers];
    for (int i = 0; i < nUsers; i++){
        char signalName[32];
            sprintf(signalName, "packetsInSpecificQueue%d", i);
            simsignal_t signal = registerSignal(signalName);
            cProperty *statisticTemplate = getProperties()->get("statisticTemplate", "packetsInSpecificQueueStat");
            getEnvir()->addResultRecorders(this, signal, signalName,  statisticTemplate);
            packetsInSpecificQueue[i] = signal;
    }
    currentCQI = new int[nUsers];
    for (int i = 0; i < nUsers; i++){
        currentCQI[i] = 0;
    }
    //inizializzo array dei valori in byte dei CQI
    //EV<<"CQIArrayLength:\t"<<par("CQIArrayLength").intValue()<<endl;
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
        //EV<<"temp:\t"<<temp<<endl;
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
    //EV<<par("timeSlot")<<endl;
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
            EV<<"packet di dimensione "<<packetSize<<endl;
            if(packetSize > freeSpace + freeBytesFromLastRB){
                break;
            }
            // #RBs occupied by current packet (packets from the same user can share the same RB)
            float RBoccupiedByPacket = ((float)(packetSize - freeBytesFromLastRB)) / RBsize;


            if (RBoccupiedByPacket > 0) {
                // we start filling a new RB of the frame
                int RBoccupied = std::ceil(RBoccupiedByPacket);
                for(int i = 0; i < RBoccupied; ++i){
                    // keep track of each RB (of the frame) occupied by the packet into the RBs vector of the packet
                    // and increase the counter of the occupied slots into the frame
                    occupiedSlots++;
                }
                // compute #freeBytes left from the last occupied RB
                freeBytesFromLastRB = (RBsize - ((packetSize - freeBytesFromLastRB) % RBsize));
            } else {
                // the new packet fits into the current RB of the frame
                freeBytesFromLastRB -= packetSize;
            }

            frame->setRBslotsUsed(occupiedSlots);
            // insert current packet into the frame
            EV<<"lo inserisco"<<endl;
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
    bool readyToSend = false;
    int fullLoop = 0; //exit loop if i have checked all users and they have nothing to send
    EV<<"entro nel loop"<<endl;
    while (!readyToSend && fullLoop < nUsers){
        UserQueue *queue = check_and_cast<UserQueue*>(RRqueues->get(toServe));
        if(queue->isEmpty()){
            EV<<"niente da trasmettere per utente "<<toServe<<endl;
            if (toServe == nUsers-1)
                toServe = 0;
            else
                toServe++; //toServe tracks the next user that needs serving
            fullLoop++;
            continue;
        }
        // no packets to transmit
        //EV<<"qualcosa vedo"<<endl;
        EV<<"sto servendo l'utente con id "<<toServe<<endl;
        readyToSend = insertIntoFrame(frame, queue);

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
    const char * o = msg->getName();
    if (msg->isSelfMessage()){
        sendFrame();
        int nQueues = getParentModule()->par("NUM_USER");
        double numPacketInQueue = 0;
        for(int i = 0; i < nQueues; i++){
            numPacketInQueue += queues[i]->getLength();
            emit(packetsInSpecificQueue[i],queues[i]->getLength());
        }


       emit(packetsInQueue, numPacketInQueue);

    }

    if(strcmp(msg->getName(),"CQI") == 0){

        CQImsg *m = check_and_cast<CQImsg*>(msg);

        updateCQI(m->getNewCQI(), m->getArrivalGate()->getIndex());
        delete(msg);

    }



    if (strcmp(o, "Packet") == 0){
        storePacket(msg);

    }
}
} /* namespace pecsn_project */
