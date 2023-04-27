#include "parselib.cpp"
#include <sstream>

int GetScheme(std::string Scheme)
{
	if (Scheme.compare("HLLE") == 0)
		return 1;
	else if (Scheme.compare("ROE") == 0)
		return 2;
	else
		return -1;
}

int main(int argc, char** argv)
{
	char fname[] = "Example.in";
	ParseLi::Dict D;
	
	ParseLi::ReadConfig(fname,&D,false);
	D.Dump();
	
	//Example of read operation (returns defaults if values are not found)
	//Set CFL number to value provided; otherwise default to 0.5
	double CFL_NUMBER = (D.CheckDouble("CFL_NUMBER")) ? D.GetDouble("CFL_NUMBER") : 0.5;
	int SCHEME = (D.CheckString("SCHEME")) ? GetScheme(D.GetString("SCHEME")) : 1;
	//Add default scheme to dictionary if not already present
	if (!D.CheckString("SCHEME")) D.add("SCHEME","HLLE");
	
	printf("CFL_NUMBER is set to %lf\n",CFL_NUMBER);
	printf("SCHEME is set to %d, where 1=HLLE and 2=ROE\n",SCHEME);
	
	//Streamed input
	std::stringstream test2;
	test2 << "String 1" << std::endl;
	test2 << "test teststring" << std::endl;
	test2 << "float 1.05" << std::endl;
	test2 << "WARNING This is a warning" << std::endl;
	ParseLi::Dict D2;
	ParseLi::ReadConfig(test2,&D2,false);
	D2.Dump();
	D2 = D;
	D2.Dump();
	
	//Another file input
	ParseLi::Dict D3;
	std::ifstream I1("Example.in");
	ParseLi::ReadConfig(I1,&D3,false);
	D3.Dump();
}
