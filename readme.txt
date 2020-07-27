Usage: ./ggraph options generator

Options:
-ncost <min> <max>
        Generate node costs within range, default: [0, 0]
-ecost <min> <max>
        Generate edge costs within range, default: [10, 1000]
-p, -print
        Print graph
-con
        Analysis: check connectivity
-sssp-hops-sum
        Analysis: single source shortest paths sum (hops)
-sssp-hops-max
        Analysis: max of single source shortest paths (hops)
-apsp-hops-sum
        Analysis: all-pair shortest paths sum (hops)
-apsp-hops-max
        Analysis: max of all-pair shortest paths (hops)
-sssp-costs-sum
        Analysis: single source shortest paths sum (costs)
-sssp-costs-max
        Analysis: max of single source shortest paths (costs)
-apsp-costs-sum
        Analysis: all-pair shortest paths sum (costs)
-apsp-costs-max
        Analysis: max of all-pair shortest paths (costs)
-sssp
        Trigger all SSSP analysis options
-apsp
        Trigger all APSP analysis options
-a, -analysis
        Trigger all analysis options
-t, -time
        Print time profile of analysis functions

Generators:
-norm <nnodes> <nedgepairs>
        Random graph with normal degree distribution
-ring <nnodes>
        Bidirectional ring graph
-tree <degree> <depth>
        Bidirectional tree graph

