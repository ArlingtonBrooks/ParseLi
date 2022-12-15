#include "src/parselib.cpp"

int main(int argc, char** argv)
{
	char fname[] = "Example.in";
	Dict D;
	
	ReadConfig(fname,&D,false);
	D.Dump();
}
