

package pecsn_project;


simple User
{
    parameters:
        string CQIdistribution = default("binomial");
    gates:
        input frame_in;
        output CQI_out;
}
