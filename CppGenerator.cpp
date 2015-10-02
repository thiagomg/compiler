///*
// * File:   AsmGenerator.cpp
// * Author: thiago
// *
// * Created on 6 de Maio de 2015, 23:38
// */
//
//#include <string>
//#include <fstream>
//#include <algorithm>
//
//#include "CppGenerator.h"
//#include "asm_exception.h"
//#include "utils.h"
//
////TODO: Remover
//#include <iostream>
//
//using namespace std;
//using namespace generator;
//
//CppGenerator::CppGenerator()
//{
//    string _modelName = "main_ex.cpp";
//
//    ifstream is;
//
//    is.open(_modelName);
//    if( !is.is_open() )
//        throw asm_exception("Error opening " + _modelName);
//
//
//    string str;
//    is.seekg(0, std::ios::end);
//    str.reserve(is.tellg());
//    is.seekg(0, std::ios::beg);
//
//    str.assign((std::istreambuf_iterator<char>(is)),
//                std::istreambuf_iterator<char>());
//
//    string::size_type pos = str.find("////--------------------////");
//    if( pos == string::npos )
//        throw asm_exception("Error parsing " + _modelName);
//
//    _before = str.substr(0, pos);
//    _after = str.substr(pos+29);
//
//}
//
//std::string getLine(const int line) {
//    return "Line: " + to_string(line) + ": ";
//}
//
//void CppGenerator::addCmd(int line, const string &cmd, std::vector< std::string > &params)
//{
//    _tokenList.push_back(CppCmdToken(line, cmd, params));
//}
//
//CppCmdToken CppGenerator::_getNext()
//{
//    CppCmdToken cmd_ref = _tokenList.front();
//    _tokenList.pop_front();
//    return std::move(cmd_ref);
//}
//
//void CppGenerator::_putBack(CppCmdToken &&CppCmdToken)
//{
//    _tokenList.push_front(std::move(CppCmdToken));
//}
//
//bool CppGenerator::_hasToken()
//{
//    return !_tokenList.empty();
//}
//
//void CppGenerator::_genEscreva(CppCmdToken &CppCmdToken)
//{
//
//}
//
//void CppGenerator::finish() {
//
//    while( _hasToken() ) {
//
//        CppCmdToken CppCmdToken = _getNext();
//
//        //Aqui, ao inves de construir um codigo cpp, melhor gerar uma Abstract Syntax Tree
//        //cmd p1 p2 ... pn
//        //Sendo P? -> Value or Cmd (que sempre deve retornar um valor)
//
//        cout << getLine(CppCmdToken.line) << CppCmdToken.cmd << " ( ";
//        for(auto &param : CppCmdToken.params) {
//            cout << param << " ";
//        }
//        cout << ") " << endl;
//    }
//
//}
//
//std::string CppGenerator::getCode()
//{
////    std::string ret;
////    std::string code = _code.str();
////    ret.reserve( _before.size() + _after.size() + code.size() + 64 );
////    ret.append(_before).append(code).append(_after);
////    return ret;
//    return "[[code]]";
//}
