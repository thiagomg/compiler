/* 
 * File:   TokenProcessor.cpp
 * Author: thiago
 * 
 * Created on 6 de Maio de 2015, 23:05
 */

#include "TokenProcessor.h"

#include <iostream>
#include "AsmGenerator.h"

using namespace std;

TokenProcessor::TokenProcessor(generator::AsmGenerator *generator) {
    _generator = generator;
}

//TokenProcessor::TokenProcessor(const TokenProcessor& orig) {
//}

TokenProcessor::~TokenProcessor() {
}

void TokenProcessor::_processCmd(const string &cmd, std::vector< std::string > &params) {
//    cout << "Transform: " << cmd;
//    for(auto &i : params)
//        cout << " " << i ;
//    cout << endl;

    _generator->addCmd(cmd,params);

}

template<typename T>
bool is_concat(T &vec, typename T::iterator &it) {
	if( it == vec.end() )
		return false;
	if( it+1 == vec.end() )
		return false;
	if( *(it+1) == "+" )
		return true;
}

int TokenProcessor::_getSize(const std::string &cmd) {
	auto it = _cmdSize.find(cmd);
	if( it == _cmdSize.end() )
		return 0;
	return it->second;
}

bool TokenProcessor::add(int line_num, const vector<string> &line_chunks) {

	//Regras:
	//1. Numero fixo de parametros
	//2. pode-se juntar mais parametros com +
	//3. Se tiver mais parametros, devolve mais de um comando.

	int iterCount = 0;
	auto it = line_chunks.begin();
	while( iterCount < line_chunks.size() ) {
		
		const string &cmd = *it;
		it++;
		
		vector<string> params;
		int param_count = _getSize(cmd);
		for(int i=0; i < param_count; i++) {
			params.push_back(*it);
			it++;
		}
		
		cout << line_num << ": " << cmd;
		for(auto p : params) {
			cout << " [" << p << "]";
		}
		cout << endl;
		
		iterCount += param_count + 1;
		
	}
	
    return true;
}

