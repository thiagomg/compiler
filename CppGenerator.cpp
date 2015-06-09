/* 
 * File:   AsmGenerator.cpp
 * Author: thiago
 * 
 * Created on 6 de Maio de 2015, 23:38
 */

#include "CppGenerator.h"

#include <string>
#include <fstream>
#include "asm_exception.h"
#include <algorithm>

//TODO: Remover
#include <iostream>

using namespace std;
using namespace generator;



CppGenerator::CppGenerator()
{
    string _modelName = "main_ex.cpp";

    ifstream is;

    is.open(_modelName);
    if( !is.is_open() )
        throw asm_exception("Error opening " + _modelName);


    string str;
    is.seekg(0, std::ios::end);
    str.reserve(is.tellg());
    is.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(is)),
                std::istreambuf_iterator<char>());

    string::size_type pos = str.find("////--------------------////");
    if( pos == string::npos )
        throw asm_exception("Error parsing " + _modelName);

    _before = str.substr(0, pos);
    _after = str.substr(pos+29);

}

std::string getVarName(const std::string &varName) {
    if( varName[0] == '\"' || varName[0] == '\'' ) {
        return varName;
    }
    return "_iv_" + varName;
}

std::string getLine(const int line) {
    return "Line: " + to_string(line) + ": ";
}

void CppGenerator::_validateVar(int line, const std::string &varName)
{
    if( varName.empty() )
        throw asm_exception(line, "nome de variavel nao pode ser vazio");

    if( varName[0] == '\"' || varName[0] == '\'' ) {
        return;
    }
    if( _varNames.find(varName) == _varNames.end() ) {
        throw asm_exception(line, "Variavel nao encontrada - " + varName);
    }
}

bool CppGenerator::_addVar(const std::string &varName)
{
    if( varName.empty() ) return false;
    if( varName[0] == '\"' || varName[0] == '\'' ) {
        return true;
    }

    if( _varNames.find(varName) == _varNames.end() ) {
        _varNames.insert(varName);
    }

    return true;
}

void CppGenerator::addCmd(int line, const string &cmd, std::vector< std::string > &params)
{
    try {

        if( _waitingStart ) {
            if( cmd == "[" ) {
                _waitingStart = false;
                _blockStarted = true;
                _blockContent = new std::vector<BlockCmd>();
            } else {
                throw asm_exception(line, "Inicio de bloco nao encontrado - [");
            }
            return;
        } else if( _blockStarted ) {
            if( cmd == "]" ) {
                if( _blockContent->empty() ) {
                    throw asm_exception(line, "comando se com bloco vazio encontrado");
                }
                std::vector<BlockCmd> *block = _blockContent;
                int bl = _blockLine;

                _blockStarted = false;
                _waitingStart = false;
                _blockContent = nullptr;
                _blockLine = 0;

                _processBlock(bl, _block, block);
            } else {
                _blockContent->push_back( BlockCmd(line, cmd, params) );
            }
            return;
        }

        if (cmd == "escreva") {

            //Ex: escreva "ola tudo bem" nome
            _code << "std::cout << ";
            for_each(begin(params), end(params), [this, line](const string &s) {
                std::string varName = getVarName(s);
                _validateVar(line, varName);
                _code << varName << " << ";
            });
            _code << "std::endl;" << endl;

        } else if (cmd == "pergunta") {

            //Ele precisa ter 3 parametros
            //Ex: pergunta 'qual o seu nome' em resposta
            if (params.size() != 3 || params[1] != "em")
                throw asm_exception(line, R"(pergunta precisa ter 3 parametros. Ex: pergunta 'qual o seu nome' em resposta)");

            _validateVar(line, params[0]); //pode ser uma string ou uma variavel

            std::string varName = getVarName(params[2]);
            _addVar(varName);

            _code << "std::string " << varName << ";" << endl;
            _code << "std::cout << " << params[0] << ";" << endl;
            _code << "std::cin >> " << varName << ";" << endl;

        } else if (cmd == "se") {

            //Ele precisa ter 3 parametros
            //Ex: se resposta for "papai"
            if (params.size() != 3 || params[1] != "for")
                throw asm_exception(line, R"(resposta precisa ter 3 parametros. Ex: se resposta for "papai")");

            std::string varPerg = getVarName(params[0]);
            std::string varResp = getVarName(params[2]);

            _validateVar(line, varPerg); //pode ser uma string ou uma variavel
            _validateVar(line, varResp); //pode ser uma string ou uma variavel

            _blockLine = line;
            _block = cmd;
            _blockParams.push_back(varPerg);
            _blockParams.push_back(varResp);
            _waitingStart = true;

//        } else if (cmd == "[") {
//            _code << "{" << endl;
//        } else if (cmd == "]") {
//            _code << "}" << endl;
        } else {

            _code << "// " << cmd << ": ";
            for_each(begin(params), end(params), [this](const string &s) {
                _code << "[" << s << "]";
            });
            _code << endl;

        }
    } catch( asm_exception &e ) {
        stringstream ss;
        ss << endl << cmd << ": ";
        for_each(begin(params), end(params), [&ss](const string &s) {
            ss << "[" << s << "]";
        });
        ss << endl;
        throw asm_exception(e.line(), e.what() + ss.str() );
    }
    //_code << cmd;
}

//must delete blockContent
void CppGenerator::_processBlock(int line, const std::string cmd, std::vector<BlockCmd> *blockContent)
{
    if( blockContent == nullptr )
        return;

    //TODO: Acertar comparacao para case insens. ou numeros
    if( cmd == "se" ) {
        const string &varPerg = _blockParams[0];
        const string &varResp = _blockParams[1];
        _code << "if( icompare(" << varPerg << ", " << varResp << ") )" << endl;
    }

    //if nested virou if um depois do outro
    _code << "{ //" << line << endl;
    for(BlockCmd &b : *blockContent) {
        this->addCmd(b.line, b.cmd, b.params);
    }
    _code << "} //" << line << endl;

    delete blockContent;
}

std::string CppGenerator::finish()
{
    std::string ret;
    std::string code = _code.str();
    ret.reserve( _before.size() + _after.size() + code.size() + 64 );
    ret.append(_before).append(code).append(_after);
    return ret;
}