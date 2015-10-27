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
using ParamVector = std::vector<std::string>;


//---------------------------------------------
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
        //this->name = params[2];
        setName(params[2]);

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
//=============================================

AstGenerator::AstGenerator(TokenProcessor &processor) : _processor(processor) {

}

void AstGenerator::generate()
{

    /*
     * Main routine. Creates the ast root and start parsing
     */

    //0, "rotina", vector<string>{"principal"}

    TokenProcessor::Range range = _processor.getRange();

    //_mainExpr = std::make_shared<FuncExpr>();
    _mainExpr.reset(new FuncExpr());
    _mainExpr->parse(0, "principal", ParamVector());

    generate(_mainExpr, range);

}

void AstGenerator::generate(FuncExprPtr parent, TokenProcessor::Range range)
{
    TokenProcessor::Iterator it = range.begin();
    while(it != range.end()) {
        it = parseCmd(parent, range, it);
        //DEBUG
        if( it != range.end() ) {
            cout << "Parsed: " << it->cmd << endl;
        }
        it++;
    }

}

TokenProcessor::Iterator AstGenerator::parseCmd(FuncExprPtr parent, TokenProcessor::Range &range, TokenProcessor::Iterator &it)
{
    using namespace Utils;

    for(; it != range.end(); it++) {

        CmdToken &token = *it;
        if(is_equal("escreva", token.cmd)) {
            if( token.params.empty() ) {
                throw asm_exception(token.line, "Escreva precisa de ao menos um parametro");
            }
            //auto call = std::make_shared<CallExpr>();
            std::shared_ptr<CallExpr> call( new CallExpr() );
            call->parse(token.line, token.cmd, token.params);
            parent->addBody(call);

            break;
        } else if( is_equal("pergunta", token.cmd) ) {
            if( token.params.size() != 3 ) {
                throw asm_exception(token.line, "Pergunta deve ter 3 parametros!");
            }
            //auto call = std::make_shared<CallExpr>();
            std::shared_ptr<CallExpr> call( new CallExpr() );
            //Pergunta xxx em var_name
            call->parse(token.line, token.cmd, { token.params[0], token.params[2] });
            parent->addBody(call);
            break;
        } else {
            if( token.params.empty() ) {
                throw asm_exception(token.line, "Funcao invalida: " + token.cmd);
            }
        }
        
        

    }

    return it;

}

void AstGenerator::finish()
{

}

std::string AstGenerator::getCode()
{
    return "";
}
