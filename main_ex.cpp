#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <functional>

bool icompare_pred(unsigned char a, unsigned char b)
{
	return std::tolower(a) == std::tolower(b);
}

bool icompare(std::string const& a, std::string const& b)
{
	if( a.size() != b.size() ) return false;
	int ms = a.size();

	for(int i=0; i < ms; i++) {
		if( a.at(i) != b.at(i) ) return false;
	}
	return true;

}

int main(int argc, char **argv)
{

    ////--------------------////


	return 0;
}
