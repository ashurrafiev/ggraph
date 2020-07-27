#ifndef _GEN_H_
#define _GEN_H_

#include "graph.h"
#include "random.h"

class Generator {
public:
	Random random;
	
	int32_t nmin, nmax;
	int32_t emin, emax;

	Generator() : nmin(0), nmax(0), emin(0), emax(0) {}

	const void ncost(int32_t min, int32_t max) {
		nmin = min;
		nmax = max;
	}
	
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

class GenNormal : public Generator {
public:
	int32_t nnodes, nedges;
	
	GenNormal(int32_t n, int32_t e) : nnodes(n), nedges(e) {
		assert(nedges <= (int64_t)nnodes * ((int64_t)nnodes -1)/2);
	}
	
	void generate(Graph& g) {
		addNodes(g, nnodes);
		for(int32_t i=0; i<nedges; i++) {
			size_t src, dst;
			do {
				src = random.nextInt(nnodes);
				dst = random.nextInt(nnodes);
			} while(src==dst);
			g.addEdgePair(src, dst, randomEdgeCost());
		}
	}
	
};

class GenRing : public Generator {
public:
	int32_t nnodes;
	
	GenRing(int32_t n) : nnodes(n) {}
	
	void generate(Graph& g) {
		addNodes(g, nnodes);
		for(int32_t i=0; i<nnodes; i++) {
			g.addEdgePair(i, i>0 ? i-1 : nnodes-1, randomEdgeCost());
		}
	}
};

class GenTree : public Generator {
public:
	int32_t base, depth;

	GenTree(int32_t b, int32_t d) : Generator(), base(b), depth(d) {
		int32_t nnodes = 1;
		for(int32_t i=0; i<depth; i++) {
			assert(nnodes<0x7fffffff/base);
			nnodes *= base;
		}
	}
	
	size_t generateTree(Graph& g, int32_t d) {
		size_t node = g.addNode(randomNodeCost());
		if(d>0) {
			for(int32_t i=0; i<base; i++) {
				size_t c = generateTree(g, d-1);
				g.addEdgePair(node, c, randomEdgeCost());
			}
		}
		return node;
	}
	
	void generate(Graph& g) {
		generateTree(g, depth);
	}
};

#endif
