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
    id = getIndex();
    rngIndex = id;
    distribution = par("CQIdistribution").stringValue();


    sendCQI();
}

void User::handleMessage(cMessage *msg){
    EV<<"rfs"<<endl;
    Frame *frame = check_and_cast<Frame*>(msg);
    std::vector<Packet*> pl = frame->getPacketList();
    EV<<"size: "<<pl.size()<<endl;
    while(pl.size() != 0){
        Packet * p = pl.back();
        pl.pop_back();
        EV<<"packet length: "<<p->getLength()<<endl;
    }
    sendCQI();
}

void User::sendCQI(){
    /*if(par("useBinomialDistribution").boolValue()){

    }*/
    //else
    if (distribution.compare("uniform") == 0){
        cqi = intuniform(1, 15, rngIndex);
    }
    else if (distribution.compare("binomial") == 0){

    }

    CQImsg * msg = new CQImsg("CQI");
    msg->setNewCQI(cqi);
    EV<<"packet name: "<<msg->getName()<<endl;
    send(msg,"CQI_out");
    EV<<"sending cqi = "<<msg->getNewCQI()<<endl;

}

} /* namespace pecsn_project */


