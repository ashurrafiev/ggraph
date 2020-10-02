#ifndef _FLIMIT_H_
#define _FLIMIT_H_

#include "graph.h"

void fanoutLimitRing(Graph* g, Graph* go, size_t limit) {
	size_t nnodes = g->nodes.size();
	size_t* map = new size_t[nnodes];
	uint8_t* add = new uint8_t[nnodes];
	for(int i=0; i!=nnodes; i++) {
		map[i] = i;
		add[i] = 0;
		go->addNode(0);
	}
	for(size_t i=0; i!=nnodes; i++) {
		Node node = g->nodes[i];
		for(size_t j=0; j!=node.edges.size(); j++) {
			Edge e = node.edges[j];
			if(e.dst<i)
				continue;
			size_t src = map[i];
			size_t dst = map[e.dst];
			// check and split src
			if(go->nodes[src].edges.size()>=limit+add[i]) {
				size_t x = go->addNode(1);
				go->addEdgePair(src, x, 0);
				map[i] = x;
				if(add[i]<2) add[i]++;
				src = x;
			}
			// check and split dst
			if(go->nodes[dst].edges.size()>=limit+add[e.dst]) {
				size_t x = go->addNode(1);
				go->addEdgePair(dst, x, 0);
				map[e.dst] = x;
				if(add[e.dst]<2) add[e.dst]++;
				dst = x;
			}
			go->addEdgePair(src, dst, e.cost);
		}
	}
	// close ring
	for(size_t i=0; i!=nnodes; i++) {
		if(add[i]>=2)
			go->addEdgePair(i, map[i], 0);
	}
	delete[] map;
	delete[] add;
}

#endif
