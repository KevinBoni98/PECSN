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

#ifndef BASESTATION_H_
#define BASESTATION_H_
#include <omnetpp.h>
#include <vector>
#include <sstream>
#include "Packet_m.h"
#include "Frame_m.h"
#include "CQImsg_m.h"
#include "UserQueue.h"

using namespace omnetpp;

namespace pecsn_project {

class BaseStation : public cSimpleModule{
private:
    int *CQITable;
    int *currentCQI;
    int nUsers;
    int frameSize;
    double warmup;
    cMessage *beep;
    int toServe;
    UserQueue** queues;
    cQueue *RRqueues;
    Frame *frame;
    simsignal_t simFrame;
    simsignal_t packetsInQueue;
    simsignal_t *packetsInSpecificQueue;


public:
    BaseStation();
    virtual ~BaseStation();
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage*);
    virtual void updateCQI(int,int);
    virtual void scheduleSelfMessage();
    virtual void assembleFrame();
    virtual bool insertIntoFrame(Frame*, UserQueue*);
    virtual void clearFrame();
    virtual void sendFrame();
    virtual void storePacket(cMessage*);
};

} /* namespace pecsn_project */

#endif /* BASESTATION_H_ */
