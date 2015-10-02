//
// Created by Thiago on 8/19/15.
//

#ifndef COMPILER_ASTGENERATOR_H
#define COMPILER_ASTGENERATOR_H

#include <string>
#include <vector>
#include <memory>
#include "TokenProcessor.h"

class TokenProcessor;
class CmdToken;

namespace generator {

    struct Expr {
        std::string name;

        using ExprPtr = std::shared_ptr<Expr>;
        virtual void parse(int line, const std::string &name, const std::vector<std::string> &params) = 0;
        virtual ~Expr() = 0;
    };

    class AstGenerator {
    public:

        AstGenerator(TokenProcessor &processor);
        void generate();
        void generate(TokenProcessor::Range range);

        Expr::ExprPtr parseCmd(TokenProcessor::Range &range, TokenProcessor::Iterator &it);

        //void addCmd(int line, const std::string &cmd, const std::vector<std::string> &params);

        void finish();

        std::string getCode();

        Expr::ExprPtr _mainExpr;
        TokenProcessor &_processor;

    };



}
#endif //COMPILER_ASTGENERATOR_H
