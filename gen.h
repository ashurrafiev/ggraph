#ifndef _GEN_H_
#define _GEN_H_

#include "graph.h"
#include "random.h"

class Generator {
public:
	Random random;
	int32_t nnodes, nedges;
	
	int32_t nmin, nmax;
	int32_t emin, emax;

	Generator(int32_t n, int32_t e) : nnodes(n), nedges(e), nmin(0), nmax(0), emin(0), emax(0) {}
	
	const void ecost(int32_t min, int32_t max) {
		emin = min;
		emax = max;
	}
	
	virtual void generate(Graph& g) = 0;
	
	void generate(Graph& g, uint64_t seed) {
		random.setSeed(seed);
		generate(g);
	}
	
	void addNodes(Graph& g, int32_t n) {
		for(int32_t i=0; i<n; i++)
			g.addNode(randomNodeCost());
	}
	
	int32_t randomNodeCost() {
		return random.nextInt(nmin, nmax);
	}

	int32_t randomEdgeCost() {
		return random.nextInt(emin, emax);
	}
	
};

#endif
