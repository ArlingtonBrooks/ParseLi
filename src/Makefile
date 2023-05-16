CXX=g++
CXXFLAGS=-Wall -Werror -Wextra -fPIC -c -O3 -std=c++17 -march=native
LIBFLAGS=--shared

Target=libparselib.so
BASENAME=libparselib
SRCS = parselib.cpp

$(Target): $(SRCS)
	mkdir -p BUILD
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(BASENAME).a
	$(CXX) $(LIBFLAGS) -o $(BASENAME).so $(BASENAME).a
	cp $(BASENAME).so BUILD/
	cp $(BASENAME).a BUILD/

test: test.cpp
	$(CXX) test.cpp -o test.out

clean:
	rm -rf BUILD/
	rm -f $(BASENAME).a
	rm -f $(BASENAME).so
	rm -f test.out
