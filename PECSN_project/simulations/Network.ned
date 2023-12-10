
package pecsn_project.simulations;
import pecsn_project.BaseStation;
import pecsn_project.User;
import pecsn_project.PacketGenerator;


network Network
{
    parameters:
        int NUM_USER = default(5);
		int lambda = default(10);
		int CQIRngMultiplier = default(0);
		int PktSizeRngMultiplier = CQIRngMultiplier+1;
		int PktGenerationRngMultiplier = PktSizeRngMultiplier+1;
		
        @display("bgb=764,368");
    submodules:
        BS: BaseStation {
            @display("p=363,178");
        }
        Users[NUM_USER]: User {
            @display("p=81,178");
        }
        PG[NUM_USER]: PacketGenerator {

            @display("p=686,178");
        }
    connections:
        for i=0..NUM_USER-1 {
            BS.frame_out++ --> Users[i].frame_in;
            BS.CQI_in++ <-- Users[i].CQI_out;
            BS.packet_in++ <-- PG[i].packet_out;
        }


}
