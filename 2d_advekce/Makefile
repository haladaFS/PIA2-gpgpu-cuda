defaults: all

# Nastavení použití počítání na GPU pomocí CUDA
# use_CUDA = yes	program se přeloží pomocí nvcc
# use_CUDA = no		program se přeloží pomocí gcc (lze nastavit i jiný překladač)
use_CUDA = no
# use_CUDA = yes

# GPU architektura (CUDA capability) - závisí na grafické kartě a verzi CUDA
#GPU_ARCH = sm_61
#GPU_ARCH = sm_35
GPU_ARCH = sm_35
#pozn. sm_35 by melo fungovat snad všude, kde není prehistorická GPU a zároveň CUDA nejvýše v11

# překladač - verze g++ aspoň 5 [?]
CXX=g++

CPPFLAGS = -I.
CXXFLAGS = -O3 -fopenmp -funroll-loops -std=c++11
LDFLAGS = -lgomp -lrt -lpng

ifeq ($(use_CUDA),yes)
	NVCC=nvcc
	NVCCFLAGS=  -std=c++11 --use_fast_math -O3 -arch $(GPU_ARCH) -Wno-deprecated-gpu-targets
	# disable useless nvcc warning
	NVCCFLAGS += -Xcudafe "\"--diag_suppress=declared_but_not_referenced\""
	NVCCFLAGS += -D USE_CUDA
	CXXFLAGS +=  -D USE_CUDA
else
	NVCC=$(CXX)
	NVCCFLAGS= -std=c++11
endif

CPP_SOURCES = $(wildcard *.cpp)
CU_SOURCES = $(wildcard *.cu)
SIMS = $(wildcard main.cu)
#SIMS = $(wildcard sim?*.cu)
#TARGETS = $(SIMS:%.cu=%)
TARGETS = $(SIMS:%.cu=%)

# OBJS are *.o except sim?*.o
OBJS = $(filter-out $(SIMS:%.cu=%.o), $(CU_SOURCES:%.cu=%.o))  $(CPP_SOURCES:%.cpp=%.o)

all: $(TARGETS)

clean:
	rm -f *.o *.d $(TARGETS)

%.o: %.cpp
	$(CXX) -MD -MP $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.o: %.cu
ifeq ($(use_CUDA),yes)
	$(NVCC) -M $(NVCCFLAGS) -o $(@:%.o=%.d) $<
	$(NVCC) $(CPPFLAGS) $(NVCCFLAGS) -c -o $@ $<
else
	$(CXX) -MD -MP $(CPPFLAGS) $(CXXFLAGS) -xc++ -c -o $@ $<
endif

$(TARGETS): % : %.o $(OBJS)
	$(NVCC) $(NVCCFLAGS) -o $@ $< $(OBJS) $(LDFLAGS)

-include $(CPP_SOURCES:%.cpp=%.d)
-include $(CU_SOURCES:%.cu=%.d)
