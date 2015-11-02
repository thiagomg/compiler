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
    void run(generator::FuncExprPtr func);
    
    using VarsType = std::unordered_map<std::string, std::string>;

protected:
    void _run(VarsType &vars, generator::FuncExprPtr func);

    
};


#endif //COMPILER_RUNNER_H
