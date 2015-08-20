#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <memory>

#include "QuotedWord.h"
#include "TokenProcessor.h"
#include "CppGenerator.h"

using namespace std;
using namespace generator;

void crackLine(vector<string> &line, istringstream &ss) {

	while( !( ss.eof() || ss.fail()) ) {
		QuotedWord s;
		ss >> s;
		
//		bool eof = ss.eof();
//		bool fail = ss.fail();
//		bool bad = ss.bad();
//		cout << s() << eof << fail << bad << endl;
		
		if( s().empty() )
			break;
		
		if( s()[0] == '#' )
			return;
		
		line.push_back( s() );
	}
		
}

//get lines and generate cmd p1 p2 p3...
//and pass to asm generator
void parse_file(ifstream &is, function<bool(int line_num, const vector<string> &)> f) {

	//FIXME: Nos chunks, se algum comecar com # descartar ate o final da linha
	
	int line_num = 0;
	vector<string> line_chunks;
	while( !is.eof() ) {
		
		std::string line;
		std::getline(is, line);
		
		line_num++;
		
		//cout << "L " << line_num << ": " << line << endl;
		
		istringstream ss(line);

		crackLine(line_chunks, ss);
		if( (!line_chunks.empty()) &&  line_chunks.back() == "\\" ) {
			line_chunks.pop_back();
			continue;
		}
		
		//TODO: Remover
		//cout << endl;
		//for(auto i : line_chunks) {		
		//	cout << line_num << ": " << i << endl;
		//}
		
		//agora temos a linha com os chunks.
		//Precisamos passar no token processor
		if( !f(line_num, line_chunks) ) {
            break;
        }
		
		line_chunks.clear();
		
	}
    //f("");
	cout << endl;
	
}


int main( /*int argc, char **argv */)
{
    unique_ptr<CppGenerator> generator( new CppGenerator() );
    TokenProcessor proc; //(generator.get());
    
    string fname = "teste.lgo";

    ifstream is(fname.c_str());
    try {

        if( is.good() ) {
			//parse_file quebra em linhas e manda um vector 
			//para o processador de tokens
            parse_file(is, [&proc](int line_num, const vector<string> &line_chunks) {
                //add sera chamado para cada linha do arquivo com conteudo
				return proc.add(line_num, line_chunks);
            });

            generator->finish();
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

    //cout << generator->finish() << endl;

	fstream fout;
	fout.open("teste.cpp", fstream::out );
	fout << generator->getCode();
	fout.close();

    return 0;
}
