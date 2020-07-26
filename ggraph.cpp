#include <iostream>
#include <functional>
#include <string.h>

#include "gennormal.h"
#include "graphalg.h"
#include "profile.h"

#define PROFILE(opt, act) if(opt) { profile.begin(); act; profile.print(); cout << endl; }

using namespace std;

char* cmd;

int help() {
	cerr << "Usage: " << cmd << " options generator" << endl;
	
	cerr << endl << "Options:" << endl;
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
	cerr << "-a, -analysis" << endl
		<< "\tTrigger all analysis options" << endl;
	cerr << "-t, -time" << endl
		<< "\tPrint time profile of analysis functions" << endl;
	
	cerr << endl << "Generators:" << endl;
	cerr << "-norm <nnodes> <nedgepairs>" << endl
		<< "\tRandom graph with normal degree distribution" << endl;

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
	
	Profile profile(false);
	
	for(int i=1; i<argc; i++) {
		if(strcmp(argv[i], "-norm")==0) {
			if(i+2>=argc)
				return help();
			int32_t nnodes = atoi(argv[++i]);
			int32_t nedges = atoi(argv[++i]);
			gen = new GenNormal(nnodes, nedges);
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
		else if(strcmp(argv[i], "-a")==0 || strcmp(argv[i], "-analysis")==0) {
			doCheckConnected = true;
			doSsspHopsSum = true;
			doSsspHopsMax = true;
			doApspHopsSum = true;
			doApspHopsMax = true;
		}
		else {
			cerr << "Unknown option: " << argv[i] << endl;
			return help();
		}
	}
	if(!gen)
		return help();
	
	gen->ecost(10, 1000);
	
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
	
	return 0;
}