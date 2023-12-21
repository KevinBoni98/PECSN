

package pecsn_project;


simple PacketGenerator
{
    parameters:
        int NUM_USER;
        double LAMBDA;
        bool ConstantValues = default(false);
    gates:
        output packet_out;
}
