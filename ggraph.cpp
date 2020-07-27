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
	cerr << "-ncost <min> <max>" << endl
		<< "\tGenerate node costs within range, default: [0, 0]" << endl;
	cerr << "-ecost <min> <max>" << endl
		<< "\tGenerate edge costs within range, default: [10, 1000]" << endl;
	cerr << "-p, -print" << endl
		<< "\tPrint graph" << endl;
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
	cerr << "-ring <nnodes>" << endl
		<< "\tBidirectional ring graph" << endl;
	cerr << "-tree <degree> <depth>" << endl
		<< "\tBidirectional tree graph" << endl;

	cerr << endl;
	return 1;
}

int main(int argc, char *argv[]) {
	cmd = argv[0];
	
	Generator *gen = 0;
	bool doPrint = false;
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
	
	Profile profile(false);
	
	for(int i=1; i<argc; i++) {
		if(strcmp(argv[i], "-norm")==0) {
			if(i+2>=argc)
				return help();
			int32_t nnodes = atoi(argv[++i]);
			int32_t nedges = atoi(argv[++i]);
			SET_GEN(new GenNormal(nnodes, nedges));
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
	gen->generate(g);
	double genTime = profile.end();
	delete gen;
	
	if(doPrint)
		g.print();
	else
		cout << g.nodes.size() << " " << g.edgeCount() << " " << g.maxFanout() <<endl;
	cout << endl;
	
	if(profile.enabled) {
		cout << "#";
		profile.print(genTime, "generated");
		cout << endl;
	}
	PROFILE(doCheckConnected, cout << (checkConnected(g) ? "# Connected" : "# Not connected"));
	PROFILE(doSsspHopsSum, cout << "# SSSP(0) sum hops = " << ssspHopsSum(g, 0));
	PROFILE(doSsspHopsMax, cout << "# SSSP(0) max hops = " << ssspHopsMax(g, 0));
	PROFILE(doApspHopsSum, cout << "# APSP sum hops = " << apspHopsSum(g));
	PROFILE(doApspHopsMax, cout << "# APSP max hops = " << apspHopsMax(g));
	PROFILE(doSsspCostsSum, cout << "# SSSP(0) sum costs = " << ssspCostsSum(g, 0));
	PROFILE(doSsspCostsMax, cout << "# SSSP(0) max costs = " << ssspCostsMax(g, 0));
	PROFILE(doApspCostsSum, cout << "# APSP sum costs = " << apspCostsSum(g));
	PROFILE(doApspCostsMax, cout << "# APSP max costs = " << apspCostsMax(g));
	
	return 0;
}