#ifndef _GRAPHALG_H_
#define _GRAPHALG_H_

#include "graph.h"

template<typename BinOp> int64_t ssspHopsT(Graph& g, size_t n, size_t buf[], bool vis[], BinOp accum) {
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
		
		size_t nodeIndex = buf[cur];
		Node& node = g.nodes[nodeIndex];
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
	size_t buf[g.nodeCount()] = {};
	bool vis[g.nodeCount()];
	
	return ssspHopsT(g, n, buf, vis, accum);
}

template<typename BinOp> int64_t apspHopsT(Graph& g, BinOp accum) {
	size_t buf[g.nodeCount()] = {};
	bool vis[g.nodeCount()];
	
	int64_t len = 0;
	for(size_t n=0; n<g.nodes.size(); n++) {
		len = accum(len, ssspHopsT(g, n, buf, vis, accum));
	}
	return len;
}

std::function<int64_t (const int64_t, const int64_t)> maxFunc = [](int64_t x, int64_t y) { return std::max(x, y); };

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

#endif
