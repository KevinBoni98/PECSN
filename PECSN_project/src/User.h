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
#include <omnetpp.h>
#include <vector>
#include "CQImsg_m.h"
#include "Frame_m.h"
#ifndef USER_H_
#define USER_H_

using namespace omnetpp;

namespace pecsn_project {

class User : public cSimpleModule{
private:
    simsignal_t packetsReceived;
    simsignal_t packetDelay;
    simsignal_t throughput;
    int id;
    int nUsers;
    int rngIndex;
    int cqi;
    std::string distribution;
    double p;
public:
    User();
    virtual ~User();
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage*);
    virtual void sendCQI();
};

} /* namespace pecsn_project */

#endif /* USER_H_ */
