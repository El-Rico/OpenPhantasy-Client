#include <iostream>
#include <GitVersion.hpp>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Open|Phantasy [Client]" << std::endl;
	std::cout << "Revision: " << GIT_COMMITHASH << " [ " <<
		GIT_COMMITTERDATE << " ]" << std::endl;
	return 0;
}

