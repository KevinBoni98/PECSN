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

#ifndef PACKETGENERATOR_H_
#define PACKETGENERATOR_H_
#include <omnetpp.h>
#include <vector>

using namespace omnetpp;

namespace pecsn_project {

class PacketGenerator : public cSimpleModule{
private:
    cMessage *beep;
    int indexArrivalTimeGen;
    int indexPkgSizeGen;
    double  lambda;
    bool constantValues;
public:
    PacketGenerator();
    virtual ~PacketGenerator();
protected:
    virtual void handleMessage(cMessage*);
    virtual void initialize();
};

} /* namespace pecsn_project */

#endif /* PACKETGENERATOR_H_ */
