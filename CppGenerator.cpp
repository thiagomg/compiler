/* 
 * File:   AsmGenerator.cpp
 * Author: thiago
 * 
 * Created on 6 de Maio de 2015, 23:38
 */

#include <string>
#include <fstream>
#include <algorithm>

#include "CppGenerator.h"
#include "asm_exception.h"
#include "utils.h"

//TODO: Remover
#include <iostream>

using namespace std;
using namespace generator;

int CppGenerator::_var_count = 0;

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

bool CppGenerator::_isValue(const string &varName)
{
    if( varName[0] == '\"' || varName[0] == '\'' || Utils::is_number(varName) ) {
        return true;
    }
    return false;
}

std::string CppGenerator::getVarName(const std::string &varName)
{
    if( _isValue(varName) ) {
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

    if( _isValue(varName) ) {
        return;
    }
    if( _varNames.find(varName) == _varNames.end() ) {
        throw asm_exception(line, "Variavel nao encontrada - " + varName);
    }
    return;
}

bool CppGenerator::_addVar(const std::string &varName)
{
    if( varName.empty() ) return false;
    if( _isValue(varName) ) {
        return true;
    }

    if( _varNames.find(varName) == _varNames.end() ) {
        _varNames.insert(varName);
    }

    return true;
}

void CppGenerator::_addBlockCmd(int line, const string &cmd, std::vector<string> &params)
{
    if( _curBlock && _curBlock->waitingStart ) {
        if( cmd == "[" ) {
            _curBlock->waitingStart = false;
            _curBlock->blockStarted = true;
        } else {
            throw asm_exception(line, "Inicio de bloco nao encontrado - [");
        }
        return;
    } else if( _curBlock && _curBlock->blockStarted ) {
        if( cmd == "[" ) {
            //subblock
            _curBlock->addSubBlock();
        }
        if( cmd == "]" ) {
            if( !_curBlock->remSubBlock() ) {
                //este fechamento eh de um sub-bloco aberto
                _curBlock->blockContent.push_back( BlockCmd(line, cmd, params) );
                return;
            }
            if( _curBlock->blockContent.empty() ) {
                throw asm_exception(line, "comando se com bloco vazio encontrado");
            }

            //se nao tem _savedBlock, nao eh um else
            if( _savedBlock.get() == nullptr ) {

                if( _curBlock->cmd == "se" ) {
                    _curBlock.swap(_savedBlock); //vamos salvar. Pode ter um senao
                } else {

                    //Nao eh se. Vamos processar
                    std::unique_ptr<Block> thisBlock;
                    _curBlock.swap(thisBlock);
                    _processSingleBlock(thisBlock);
                }

            } else {

                //tem um _savedBlock
                if( _curBlock->cmd == "senao" ) {
                    //Se/senao
                    std::unique_ptr<Block> thisBlock;
                    std::unique_ptr<Block> savedBlock;
                    _curBlock.swap(thisBlock);
                    _savedBlock.swap(savedBlock);

                    _processBlock(thisBlock, savedBlock);
                } else {
                    throw asm_exception("bloco desconhecido - " + _curBlock->cmd);
                }

            }

            //Processo terminado.
            //Agora vamos esperar o proximo para verificar se eh else
        } else {
            _curBlock->blockContent.push_back( BlockCmd(line, cmd, params) );
        }
        return;
    }

}

void CppGenerator::addCmd(int line, const string &cmd, std::vector< std::string > &params)
{
    try {

        if( _curBlock ) {
            _addBlockCmd(line, cmd, params);
            return;
        }

        if( _savedBlock && cmd != "senao" ) {
            std::unique_ptr<Block> thisBlock;
            _savedBlock.swap(thisBlock);
            _processSingleBlock(thisBlock);
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
                throw asm_exception(line, R"(se precisa ter 3 parametros. Ex: se resposta for "papai")");

            std::string varPerg = getVarName(params[0]);
            std::string varResp = getVarName(params[2]);

            _validateVar(line, varPerg); //pode ser uma string ou uma variavel
            _validateVar(line, varResp); //pode ser uma string ou uma variavel

            //_curBlock.reset(new Block());
            _curBlock = make_unique<Block>();
            _curBlock->blockLine = line;
            _curBlock->cmd = cmd;
            _curBlock->blockParams.push_back(varPerg);
            _curBlock->blockParams.push_back(varResp);
            _curBlock->waitingStart = true;

        } else if (cmd == "senao") {

            //Ele precisa ter 3 parametros
            //Ex: se resposta for "papai"
            if (params.size() != 0 )
                throw asm_exception(line, R"(senao nao tem parametros!)");

            if( _savedBlock == nullptr || _savedBlock->cmd != "se") {
                throw asm_exception(line, R"(senao so pode existir depois de um bloco se!)");
            }

            //_curBlock.reset(new Block());
            _curBlock = make_unique<Block>();
            _curBlock->blockLine = line;
            _curBlock->cmd = cmd;
            _curBlock->waitingStart = true;

        } else if (cmd == "repita") {

            //Ele precisa ter 3 parametros
            //Ex: se resposta for "papai"
            if (params.size() != 2 || params[1] != "vezes")
                throw asm_exception(line, R"(repita precisa ter 2 parametros. Ex: repita 3 vezes)");

            std::string varVezes = getVarName(params[0]);

            if( !Utils::is_number(varVezes) ) {
                throw asm_exception(line, R"(parametro de repita precisa ser um numero)" + varVezes);
            }

            //_curBlock.reset(new Block());
            _curBlock = make_unique<Block>();
            _curBlock->blockLine = line;
            _curBlock->cmd = cmd;
            _curBlock->blockParams.push_back(varVezes);
            _curBlock->waitingStart = true;

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

void CppGenerator::_processSingleBlock(std::unique_ptr<Block> &block)
{

    //DEBUG:
    cout << "BLK CMD: " << block->cmd << "(";
    for(string &sb : block->blockParams) {
        cout << " " << sb;
    }
    cout << ") {" << endl;
    cout << "//(size: " << block->blockContent.size() << ")" << endl;

    for(BlockCmd &b : block->blockContent) {
        cout << "    " << b.cmd << "(";
        for(string &sb : b.params) {
            cout << " " << sb;
        }
        cout << ")" << endl;
    }
    cout << "}" << endl ;
    cout << "-------------------" << endl;

    //TODO: Acertar comparacao para case insens. ou numeros
    if( block->cmd == "se" ) {
        const string &varPerg = block->blockParams[0];
        const string &varResp = block->blockParams[1];
        _code << "if( icompare(" << varPerg << ", " << varResp << ") )" << endl;
    } else if( block->cmd == "senao" ) {
        _code << "else" << endl;
    } else if( block->cmd == "repita" ) {
        const string &vezes = block->blockParams[0];
        int c = _getNextVar();
        _code << "for( int _c_i_" << c << "=0; _c_i_" << c << "<" << vezes << "; _c_i_" << c << "++)" << endl;
    }

    _code << "{ //" << block->blockLine << " (size: " << block->blockContent.size() << ")" << endl;
    for(BlockCmd &b : block->blockContent) {
        this->addCmd(b.line, b.cmd, b.params);
    }
    _code << "} //" << block->blockLine << endl;

    cout << "==================" << endl<< endl;
}

void CppGenerator::_processBlock(std::unique_ptr<Block> &block, std::unique_ptr<Block> &savedBlock)
{
    if( block.get() == nullptr )
        return;

    if( savedBlock.get() != nullptr ) {
        _processSingleBlock(savedBlock);
    }

    _processSingleBlock(block);

}

std::string CppGenerator::finish()
{
    std::string ret;
    std::string code = _code.str();
    ret.reserve( _before.size() + _after.size() + code.size() + 64 );
    ret.append(_before).append(code).append(_after);
    return ret;
}
