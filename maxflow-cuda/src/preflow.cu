#include"../include/parallel_graph.cuh"

void preflow(int V, int source, int sink, int *cpu_height, int *cpu_excess_flow, 
             int *offsets, int *destinations, int* capacities, int* forward_flows, int* backward_flows,
             int *roffsets, int* rdestinations, int* flow_idx, int *Excess_total)
{
    // initialising height values and excess flow, Excess_total values
    for(int i = 0; i < V; i++)
    {
        cpu_height[i] = 0; 
        cpu_excess_flow[i] = 0;
    }
    
    cpu_height[source] = V;
    *Excess_total = 0;

    // pushing flow in all edges going out from the source node
    for(int i = offsets[source];  i < offsets[source + 1]; i++) {
        // pushing out of source node
        // cpu_rflowmtx[IDX(source,i)] = 0;
        int neighborID = destinations[i];
        
        if (capacities[i] > 0)  {
            forward_flows[i] = 0;
            backward_flows[i] = capacities[i];
            cpu_excess_flow[neighborID] = capacities[i];
            *Excess_total += cpu_excess_flow[neighborID];
        } else {
            continue;
        }
    }
}