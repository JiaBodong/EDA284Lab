IDIR=include
CXX=aarch64-linux-gnu-g++
#CXX=g++
CXXFLAGS=-I$(IDIR) -std=c++11 -fopenmp -O2 -Wall

ODIR=src
LDIR =../lib

LIBS=-lm -fopenmp -static


%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< ${CXXFLAGS}

%.o: %.cxx $(DEPS)
	$(CXX) -c -o $@ $< ${CXXFLAGS}

gemm_threaded_tiled.o:
	$(CXX) -c -o $@ gemm.cxx ${CXXFLAGS} -DBLOCK_TILE -DENABLE_THREADING

gemm_threaded.o:
	$(CXX) -c -o $@ gemm.cxx ${CXXFLAGS} -DENABLE_THREADING

gemm_tiled.o:
	$(CXX) -c -o $@ gemm.cxx ${CXXFLAGS} -DBLOCK_TILE

gemm.o:
	$(CXX) -c -o $@ gemm.cxx ${CXXFLAGS} 

gemm_threaded_tiled_single.o:
	$(CXX) -c -o $@ gemm.cxx ${CXXFLAGS} -DBLOCK_TILE -DENABLE_THREADING -DSINGLE

gemm_threaded_single.o:
	$(CXX) -c -o $@ gemm.cxx ${CXXFLAGS} -DENABLE_THREADING -DSINGLE

gemm_tiled_single.o:
	$(CXX) -c -o $@ gemm.cxx ${CXXFLAGS} -DBLOCK_TILE -DSINGLE

gemm_single.o:
	$(CXX) -c -o $@ gemm.cxx ${CXXFLAGS} -DSINGLE

gemm_threaded: gemm_threaded.o
	$(CXX) -o $@ $^ $(LIBS)

gemm_tiled: gemm_tiled.o
	$(CXX) -o $@ $^ $(LIBS)

gemm_threaded_tiled: gemm_threaded_tiled.o
	$(CXX) -o $@ $^ $(LIBS)

gemm: gemm.o
	$(CXX) -o $@ $^ $(LIBS)

gemm_threaded_single: gemm_threaded_single.o
	$(CXX) -o $@ $^ $(LIBS)

gemm_tiled_single: gemm_tiled_single.o
	$(CXX) -o $@ $^ $(LIBS)

gemm_threaded_tiled_single: gemm_threaded_tiled_single.o
	$(CXX) -o $@ $^ $(LIBS)

gemm_single: gemm_single.o
	$(CXX) -o $@ $^ $(LIBS)


#all: clean gemm_threaded_tiled_double gemm_threaded_double gemm_tiled_double gemm_double gemm_threaded_tiled_single gemm_threaded_single gemm_tiled_single gemm_single
all: clean gemm_threaded_tiled gemm_threaded gemm_tiled gemm 

.PHONY: clean

default: clean all

.DEFAULT_GOAL := default

clean:
	rm -f *.o gemm_* gemm 
