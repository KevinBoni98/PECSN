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
    //scheduling timer for frame
    scheduleSelfMessage();


}

void BaseStation::scheduleSelfMessage(){
    double time = par("timeSlot").doubleValueInUnit("s");
    scheduleAt(simTime() + time, beep);
}

void BaseStation::assembleFrame(){

}

void BaseStation::sendFrame(){
    assembleFrame();
    //...

    scheduleSelfMessage();

}

void BaseStation::storePacket(cMessage *msg){

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
