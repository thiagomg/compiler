#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <functional>

#include "QuotedWord.h"
#include "TokenProcessor.h"

using namespace std;

void parse_file(ifstream &is, function<bool(const string &)> f) {
	
	while( !is.eof() ) {
		QuotedWord s;
		is >> s;
		if( !f(s()) ) {
            break;
        }
	}
    f("");
	cout << endl;
	
}

int main(int argc, char **argv)
{
    TokenProcessor proc;
    
	string fname = "./teste.lgo";
    
	ifstream is(fname.c_str());
	if( is.good() ) {
		parse_file(is, [&proc](const string &s) {
            return proc.add(s);
        });
	} else
		cout << "Error opening file: " << fname << endl;
	
	//string s;
	//cin >> s;
	cout << "Fim" << endl;
	//getchar();
	
	return 0;
}