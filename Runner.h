//
// Created by Thiago on 10/1/15.
//

#ifndef COMPILER_RUNNER_H
#define COMPILER_RUNNER_H

//TODO: Move to Expressions.h
#include "AstGenerator.h"

#include <unordered_map>

class Runner {
public:
    //using VarsType = std::unordered_map<std::string, std::string>;
    using VarType = std::pair<std::string, std::string>;
    using VarsType = std::vector<VarType>;

    void run(generator::FuncExprPtr func);
    void _run(VarsType &vars, generator::FuncExprPtr func);
    

protected:

};


#endif //COMPILER_RUNNER_H
