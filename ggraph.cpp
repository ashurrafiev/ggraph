#include <iostream>
#include <functional>
#include <string.h>

#include "graphalg.h"
#include "profile.h"
#include "gen.h"

#define PROFILE(opt, act) if(opt) { profile.begin(); act; profile.print(); cout << endl; }
#define SET_GEN(g) if(gen) { cerr << "Generator already defined" << endl; return help(); } else gen = g;

using namespace std;

char* cmd;

int help() {
	cerr << "Usage: " << cmd << " options generator" << endl;
	
	cerr << endl << "Options:" << endl;
	cerr << "-seed <seed>" << endl
		<< "\tSet random seed for generator, default is based on current time in ms" << endl;
	cerr << "-ncost <min> <max>" << endl
		<< "\tGenerate node costs within range, default: [0, 0]" << endl;
	cerr << "-ecost <min> <max>" << endl
		<< "\tGenerate edge costs within range, default: [10, 1000]" << endl;
	cerr << "-p, -print" << endl
		<< "\tPrint graph" << endl;
	cerr << "-dd, -ddist" << endl
		<< "\tPrint degree distribution" << endl;
	cerr << "-root" << endl
		<< "\tSuggest root node" << endl;
	cerr << "-con" << endl
		<< "\tAnalysis: check connectivity" << endl;
	cerr << "-sssp-hops-sum" << endl
		<< "\tAnalysis: single source shortest paths sum (hops)" << endl;
	cerr << "-sssp-hops-max" << endl
		<< "\tAnalysis: max of single source shortest paths (hops)" << endl;
	cerr << "-apsp-hops-sum" << endl
		<< "\tAnalysis: all-pair shortest paths sum (hops)" << endl;
	cerr << "-apsp-hops-max" << endl
		<< "\tAnalysis: max of all-pair shortest paths (hops)" << endl;
	cerr << "-sssp-costs-sum" << endl
		<< "\tAnalysis: single source shortest paths sum (costs)" << endl;
	cerr << "-sssp-costs-max" << endl
		<< "\tAnalysis: max of single source shortest paths (costs)" << endl;
	cerr << "-apsp-costs-sum" << endl
		<< "\tAnalysis: all-pair shortest paths sum (costs)" << endl;
	cerr << "-apsp-costs-max" << endl
		<< "\tAnalysis: max of all-pair shortest paths (costs)" << endl;
	cerr << "-sssp" << endl
		<< "\tTrigger all SSSP analysis options" << endl;
	cerr << "-apsp" << endl
		<< "\tTrigger all APSP analysis options" << endl;
	cerr << "-a, -analysis" << endl
		<< "\tTrigger all analysis options" << endl;
	cerr << "-t, -time" << endl
		<< "\tPrint time profile of analysis functions" << endl;
	
	cerr << endl << "Generators:" << endl;
	cerr << "-norm <nnodes> <nedgepairs>" << endl
		<< "\tRandom graph with normal degree distribution" << endl;
	cerr << "-sfree <nnodes> <nedgepairs>" << endl
		<< "\tRandom scale-free graph" << endl;
	cerr << "-sfreea <nnodes> <nedgepairs> <alpha>" << endl
		<< "\tRandom scale-free aproximate graph with approximation parameter alpha (float in [0.0, 1.0])" << endl;
	cerr << "-ring <nnodes>" << endl
		<< "\tBidirectional ring graph" << endl;
	cerr << "-tree <degree> <depth>" << endl
		<< "\tBidirectional balanced tree graph" << endl;
	cerr << "-grid2d4 <w> <h>" << endl
		<< "\t4-connected 2D grid" << endl;
	cerr << "-grid2d8 <w> <h>" << endl
		<< "\t8-connected 2D grid" << endl;
	cerr << "-grid3d6 <w> <h>" << endl
		<< "\t6-connected 3D grid" << endl;
	cerr << "-grid3d26 <w> <h>" << endl
		<< "\t26-connected 3D grid" << endl;
	cerr << "-randgrid <size> <ratio%>" << endl
		<< "\t(Undocumented)" << endl;

	cerr << endl;
	return 1;
}

int main(int argc, char *argv[]) {
	cmd = argv[0];
	
	Generator *gen = 0;
	uint64_t seed = currentTimeMillis();
	bool doPrint = false;
	bool doPrintDDist = false;
	bool doRootNode = false;
	bool doCheckConnected = false;
	bool doSsspHopsSum = false;
	bool doSsspHopsMax = false;
	bool doApspHopsSum = false;
	bool doApspHopsMax = false;
	bool doSsspCostsSum = false;
	bool doSsspCostsMax = false;
	bool doApspCostsSum = false;
	bool doApspCostsMax = false;
	
	int32_t nmin = 0;
	int32_t nmax = 0;
	int32_t emin = 10;
	int32_t emax = 1000;
	size_t rootNode = 0;
	
	Profile profile(false);
	
	for(int i=1; i<argc; i++) {
		if(strcmp(argv[i], "-norm")==0) {
			if(i+2>=argc)
				return help();
			int32_t nnodes = atoi(argv[++i]);
			int32_t nedges = atoi(argv[++i]);
			SET_GEN(new GenNormal(nnodes, nedges));
		}
		else if(strcmp(argv[i], "-sfree")==0) {
			if(i+2>=argc)
				return help();
			int32_t nnodes = atoi(argv[++i]);
			int32_t nedges = atoi(argv[++i]);
			SET_GEN(new GenScaleFreeOpt(nnodes, nedges));
		}
		else if(strcmp(argv[i], "-sfreea")==0) {
			if(i+3>=argc)
				return help();
			int32_t nnodes = atoi(argv[++i]);
			int32_t nedges = atoi(argv[++i]);
			float alpha = atof(argv[++i]);
			SET_GEN(new GenScaleFree(nnodes, nedges, alpha));
		}
		else if(strcmp(argv[i], "-ring")==0) {
			if(i+1>=argc)
				return help();
			int32_t nnodes = atoi(argv[++i]);
			SET_GEN(new GenRing(nnodes));
		}
		else if(strcmp(argv[i], "-tree")==0) {
			if(i+2>=argc)
				return help();
			int32_t base = atoi(argv[++i]);
			int32_t depth = atoi(argv[++i]);
			SET_GEN(new GenTree(base, depth));
		}
		else if(strcmp(argv[i], "-grid2d4")==0) {
			if(i+2>=argc)
				return help();
			int32_t w = atoi(argv[++i]);
			int32_t h = atoi(argv[++i]);
			SET_GEN(new GenGrid2D4(w, h));
			rootNode = ((GenGrid2D4 *) gen)->index(w/2, h/2);
		}
		else if(strcmp(argv[i], "-grid2d8")==0) {
			if(i+2>=argc)
				return help();
			int32_t w = atoi(argv[++i]);
			int32_t h = atoi(argv[++i]);
			SET_GEN(new GenGrid2D8(w, h));
			rootNode = ((GenGrid2D8 *) gen)->index(w/2, h/2);
		}
		else if(strcmp(argv[i], "-grid3d6")==0) {
			if(i+3>=argc)
				return help();
			int32_t w = atoi(argv[++i]);
			int32_t l = atoi(argv[++i]);
			int32_t h = atoi(argv[++i]);
			SET_GEN(new GenGrid3D6(w, l, h));
			rootNode = ((GenGrid3D6 *) gen)->index(w/2, l/2, h/2);
		}
		else if(strcmp(argv[i], "-grid3d26")==0) {
			if(i+3>=argc)
				return help();
			int32_t w = atoi(argv[++i]);
			int32_t l = atoi(argv[++i]);
			int32_t h = atoi(argv[++i]);
			SET_GEN(new GenGrid3D26(w, l, h));
			rootNode = ((GenGrid3D26 *) gen)->index(w/2, l/2, h/2);
		}
		else if(strcmp(argv[i], "-randgrid")==0) {
			if(i+2>=argc)
				return help();
			int32_t size = atoi(argv[++i]);
			float ratio = (float)atoi(argv[++i]) / 100.0;
			SET_GEN(new GenRandGrid(size, ratio));
			rootNode = ((GenRandGrid *) gen)->index(size/2, size/2);
		}
		else if(strcmp(argv[i], "-seed")==0) {
			if(i+1>=argc)
				return help();
			seed = (uint64_t) strtoull(argv[++i], 0, 10);
		}
		else if(strcmp(argv[i], "-ncost")==0) {
			if(i+2>=argc)
				return help();
			nmin = atoi(argv[++i]);
			nmax = atoi(argv[++i]);
		}
		else if(strcmp(argv[i], "-ecost")==0) {
			if(i+2>=argc)
				return help();
			emin = atoi(argv[++i]);
			emax = atoi(argv[++i]);
		}
		else if(strcmp(argv[i], "-p")==0 || strcmp(argv[i], "-print")==0)
			doPrint = true;
		else if(strcmp(argv[i], "-dd")==0 || strcmp(argv[i], "-ddist")==0)
			doPrintDDist = true;
		else if(strcmp(argv[i], "-root")==0)
			doRootNode = true;
		else if(strcmp(argv[i], "-t")==0 || strcmp(argv[i], "-time")==0)
			profile.enabled = true;
		else if(strcmp(argv[i], "-con")==0)
			doCheckConnected = true;
		else if(strcmp(argv[i], "-sssp-hops-sum")==0)
			doSsspHopsSum = true;
		else if(strcmp(argv[i], "-sssp-hops-max")==0)
			doSsspHopsMax = true;
		else if(strcmp(argv[i], "-apsp-hops-sum")==0)
			doApspHopsSum = true;
		else if(strcmp(argv[i], "-apsp-hops-max")==0)
			doApspHopsMax = true;
		else if(strcmp(argv[i], "-sssp-costs-sum")==0)
			doSsspCostsSum = true;
		else if(strcmp(argv[i], "-sssp-costs-max")==0)
			doSsspCostsMax = true;
		else if(strcmp(argv[i], "-apsp-costs-sum")==0)
			doApspCostsSum = true;
		else if(strcmp(argv[i], "-apsp-costs-max")==0)
			doApspCostsMax = true;
		else if(strcmp(argv[i], "-sssp")==0) {
			doCheckConnected = true;
			doSsspHopsSum = true;
			doSsspHopsMax = true;
			doSsspCostsSum = true;
			doSsspCostsMax = true;
		}
		else if(strcmp(argv[i], "-apsp")==0) {
			doCheckConnected = true;
			doApspHopsSum = true;
			doApspHopsMax = true;
			doApspCostsSum = true;
			doApspCostsMax = true;
		}
		else if(strcmp(argv[i], "-a")==0 || strcmp(argv[i], "-analysis")==0) {
			doCheckConnected = true;
			doSsspHopsSum = true;
			doSsspHopsMax = true;
			doApspHopsSum = true;
			doApspHopsMax = true;
			doSsspCostsSum = true;
			doSsspCostsMax = true;
			doApspCostsSum = true;
			doApspCostsMax = true;
		}
		else {
			cerr << "Unknown option: " << argv[i] << endl;
			return help();
		}
	}
	if(!gen)
		return help();
	
	gen->ncost(nmin, nmax);
	gen->ecost(emin, emax);
	
	Graph g;
	
	profile.begin();
	gen->generate(g, seed);
	double genTime = profile.end();
	delete gen;
	
	if(doPrint)
		g.print();
	else
		cout << g.nodes.size() << " " << g.edgeCount() << " " << g.maxFanout() <<endl;
	cout << endl;
	
	cout << "$seed " << seed << endl;
	if(doRootNode)
		cout << "$rootNode " << rootNode << endl;
	else
		rootNode = 0;
	
	cout << endl;
	if(profile.enabled) {
		cout << "#";
		profile.print(genTime, "generated");
		cout << endl;
	}
	PROFILE(doCheckConnected, cout << (checkConnected(g) ? "# Connected" : "# Not connected"));
	PROFILE(doSsspHopsSum, cout << "# SSSP(" << rootNode << ") sum hops = " << ssspHopsSum(g, rootNode));
	PROFILE(doSsspHopsMax, cout << "# SSSP(" << rootNode << ") max hops = " << ssspHopsMax(g, rootNode));
	PROFILE(doApspHopsSum, cout << "# APSP sum hops = " << apspHopsSum(g));
	PROFILE(doApspHopsMax, cout << "# APSP max hops = " << apspHopsMax(g));
	PROFILE(doSsspCostsSum, cout << "# SSSP(" << rootNode << ") sum costs = " << ssspCostsSum(g, rootNode));
	PROFILE(doSsspCostsMax, cout << "# SSSP(" << rootNode << ") max costs = " << ssspCostsMax(g, rootNode));
	PROFILE(doApspCostsSum, cout << "# APSP sum costs = " << apspCostsSum(g));
	PROFILE(doApspCostsMax, cout << "# APSP max costs = " << apspCostsMax(g));
	if(doPrintDDist) {
		cout << "# Degree dist: ";
		g.printDDist();
		cout << "# Fanout: mean = " << g.meanFanout() << ", sdev = " << g.sdevFanout() << ", max = " << g.maxFanout() << endl;
	}
	
	return 0;
}