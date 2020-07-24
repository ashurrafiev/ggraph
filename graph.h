#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <iostream>
#include <vector>

class Edge {
public:
	size_t dst;
	int32_t cost;
	
	Edge(uint32_t d) : dst(d), cost(0) {}
	Edge(uint32_t d, int32_t c) : dst(d), cost(c) {}
};

class Node {
public:
	int32_t cost;
	std::vector<Edge> edges;
	
	Node() : cost(0) {}
	Node(int32_t c) : cost(c) {}
	
	Edge* addEdge(size_t dst, int32_t c) {
		Edge* edge = findEdge(dst);
		if(!edge) {
			Edge e(dst, c);
			edges.push_back(e);
			edge = &edges.back();
		}
		return edge;
	}
	
	Edge* findEdge(size_t dst) {
		for(size_t i=0; i!=edges.size(); i++) {
			if(edges[i].dst == dst)
				return &edges[i];
		}
		return 0;
	}
};

class Graph {
public:
	std::vector<Node> nodes;
	
	Edge* findEdge(size_t src, size_t dst) {
		return nodes[src].findEdge(dst);
	}
	
	Node* addNode(int32_t cost) {
		Node node(cost);
		nodes.push_back(node);
		return &nodes.back();
	}
	
	Edge* addEdge(size_t src, size_t dst, int32_t cost) {
		return nodes[src].addEdge(dst, cost);
	}

	void addEdgePair(size_t src, size_t dst, int32_t cost) {
		addEdge(src, dst, cost);
		addEdge(dst, src, cost);
	}
	
	int32_t nodeCount() {
		return nodes.size();
	}
	
	int32_t edgeCount() {
		int32_t sum = 0;
		for(size_t i=0; i!=nodes.size(); i++) {
			sum += nodes[i].edges.size();
		}
		return sum;
	}
	
	int32_t maxFanout() {
		int32_t max = 0;
		for(size_t i = 0; i != nodes.size(); i++) {
			int32_t f = nodes[i].edges.size();
			if(f>max) max = f;
		}
		return max;
	}
	
	void print(std::ostream& os = std::cout) {
		os << nodes.size() << " " << edgeCount() << " " << maxFanout() <<std::endl;
		for(size_t i=0; i!=nodes.size(); i++) {
			Node* node = &nodes[i];
			os << node->cost << " ";
			for(size_t j=0; j!=node->edges.size(); j++) {
				Edge* edge = &node->edges[j];
				os << edge->dst << " " << edge->cost << " ";
			}
			os << std::endl;
		}
	}
	
};

#endif
