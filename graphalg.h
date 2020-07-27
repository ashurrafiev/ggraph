#ifndef _GRAPHALG_H_
#define _GRAPHALG_H_

#include "graph.h"

template<typename BinOp> int64_t ssspHopsT(Graph& g, size_t n, size_t *buf, bool *vis, BinOp accum) {
	std::fill_n(vis, g.nodeCount(), false);
	int64_t len = 0;
	
	vis[n] = true;
	buf[0] = n;
	int64_t l = 0;
	int32_t cur = 0;
	int32_t lend = 0;
	int32_t end = 0;
	while(cur<=end) {
		if(l>0)
			len = accum(len, l);
		
		Node& node = g.nodes[buf[cur]];
		for(size_t e=0; e<node.edges.size(); e++) {
			size_t dst = node.edges[e].dst;
			if(!vis[dst]) {
				end++;
				buf[end] = dst;
				vis[dst] = true;
			}
		}
		
		cur++;
		if(cur>lend) {
			l++;
			lend = end;
		}
	}
	
	return len;
}

template<typename BinOp> int64_t ssspHopsT(Graph& g, size_t n, BinOp accum) {
	size_t nnodes = g.nodeCount();
	size_t *buf = new size_t[nnodes];
	std::fill_n(buf, nnodes, false);
	bool *vis = new bool[nnodes];
	
	int64_t len = ssspHopsT(g, n, buf, vis, accum);
	
	delete[] buf;
	delete[] vis;
	return len;
}

template<typename BinOp> int64_t apspHopsT(Graph& g, BinOp accum) {
	size_t nnodes = g.nodeCount();
	size_t *buf = new size_t[nnodes];
	std::fill_n(buf, nnodes, false);
	bool *vis = new bool[nnodes];
	
	int64_t len = 0;
	for(size_t n=0; n<nnodes; n++) {
		len = accum(len, ssspHopsT(g, n, buf, vis, accum));
	}
	
	delete[] buf;
	delete[] vis;
	return len;
}

const std::function<int64_t (const int64_t, const int64_t)> maxFunc = [](int64_t x, int64_t y) { return std::max(x, y); };

int64_t ssspHopsSum(Graph& g, size_t n) {
	return ssspHopsT(g, n, std::plus<int64_t>());
}

int64_t ssspHopsMax(Graph& g, size_t n) {
	return ssspHopsT(g, n, maxFunc);
}

int64_t apspHopsSum(Graph& g) {
	return apspHopsT(g, std::plus<int64_t>());
}

int64_t apspHopsMax(Graph& g) {
	return apspHopsT(g, maxFunc);
}

bool checkConnected(Graph& g) {
	return ssspHopsT(g, 0, [](int64_t x, int64_t y) { return x+1; }) == g.nodeCount()-1;
}

template<typename BinOp> int64_t ssspCostsT(Graph& g, size_t n, int64_t *dist, bool *vis, BinOp accum) {
	size_t nnodes = g.nodeCount();
	std::fill_n(dist, nnodes, -1);
	std::fill_n(vis, nnodes, false);
	dist[n] = g.nodes[n].cost;
	
	for(size_t c=0; c<nnodes; c++) {
		int64_t min = -1;
		size_t u;
		for(size_t v=0; v<nnodes; v++) {
			if(!vis[v] && dist[v]>=0 && (min<0 || dist[v]<min)) {
				min = dist[v];
				u = v;
			}
		}
		vis[u] = true;
		for(size_t e=0; e<g.nodes[u].edges.size(); e++) {
			Edge& edge = g.nodes[u].edges[e];
			size_t v = edge.dst;
			if(!vis[v]) {
				int64_t d = dist[u] + edge.cost + g.nodes[v].cost;
				if(dist[v]<0 || d<dist[v])
					dist[v] = d;
			}
		}
	}

	int64_t len = 0;
	for(size_t c=0; c<nnodes; c++) {
		len = accum(len, dist[c]);
	}
	return len;
}

template<typename BinOp> int64_t ssspCostsT(Graph& g, size_t n, BinOp accum) {
	size_t nnodes = g.nodeCount();
	int64_t *dist = new int64_t[nnodes];
	bool *vis = new bool[nnodes];
	
	int64_t len = ssspCostsT(g, n, dist, vis, accum);
	
	delete[] dist;
	delete[] vis;
	return len;
}

template<typename BinOp> int64_t apspCostsT(Graph& g, BinOp accum) {
	size_t nnodes = g.nodeCount();
	int64_t *dist = new int64_t[nnodes];
	bool *vis = new bool[nnodes];
	
	int64_t len = 0;
	for(size_t n=0; n<nnodes; n++) {
		len = accum(len, ssspCostsT(g, n, dist, vis, accum));
	}
	
	delete[] dist;
	delete[] vis;
	return len;
}

int64_t ssspCostsSum(Graph& g, size_t n) {
	return ssspCostsT(g, n, std::plus<int64_t>());
}

int64_t ssspCostsMax(Graph& g, size_t n) {
	return ssspCostsT(g, n, maxFunc);
}

int64_t apspCostsSum(Graph& g) {
	return apspCostsT(g, std::plus<int64_t>());
}

int64_t apspCostsMax(Graph& g) {
	return apspCostsT(g, maxFunc);
}

#endif
