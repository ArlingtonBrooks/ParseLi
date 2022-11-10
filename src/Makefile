CXX=gcc
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

clean:
	rm -rf BUILD/
	rm -f $(BASENAME).o
	rm -f $(BASENAME).so
