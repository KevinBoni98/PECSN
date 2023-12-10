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

#include "UserQueue.h"
using namespace omnetpp;
namespace pecsn_project {


UserQueue::UserQueue(const char *name):cQueue(name) {
    byteSent = 0;
}

UserQueue::~UserQueue() {
    // TODO Auto-generated destructor stub
}

int UserQueue::queuecmp(cObject* _a,cObject* _b) {
    UserQueue *a = omnetpp::check_and_cast<UserQueue*>(_a);
    UserQueue *b = omnetpp::check_and_cast<UserQueue*>(_b);
    if (a->byteSent < b->byteSent)
        return -1;
    if (a->byteSent == b->byteSent)
        return 0;
   return 1;
}
}