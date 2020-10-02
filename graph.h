#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <math.h>
#include <iostream>
#include <vector>

#define NOT_FOUND ((size_t)(-1))

class Edge {
public:
	size_t dst;
	int32_t cost;
	
	Edge(uint32_t d) : dst(d), cost(0) {}
	Edge(uint32_t d, int32_t c) : dst(d), cost(c) {}
};

class Node {
public:
	size_t index;
	int32_t cost;
	std::vector<Edge> edges;
	
	Node(size_t i) : index(i), cost(0) {}
	Node(size_t i, int32_t c) : index(i), cost(c) {}
	
	size_t addEdge(size_t dst, int32_t c) {
		size_t edge = findEdge(dst);
		if(edge==NOT_FOUND) {
			Edge e(dst, c);
			edge = edges.size();
			edges.push_back(e);
		}
		return edge;
	}
	
	size_t findEdge(size_t dst) {
		for(size_t i=0; i!=edges.size(); i++) {
			if(edges[i].dst == dst)
				return i;
		}
		return NOT_FOUND;
	}
	
	void remove(size_t edgeIndex) {
		edges.erase(edges.begin()+edgeIndex);
	}
};

class Graph {
public:
	std::vector<Node> nodes;
	
	size_t findEdge(size_t src, size_t dst) {
		return nodes[src].findEdge(dst);
	}
	
	size_t addNode(int32_t cost) {
		Node node(nodes.size(), cost);
		nodes.push_back(node);
		return node.index;
	}
	
	size_t addEdge(size_t src, size_t dst, int32_t cost) {
		return nodes[src].addEdge(dst, cost);
	}

	bool addEdgePair(size_t src, size_t dst, int32_t cost) {
		return addEdge(src, dst, cost)!=NOT_FOUND &&
			addEdge(dst, src, cost)!=NOT_FOUND;
	}
	
	void flattenCosts() {
		for(size_t i=0; i!=nodes.size(); i++) {
			Node& node = nodes[i];
			node.cost = (node.cost!=0) ? 1 : 0;
			for(size_t j=0; j!=node.edges.size(); j++) {
				Edge& edge = node.edges[j];
				edge.cost = (edge.cost!=0) ? 1 : 0;
			}
		}
	}
	
	size_t nodeCount() {
		return nodes.size();
	}
	
	size_t edgeCount() {
		int32_t sum = 0;
		for(size_t i=0; i!=nodes.size(); i++) {
			sum += nodes[i].edges.size();
		}
		return sum;
	}
	
	size_t maxFanout() {
		size_t max = 0;
		for(size_t i = 0; i != nodes.size(); i++) {
			size_t f = nodes[i].edges.size();
			if(f>max) max = f;
		}
		return max;
	}
	
	double meanFanout() {
		return (double)edgeCount() / nodes.size();
	}
	
	double sdevFanout() {
		double mean = meanFanout();
		double s = 0;
		for(size_t i = 0; i != nodes.size(); i++) {
			size_t f = nodes[i].edges.size();
			s += (f-mean)*(f-mean);
		}
		return sqrt(s/(nodes.size()-1));
	}
	
	void read(std::istream& is) {
		size_t totalNodes, totalEdges, maxFanout;
		is >> totalNodes >> totalEdges >> maxFanout;
		for(int32_t i=0; i<totalNodes; i++)
			addNode(0);
		for(size_t i = 0; i < totalNodes; i++) {
			Node& node = nodes[i];
			size_t edges;
			is >> node.cost >> edges;
			for(size_t j=0; j<edges; j++) {
				size_t dst;
				int32_t w;
				is >> dst >> w;
				node.addEdge(dst, w);
				totalEdges--;
			}
		}
		// assert(totalEdges==0);
	}
	
	void print(std::ostream& os = std::cout) {
		os << nodes.size() << " " << edgeCount() << " " << maxFanout() <<std::endl;
		for(size_t i=0; i!=nodes.size(); i++) {
			Node& node = nodes[i];
			os << node.cost << " " << node.edges.size() << " ";
			for(size_t j=0; j!=node.edges.size(); j++) {
				Edge& edge = node.edges[j];
				os << edge.dst << " " << edge.cost << " ";
			}
			os << std::endl;
		}
	}
	
	void printDDist(std::ostream& os = std::cout) {
		std::vector<size_t> dd;
		for(size_t i=0; i!=nodes.size(); i++) {
			Node& node = nodes[i];
			size_t f = node.edges.size();
			while(dd.size()<=f)
				dd.push_back(0);
			dd[f] += 1;
		}
		
		bool first = true; 
		for(size_t i=0; i!=dd.size(); i++) {
			if(!first) os << ",";
			os << dd[i];
			first = false;
		}
		os << std::endl;
	}
	
};

#endif
