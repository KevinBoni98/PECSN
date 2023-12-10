
package pecsn_project.simulations;
import pecsn_project.BaseStation;
import pecsn_project.User;
import pecsn_project.PacketGenerator;


network Network
{
    parameters:
        int N_USERS = default(5);
		int lambda = default(10);
		
        @display("bgb=764,368");
    submodules:
        BS: BaseStation {
            @display("p=363,178");
        }
        Users[N_USERS]: User {
            @display("p=81,178");
        }
        PG[N_USERS]: PacketGenerator {

            @display("p=686,178");
        }
    connections:
        for i=0..N_USERS-1 {
            BS.frame_out++ --> Users[i].frame_in;
            BS.CQI_in++ <-- Users[i].CQI_out;
            BS.packet_in++ <-- PG[i].packet_out;
        }


}
