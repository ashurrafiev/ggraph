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
	
	virtual void generate(Graph& g) {
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
	
	virtual void generate(Graph& g) {
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
	
	virtual void generate(Graph& g) {
		generateTree(g, depth);
	}
};

class GenGrid2D8 : public Generator {
public:
	int32_t w, h;
	
	GenGrid2D8(int32_t width, int32_t height) : w(width), h(height) {}
	
	size_t index(int32_t i, int32_t j) {
		return j*w+i;
	}

	bool inRange(int32_t i, int32_t j) {
		return i>=0 && i<w && j>=0 && j<h;
	}
	
	virtual bool hasEdge(int32_t ri, int32_t rj) {
		return ri!=0 || rj!=0;
	}
	
	virtual void generate(Graph& g) {
		addNodes(g, w*h);
		for(int32_t sj=0; sj<h; sj++)
			for(int32_t si=0; si<w; si++) {
				size_t s = index(si, sj);
				for(int32_t rj=-1; rj<=1; rj++)
					for(int32_t ri=-1; ri<=1; ri++) {
						if(hasEdge(ri, rj)) {
							int32_t di = si+ri;
							int32_t dj = sj+rj;
							if(!inRange(di, dj))
								continue;
							size_t d = index(di, dj);
							if(s<d)
								g.addEdgePair(s, d, randomEdgeCost());
						}
					}
			}
	}
};

class GenGrid2D4 : public GenGrid2D8 {
public:
	GenGrid2D4(int32_t width, int32_t height) : GenGrid2D8(width, height) {}
	
	virtual bool hasEdge(int32_t ri, int32_t rj) {
		return (ri!=0)!=(rj!=0);
	}
	
};

class GenGrid3D26 : public Generator {
public:
	int32_t w, l, h;
	
	GenGrid3D26(int32_t width, int32_t length, int32_t height) : w(width), l(length), h(height) {}
	
	size_t index(int32_t i, int32_t j, int32_t k) {
		return (k*l+j)*w+i;
	}

	bool inRange(int32_t i, int32_t j, int32_t k) {
		return i>=0 && i<w && j>=0 && j<l && k>=0 && k<h;
	}
	
	virtual bool hasEdge(int32_t ri, int32_t rj, int32_t rk) {
		return ri!=0 || rj!=0 || rk!=0;
	}
	
	virtual void generate(Graph& g) {
		addNodes(g, w*l*h);
		for(int32_t sk=0; sk<h; sk++)
			for(int32_t sj=0; sj<l; sj++)
				for(int32_t si=0; si<w; si++) {
					size_t s = index(si, sj, sk);
					for(int32_t rk=-1; rk<=1; rk++)
						for(int32_t rj=-1; rj<=1; rj++)
							for(int32_t ri=-1; ri<=1; ri++) {
								if(hasEdge(ri, rj, rk)) {
									int32_t di = si+ri;
									int32_t dj = sj+rj;
									int32_t dk = sk+rk;
									if(!inRange(di, dj, dk))
										continue;
									size_t d = index(di, dj, dk);
									if(s<d)
										g.addEdgePair(s, d, randomEdgeCost());
								}
							}
				}
	}
};

class GenGrid3D6 : public GenGrid3D26 {
public:
	GenGrid3D6(int32_t width, int32_t length, int32_t height) : GenGrid3D26(width, length, height) {}
	
	virtual bool hasEdge(int32_t ri, int32_t rj, int32_t rk) {
		return (ri!=0)!=(rj!=0) && (rk==0) || (ri==0) && (rj==0) && (rk!=0);
	}
};

#endif
