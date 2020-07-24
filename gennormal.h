#ifndef _GENNORMAL_H_
#define _GENNORMAL_H_

#include "gen.h"

class GenNormal : public Generator {
public:
	GenNormal(int32_t n, int32_t e) : Generator(n, e) {
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

#endif
