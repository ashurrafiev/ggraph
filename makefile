CPP = ggraph.cpp
HDR = profile.h random.h graph.h graphalg.h graphalg_p.h gen.h flimit.h
.PHONY: default all clean

default: ggraph
all: default

ggraph: $(CPP) $(HDR)
	g++ ggraph.cpp -o ggraph -lpthread
	
clean:
	rm ggraph
