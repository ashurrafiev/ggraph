Usage: ./ggraph options generator

Options:
-ncost <min> <max>
        Generate node cost within range, default is [0, 0]
-ecost <min> <max>
        Generate edge cost within range, default is [10, 1000]
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
