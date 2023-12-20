

package pecsn_project;


simple BaseStation
{
    parameters:
        @signal[packetsInSpecificQueue*](type="int");
        @statisticTemplate[packetsInSpecificQueueStat](title="Packets In a specific Queue"; record=vector,stats;);
        @signal[packetsInQueue](type="int");
        @statistic[packetsInQueueStat](title="Packets In Queue"; source="packetsInQueue"; record=vector,stats;);
        @signal[simFrame](type="int");
        @statistic[simFrameStat](title="Occupied frame slots"; source="simFrame"; record=vector,stats;);
        string CQIValues = default("3 3 6 11 15 20 25 36 39 50 63 72 80 93 93");
        int CQIArrayLength = default(15);
        int frameSize = default(25);
        double timeSlot = default(2);
    gates:
        input packet_in[];
        input CQI_in[];
        output frame_out[];
}
