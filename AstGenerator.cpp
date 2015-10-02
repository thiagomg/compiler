//
// Created by Thiago on 8/19/15.
//

#include "AstGenerator.h"
#include "VarUtils.h"
#include "utils.h"
#include "asm_exception.h"
#include "TokenProcessor.h"

#include <iostream>

using namespace generator;
using namespace std;

VarUtils varUtils;

AstGenerator::AstGenerator(TokenProcessor &processor) : _processor(processor) {

}

void AstGenerator::generate()
{
    //0, "rotina", vector<string>{"principal"}

//    auto tokens = _processor.tokenList();
//
//    for(auto it = tokens.begin(); it != tokens.end(); it++) {
//
//        CmdToken &token = *it;
//        if(Utils::is_equal(token.cmd, "se")) {
//            CompExpr
//        }
//
//    }

//    if( _mainExpr.get() == nullptr ) {
//        auto expr = new FuncExpr();
//        expr->name = "main";
//    }
//
//    TokenProcessor::Range tokenRange = _processor.getRange();
//    TokenProcessor::Iterator it = tokenRange.first;
//    while()
//        Expr::ExprPtr expr = parseCmd();
//        //Colocar na arvore.
//    }


}

void AstGenerator::generate(TokenProcessor::Range range)
{
    TokenProcessor::Iterator it = range.begin();
    while(it != range.end()) {
        Expr::ExprPtr expr = parseCmd(range, it);
        //Colocar na arvore.
    }
}

Expr::ExprPtr AstGenerator::parseCmd(TokenProcessor::Range &range, TokenProcessor::Iterator &it)
{
    CmdToken &token = *it;

//    if(Utils::is_equal("se", token.cmd)) {
//        CompExpr *expr = new CompExpr();
//
//    }

    return Expr::ExprPtr();

}

//void addCmd(int line, const std::string &cmd, const std::vector<std::string> &params)
//{
//    if( Utils::is_equal(cmd, "rotina") ) {
//        FuncExpr *f = new FuncExpr();
//        f->parse(line, cmd, params);
//
//        Expr::ExprPtr expr( f );
//        if( mainExpr == nullptr ) {
//            mainExpr = expr;
//        }
//        return;
//    }
//
//    if( Utils::is_equal(cmd, "if") ) {
//        return;
//    }
//}

void AstGenerator::finish()
{

}

std::string AstGenerator::getCode()
{
    return "";
}

struct FuncExpr : public Expr {

    std::vector<std::string> params;
    std::vector<ExprPtr> body;

    virtual void parse(int line, const std::string &name, const std::vector<std::string> &params) override {
        this->name = name;
        this->params = params;
    }

    void addBody(ExprPtr expr) {
        body.push_back(expr);
    }

    virtual ~FuncExpr() {}
};

struct VarExpr : public Expr {

    std::string value;

    virtual void parse(int line, const std::string &name, const std::vector<std::string> &params) override {
        if( !Utils::is_equal(name, "defina") ) {
            throw asm_exception(line, "defina nao esta com o nome defina");
        }

        //defina 2 em valor
        if( params.size() != 3 )
            throw asm_exception(line, "numero de parametros incorretos em defina: ", params);

        this->value = params[0];
        this->name = params[2];

    }
    virtual ~VarExpr() {}
};

struct CompExpr : public Expr {

    std::vector<ExprPtr> true_body;
    std::vector<ExprPtr> false_body;
    std::vector<std::string> params;

    virtual void parse(int line, const std::string &name, const std::vector<std::string> &params) override {
        //este eh um SE xxx for yyy
        if( !Utils::is_equal(name, "se") ) {
            throw asm_exception(line, "defina nao esta com o nome defina");
        }

        //defina 2 em valor
        if( params.size() != 3 )
            throw asm_exception(line, "numero de parametros incorretos em se: ", params);

        this->params.push_back(params[0]);
        this->params.push_back(params[2]);

        varUtils.validateVar(line, params[0]);
        varUtils.validateVar(line, params[1]);
    }
    virtual ~CompExpr() {};
};