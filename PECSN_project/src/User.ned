

package pecsn_project;


simple User
{
    parameters:
        string CQIdistribution = default("binomial");
        @signal[packetDelay](type="double");
        @statistic[packetDelayStat](title="Packet delay"; source="packetDelay"; record=vector,stats;);
        @signal[userThroughput](type="double");
        @statistic[userThroughputStat](title="User throughput"; source="userThroughput"; record=vector,stats;);
        
    gates:
        input frame_in;
        output CQI_out;
}
