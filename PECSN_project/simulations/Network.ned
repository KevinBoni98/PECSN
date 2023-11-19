
package pecsn_project.simulations;
import pecsn_project.Base_Station;
import pecsn_project.User;
import pecsn_project.Packet_Generator;


network Network
{
    parameters:
        int N_USERS = default(5);
		int lambda = default(10);
		
        @display("bgb=764,368");
    submodules:
        BS: Base_Station {
            @display("p=363,178");
        }
        Users[N_USERS]: User {
            @display("p=81,178");
        }
        PG[N_USERS]: Packet_Generator {

            @display("p=686,178");
        }
    connections:
        for i=0..N_USERS {
            BS.frame_out++ --> Users[i].frame_in;
            BS.CQI_in++ <-- Users[i].CQI_out;
            BS.packet_in++ <-- PG[i].packet_out;
        }


}
