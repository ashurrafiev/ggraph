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

class GenScaleFreeOpt : public Generator {
public:
	int32_t nnodes, nedges;
	
	GenScaleFreeOpt(int32_t n, int32_t e) : nnodes(n), nedges(e) {}
	
	virtual void generate(Graph& g) {
		if(nedges<nnodes)
			nedges = nnodes;
		addNodes(g, nnodes);

		size_t *edgeList = new size_t[nedges*2];
		size_t edges = 0;
		g.addEdgePair(0, 1, randomEdgeCost());
		edgeList[edges++] = 0;
		edgeList[edges++] = 1;
		float mi = 0.0;
		for(size_t i=2; i<nnodes; i++) {
			mi += (float)(nedges-edges/2) / (nnodes-i);
			size_t dst = edgeList[random.nextInt(edges)];
			g.addEdgePair(i, dst, randomEdgeCost());
			mi -= 1.0;
			edgeList[edges++] = i;
			edgeList[edges++] = dst;

			size_t misses = 0;
			while(mi>=i+1 && misses<i) {
				size_t src = edgeList[random.nextInt(edges)];
				dst = edgeList[random.nextInt(edges)];
				if(dst!=src) {
					if(g.nodes[src].findEdge(dst)==NOT_FOUND) {
						g.addEdgePair(src, dst, randomEdgeCost());
						mi -= 1.0;
						edgeList[edges++] = src;
						edgeList[edges++] = dst;
					}
					else {
						misses++;
					}
				}
			}
		}
		delete[] edgeList;
	}
};

class GenScaleFree : public Generator {
public:
	int32_t nnodes, nedges;
	float a;
	
	GenScaleFree(int32_t n, int32_t e) : nnodes(n), nedges(e), a(1.0) {}
	GenScaleFree(int32_t n, int32_t e, float alpha) : nnodes(n), nedges(e), a(alpha) {}
	
	size_t scaleFreeRandomNode(Graph& g, size_t nodes, float max, float a) {
		if(max>0 && nodes>0) {
			float wn = max * (1.0 - a) / nodes;
			float x = random.nextFloat() * max;
			for(size_t j=0; j<nodes; j++) {
				float w = g.nodes[j].edges.size() * a + wn; 
				if(x < w)
					return j;
				x -= w;
			}
		}
		return 0;
	}

	size_t scaleFreeRandomNodeMax(Graph& g, size_t nodes) {
		size_t max = 0;
		for(size_t j=0; j<nodes; j++)
			max += g.nodes[j].edges.size();
		return max;
	}
	
	virtual void generate(Graph& g) {
		if(nedges<nnodes)
			nedges = nnodes;
		addNodes(g, nnodes);

		size_t edges = 0;
		float mi = 0.0;
		for(size_t i=1; i<nnodes; i++) {
			mi += (float)(nedges-edges) / (nnodes-i);
			size_t max = scaleFreeRandomNodeMax(g, i+1);
			size_t dst = scaleFreeRandomNode(g, i, max, 1.0);
			if(dst!=i) {
				g.addEdgePair(i, dst, randomEdgeCost());
				mi -= 1.0;
				edges++;
			}
			size_t misses = 0;
			while(mi>=i+1 && misses<i) {
				max = scaleFreeRandomNodeMax(g, i+1);
				size_t src = scaleFreeRandomNode(g, i+1, max, a);
				dst = scaleFreeRandomNode(g, i+1, max, 1.0);
				if(dst!=src) {
					if(g.nodes[src].findEdge(dst)==NOT_FOUND) {
						g.addEdgePair(src, dst, randomEdgeCost());
						mi -= 1.0;
						edges++;
					}
					else {
						misses++;
					}
				}
			}
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

class GenRandGrid : public GenGrid2D8 {
public:
	float randRatio;
	
	GenRandGrid(int32_t size, float ratio) : GenGrid2D8(size, size), randRatio(ratio) {}
	
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
							if(random.nextFloat()<randRatio) {
								size_t src, dst;
								do {
									do {
										src = random.nextInt(w*h);
										dst = random.nextInt(w*h);
									} while(src==dst);
								} while(!g.addEdgePair(src, dst, randomEdgeCost()));
							}
							else {
								if(s<d)
									g.addEdgePair(s, d, randomEdgeCost());
							}
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
