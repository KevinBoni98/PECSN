

package pecsn_project;


simple BaseStation
{
    parameters:
        string CQIValues = default("3 3 6 11 15 20 25 36 39 50 63 72 80 93 93");
        int CQIArrayLength = default(15);
        int frameSize = default(25);
    gates:
        input packet_in[];
        input CQI_in[];
        output frame_out[];
}
