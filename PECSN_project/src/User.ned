

package pecsn_project;


simple User
{
    parameters:
        @signal[packetDelay](type="double");
        @statistic[packetDelayStat](title="Packet delay"; source="packetDelay"; record=vector,stats;);
        @signal[packetsReceived](type="int");
        @statistic[packetsReceivedStat](title="Packets received per timeslot"; source="packetsReceived"; record=vector,stats;);
        @signal[throughput](type="double");
        @statistic[throughputStat](title="Throughput"; source="throughput"; record=vector,stats;);
        
        //@signal[userThroughputTotal](type="double");
        //@statistic[userThroughputTotalStat](title="User throughput"; source="userThroughputTotal"; record=stats;);
        string CQIdistribution = default("binomial");
    gates:
        input frame_in;
        output CQI_out;
}
