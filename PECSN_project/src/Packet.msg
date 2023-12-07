
namespace pecsn_project;



packet Packet {
    int destination;
    int length;
    simtime_t arrivalTime; //same as time of generation
}
