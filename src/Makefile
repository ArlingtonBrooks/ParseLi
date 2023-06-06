CXX=g++
WARNINGS ?= YES
WFLAGS := 

ifeq ($(WARNINGS),YES)
	WFLAGS = -Wall
	WFLAGS += -Wcast-align
	WFLAGS += -Wcast-qual
	WFLAGS += -Wlogical-op
	WFLAGS += -Wmissing-declarations
	WFLAGS += -Wmissing-include-dirs
	WFLAGS += -Wpointer-arith
	WFLAGS += -Wredundant-decls
	WFLAGS += -Wsequence-point
	WFLAGS += -Wshadow
	WFLAGS += -Wswitch
	WFLAGS += -Wundef
	WFLAGS += -Wunreachable-code
	WFLAGS += -Wunused-but-set-parameter
	WFLAGS += -Wwrite-strings
	WFLAGS += -Wdisabled-optimization
	WFLAGS += -Wpedantic
endif

CXXFLAGS=-fPIC -c -O3 -std=c++17 -march=native
LIBFLAGS=--shared -std=c++17

Target=libparselib.so
BASENAME=libparselib
SRCS = parselib.cpp

$(Target): $(SRCS)
	mkdir -p BUILD
	$(CXX) $(WFLAGS) $(CXXFLAGS) $(SRCS) -o $(BASENAME).a
	$(CXX) $(WFLAGS) $(LIBFLAGS) -o $(BASENAME).so $(BASENAME).a
	cp $(BASENAME).so BUILD/
	cp $(BASENAME).a BUILD/

test: test.cpp
	$(CXX) $(WFLAGS) test.cpp -o test.out

clean:
	rm -rf BUILD/
	rm -f $(BASENAME).a
	rm -f $(BASENAME).so
	rm -f test.out
	

