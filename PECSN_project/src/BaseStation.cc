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
    //inizializzo array dei valori in byte dei CQI
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
            EV<<"found: "<<found<<endl;
            CQITable[i] = found;
            EV<<"value "<<i<<": "<<CQITable[i]<<endl;
            i++;
            if (i >= len) break;
        }
        temp = "";
    }


}
void BaseStation::handle_message(cMessage *msg){

}
} /* namespace pecsn_project */
