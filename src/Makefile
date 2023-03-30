CXX=g++
CXXFLAGS=-Wall -Werror -Wextra -fPIC -c -O3 -march=native
LIBFLAGS=--shared

Target=libparselib.so
BASENAME=libparselib
SRCS = parselib.cpp

$(Target): $(SRCS)
	mkdir -p BUILD
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(BASENAME).o
	$(CXX) $(LIBFLAGS) -o $(BASENAME).so $(BASENAME).o
	cp $(BASENAME).so BUILD/

test: test.cpp
	$(CXX) test.cpp -o test.out

clean:
	rm -rf BUILD/
	rm -f $(BASENAME).o
	rm -f $(BASENAME).so
	rm -f test.out
