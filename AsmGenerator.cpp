/* 
 * File:   AsmGenerator.cpp
 * Author: thiago
 * 
 * Created on 6 de Maio de 2015, 23:38
 */

#include "AsmGenerator.h"

#include <string>
#include <fstream>
#include <exception>
#include <algorithm>

using namespace std;
using namespace generator;

struct asm_exception : std::exception
{
    char text[1000];

    asm_exception(const std::string &s)
    {
        std::copy(begin(text), end(text), begin(text));
    }
/*
    asm_exception(char const* fmt, ...) __attribute__((format(printf,2,3))) {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(text, sizeof text, fmt, ap);
        va_end(ap);
    }
*/
    char const* what() const throw() { return text; }
};

AsmGenerator::AsmGenerator()
{
    string _modelName = "../compiler/main_ex.cpp";

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

void AsmGenerator::addCmd(const string &cmd, std::vector< std::string > &params)
{
    if( cmd == "escreva" ) {
        _code << "std::cout << ";
        for_each(begin(params), end(params), [this](const string &s) {
            _code << s << " << ";
        });
        _code << "std::endl;" << endl;
    }

    //_code << cmd;
}

std::string AsmGenerator::finish()
{
    std::string ret;
    std::string code = _code.str();
    ret.reserve( _before.size() + _after.size() + code.size() + 64 );
    ret.append(_before).append(code).append(_after);
    return ret;
}
