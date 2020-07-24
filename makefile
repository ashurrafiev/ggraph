CPP = ggraph.cpp
HDR = random.h graph.h gen.h gennormal.h graphalg.h
.PHONY: default all clean

default: ggraph
all: default

ggraph: $(CPP) $(HDR)
	g++ ggraph.cpp -o ggraph
	
clean:
	rm ggraph
