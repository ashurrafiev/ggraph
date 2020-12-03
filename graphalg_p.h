#ifndef _GRAPHALG_P_H_
#define _GRAPHALG_P_H_

#include "graphalg.h"

struct ParInfo {
	pthread_t threadId;
	Graph* g;
	std::function<int64_t (const int64_t, const int64_t)> accum;
	size_t nstart, nend;
	int64_t len;
};

void* apspHopsT_Func(void* arg) {
	ParInfo* info = (ParInfo*) arg;
	Graph* g = info->g;
	size_t nstart = info->nstart;
	size_t nnodes = g->nodeCount();
	size_t *buf = new size_t[nnodes];
	std::fill_n(buf, nnodes, false);
	bool *vis = new bool[nnodes];
	
	int64_t len = 0;
	for(size_t n=info->nstart; n<=info->nend; n++) {
		if(!ignoreWeightedNodes || !g->nodes[n].cost)
			len = info->accum(len, ssspHopsT(*g, n, buf, vis, info->accum));
	}
	
	delete[] buf;
	delete[] vis;
	info->len = len;
	return NULL;
}

template<typename BinOp> int64_t apspHopsT_Par(Graph& g, BinOp accum, int32_t nthreads) {
	if(nthreads<=1) {
		return apspHopsT(g, accum);
	}
	
	ParInfo* info = new ParInfo[nthreads];
	
	size_t nnodes = g.nodeCount();
	size_t tnodes = (nnodes+nthreads-1) / nthreads;
	size_t nstart = 0;
	for(int32_t tnum = 0; tnum < nthreads; tnum++) {
		info[tnum].g = &g;
		info[tnum].accum = accum;
		info[tnum].nstart = nstart;
		nstart += tnodes;
		if(nstart>nnodes)
			nstart = nnodes;
		info[tnum].nend = nstart-1;
	}
	for(int32_t tnum = 0; tnum < nthreads; tnum++) {
		int32_t err = pthread_create(&info[tnum].threadId, NULL, apspHopsT_Func, &info[tnum]);
		if(err)
			printf("*error(%d): pthread_create, %d\n", __LINE__, err);
	}
	
	for(int32_t tnum = 0; tnum < nthreads; tnum++) {
		pthread_join(info[tnum].threadId, NULL);
	}
	
	int64_t len = 0;
	for(int32_t tnum = 0; tnum < nthreads; tnum++) {
		len = accum(len, info[tnum].len);
	}
	
	delete[] info;
	return len;
}

int64_t apspHopsSum_Par(Graph& g, int32_t nthreads) {
	return apspHopsT_Par(g, std::plus<int64_t>(), nthreads);
}

int64_t apspHopsMax_Par(Graph& g, int32_t nthreads) {
	return apspHopsT_Par(g, maxFunc, nthreads);
}

/*template<typename BinOp> int64_t apspCostsT_Par(Graph& g, BinOp accum, int32_t nthreads) {
	size_t nnodes = g.nodeCount();
	int64_t *dist = new int64_t[nnodes];
	bool *vis = new bool[nnodes];
	
	int64_t len = 0;
	for(size_t n=0; n<nnodes; n++) {
		if(!ignoreWeightedNodes || !g.nodes[n].cost)
			len = accum(len, ssspCostsT(g, n, dist, vis, accum));
	}
	
	delete[] dist;
	delete[] vis;
	return len;
}

int64_t apspCostsSum_Par(Graph& g, int32_t nthreads) {
	return apspCostsT_Par(g, std::plus<int64_t>(), nthreads);
}

int64_t apspCostsMax_Par(Graph& g, int32_t nthreads) {
	return apspCostsT_Par(g, maxFunc, nthreads);
}*/

#endif
