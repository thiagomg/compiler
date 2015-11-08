/* 
 * File:   TokenProcessor.cpp
 * Author: thiago
 * 
 * Created on 6 de Maio de 2015, 23:05
 */

#include "TokenProcessor.h"

#include <iostream>

using namespace std;


TokenProcessor::TokenProcessor() {

}

TokenProcessor::~TokenProcessor() {
}

void TokenProcessor::_processCmd(int line_num, const string &cmd, std::vector< std::string > &params) {
//    cout << "Transform: " << cmd;
//    for(auto &i : params)
//        cout << " " << i ;
//    cout << endl;

    _tokens.push_back(CmdToken(line_num, cmd, params));

}

template<typename T, typename U> //typename T::iterator
bool is_concat(T &vec, U it) {
	if( it == vec.end() )
		return false;
	if( *(it) == "+" )
		return true;

    return false;
}

int TokenProcessor::_getSize(const std::string &cmd) {
	auto it = _cmdSize.find(cmd);
	if( it == _cmdSize.end() )
		return 0;
	return it->second;
}

std::vector<CmdToken> &TokenProcessor::tokenList()
{
    return _tokens;
}

TokenProcessor::Range TokenProcessor::getRange()
{
    return TokenProcessor::Range(_tokens.begin(), _tokens.end());
}


bool TokenProcessor::add(int line_num, const vector<string> &line_chunks) {

	//Regras:
	//1. Numero fixo de parametros
	//2. pode-se juntar mais parametros com +
	//3. Se tiver mais parametros, devolve mais de um comando.

    size_t iterCount = 0;
	auto it = line_chunks.begin();
	while( iterCount < line_chunks.size() ) {
		
		const string &cmd = *it;
		it++;
		
		vector<string> params;
		int param_count = _getSize(cmd);
		for(int i=0; i < param_count; i++) {
			const string &p = *it;
            
            params.push_back(p);

            it++;
			
			if( is_concat(line_chunks, it) ) {
				param_count+=2;// + next 
				it++;
				i++; //pular +
				continue;
			}
		}
		
//		cout << line_num << ":	 " << cmd;
//        if( !params.empty() ) cout << " (";
//		for(auto p : params) {
//			cout << " " << p;
//		}
//        if( !params.empty() ) cout << " )";
//		cout << endl;

		_processCmd(line_num, cmd, params);
		
		iterCount += param_count + 1;
		
	}
	
    return true;
}

