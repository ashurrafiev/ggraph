#include <iostream>
#include <functional>

#include "gennormal.h"
#include "graphalg.h"

int main(int argc, char *argv[]) {
	if(argc < 3) {
		std::cerr << "Usage: " << argv[0] << " nnodes nedges" << std::endl;
		return 1;
	}
	int32_t nnodes = atoi(argv[1]);
	int32_t nedges = atoi(argv[2]);
		
	Graph g;
	GenNormal gen(nnodes, nedges);
	gen.ecost(10, 1000);
	gen.generate(g);
	//g.print();
	
	std::cout << std::endl;
	
	std::cout << (checkConnected(g) ? "# Connected" : "# Not connected") << std::endl;
	std::cout << "# SSSP(0) sum hops = " << ssspHopsSum(g, 0) << std::endl;
	std::cout << "# SSSP(0) max hops = " << ssspHopsMax(g, 0) << std::endl;
	std::cout << "# APSP sum hops = " << apspHopsSum(g) << std::endl;
	std::cout << "# APSP max hops = " << apspHopsMax(g) << std::endl;
	
	return 0;
}