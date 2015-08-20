//
// Created by Thiago on 8/19/15.
//

#include "VarUtils.h"
#include "utils.h"
#include "asm_exception.h"

std::string VarUtils::getVarName(const std::string &varName)
{
    if( Utils::is_value(varName) ) {
        return varName;
    }
    return "_iv_" + varName;
}


void VarUtils::validateVar(int line, const std::string &varName)
{
    if( varName.empty() )
        throw asm_exception(line, "nome de variavel nao pode ser vazio");

    if( Utils::is_value(varName) ) {
        return;
    }
    if( _varNames.find(varName) == _varNames.end() ) {
        throw asm_exception(line, "Variavel nao encontrada - " + varName);
    }
    return;
}

bool VarUtils::addVar(const std::string &varName)
{
    if( varName.empty() ) return false;
    if( Utils::is_value(varName) ) {
        return true;
    }

    if( _varNames.find(varName) == _varNames.end() ) {
        _varNames.insert(varName);
    }

    return true;
}
