cplusplus{{
	#include <vector>
	#include "Packet_m.h"
	
	typedef std::vector<Packet *> PacketVector;
}}

class PacketVector{
    @existingClass;
}

packet Frame {
    int RBused;
    PacketVector packets;
}