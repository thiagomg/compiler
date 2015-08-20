//
// Created by Thiago on 8/19/15.
//

#ifndef COMPILER_ASTGENERATOR_H
#define COMPILER_ASTGENERATOR_H

#include <string>
#include <vector>
#include <memory>

class AstGenerator {



};

struct CmdToken {
    CmdToken(int line, const std::string &cmd, std::vector< std::string > &params) {
        this->line = line;
        this->cmd = cmd;
        this->params = params;
    }
//        CmdToken(CmdToken &&src) {
//            this->line = src.line;
//            this->cmd = src.cmd;
//            this->params = src.params;
//        }
    CmdToken(const CmdToken &src) {
        this->line = src.line;
        this->cmd = src.cmd;
        this->params = src.params;
    }
    int line;
    std::string cmd;
    std::vector<std::string> params;
};

struct Expr {
    std::string name;

    using ExprPtr = std::shared_ptr<Expr>;

    virtual void parse(int line, const std::string &name, const std::vector<std::string> &params) = 0;
    virtual ~Expr() = 0;
};

#endif //COMPILER_ASTGENERATOR_H
