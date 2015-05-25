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

void TokenProcessor::processCmd(const string &cmd, std::vector< std::string > &params) {
//    cout << "Transform: " << cmd;
//    for(auto &i : params)
//        cout << " " << i ;
//    cout << endl;

    _generator->addCmd(cmd,params);

}


bool TokenProcessor::add(const string &token) {

    //cout << "=" << token << "=" << endl;
    if(_missing == 0) {
        if( !_cmd.empty() ) {
            processCmd(_cmd, _params);
            _params.clear();
            _cmd.clear();
        }
        if( token.empty() )
            return false;
        if( _cmd.empty() ) {
            auto it =_cmdSize.find(token);
            if( it != _cmdSize.end() ) {
                _missing = it->second;
                _cmd = token;
            } else {
                //TODO: ERRO!!!
                cerr <<"ERRO - " << token << endl;
            }
        }
    } else {
        //vamos colocar os parametros
        _params.push_back(token);
        _missing--;
    }
    
    return true;
}

