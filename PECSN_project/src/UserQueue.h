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
#ifndef USERQUEUE_H_
#define USERQUEUE_H_

#include <omnetpp.h>


using namespace omnetpp;

namespace pecsn_project {

class UserQueue: public omnetpp::cQueue {
public:
    int RBsize;
    long byteSent;

    UserQueue(const char *name);
    virtual ~UserQueue();

    static int queuecmp(cObject *a, cObject *b);
};
}

#endif /* USERQUEUE_H_ */
