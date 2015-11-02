//
// Created by Thiago on 8/19/15.
//
#pragma once

#ifndef COMPILER_VARUTILS_H
#define COMPILER_VARUTILS_H

#include <string>
#include <set>

class VarUtils {
public:

    void validateVar(int line, const std::string &varName);
    bool addVar(const std::string &varName);
    std::string getVarName(const std::string &varName);


private:
    //variable name list
    std::set<std::string> _varNames;

};


#endif //COMPILER_VARUTILS_H
