#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <memory>

#include "QuotedWord.h"
#include "TokenProcessor.h"
#include "AsmGenerator.h"

using namespace std;
using namespace generator;

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
    unique_ptr<AsmGenerator> generator( new AsmGenerator() );
    TokenProcessor proc(generator.get());
    
    string fname = "teste.lgo";

    ifstream is(fname.c_str());
    try {

        if( is.good() ) {
            parse_file(is, [&proc](const string &s) {
                return proc.add(s);
            });
        } else
            cerr << "Erro abrindo arquivo: " << fname << endl;
	
    } catch(std::exception &e) {
        cerr << "Erro compilando: " << e.what() << endl;
    }

	//string s;
	//cin >> s;
    //cout << "Fim" << endl;
	//getchar();

    if(is.is_open())
        is.close();

    cout << generator->finish() << endl;

    return 0;
}
