CXX=gcc
CXXFLAGS=-Wall -Werror -Wextra -fPIC -c -O3 -march=native
LIBFLAGS=--shared

Target=parselib

$(Target): $(Target).cpp
	mkdir -p BUILD
	$(CXX) $(CXXFLAGS) $(Target).cpp -o $(Target).o
	$(CXX) $(LIBFLAGS) -o lib$(Target).so $(Target).o
	mv lib$(Target).so BUILD/

clean:
	rm -rf BUILD/
	rm $(Target).o
