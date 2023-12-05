

cplusplus {{
#include <vector>
#include "Packet_m.h"
typedef std::vector<pecsn_project::Packet*> PacketVector;
}}

class PacketVector {
    @existingClass;
}

namespace pecsn_project;

//
// TODO generated message class
//
packet Frame {
    int RBslotsUsed;
    PacketVector packetList;
}
