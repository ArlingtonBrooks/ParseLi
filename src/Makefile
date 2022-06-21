CXX=gcc
CXXFLAGS=-Wall -Werror -Wextra -fPIC -c -O3 -march=native
LIBFLAGS=--shared

Target=parselib

$(Target): $(Target).cpp
	mkdir BUILD
	$(CXX) $(CXXFLAGS) $(Target).cpp -o $(Target).o
	$(CXX) $(LIBFLAGS) -o lib$(Target).so $(Target).o

clean:
	rm -rf BUILD/
	rm $(Target).o
