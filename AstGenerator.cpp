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
    }

}

template<typename T>
TokenProcessor::Iterator check_and_parse(FuncExprPtr parent, TokenProcessor::Range &range, TokenProcessor::Iterator &it, bool &found) {

    CmdToken &token = *it;
    
    Expr *expr = T::create_if(token.cmd);
    if( expr != nullptr ) {
        expr->parse(token.line, token.cmd, token.params);
        generator::Expr::ExprPtr p(expr);
        parent->addBody(p);
        ++it;
        found = true;
        return it;
    }
    found = false;
    return range.end();
}

TokenProcessor::Iterator AstGenerator::parseCmd(FuncExprPtr parent, TokenProcessor::Range &range, TokenProcessor::Iterator &it)
{
    using namespace Utils;
    bool found = false;

    TokenProcessor::Iterator itr = check_and_parse<BuiltIn>(parent, range, it, found);
    if( found ) return itr;

    itr = check_and_parse<VarExpr>(parent, range, it, found);
    if( found ) return itr;

    itr = check_and_parse<CompExpr>(parent, range, it, found);
    if( found ) return itr;
    
    throw asm_exception(it->line, "Funcao invalida: " + it->cmd);

//    for(; it != range.end(); it++) {
//
//        
//        Expr *expr = BuiltIn::create_if(token.cmd);
//        if( expr != nullptr ) {
//            expr->parse(token.line, token.cmd, token.params);
//        }
//        
//        if(is_equal("escreva", token.cmd)) {
//            if( token.params.empty() ) {
//                throw asm_exception(token.line, "Escreva precisa de ao menos um parametro");
//            }
//            //auto call = std::make_shared<CallExpr>();
//            std::shared_ptr<CallExpr> call( new CallExpr() );
//            call->parse(token.line, token.cmd, token.params);
//            parent->addBody(call);
//
//            break;
//        } else if( is_equal("pergunta", token.cmd) ) {
//            if( token.params.size() != 3 ) {
//                throw asm_exception(token.line, "Pergunta deve ter 3 parametros!");
//            }
//            //auto call = std::make_shared<CallExpr>();
//            std::shared_ptr<CallExpr> call( new CallExpr() );
//            //Pergunta xxx em var_name
//            call->parse(token.line, token.cmd, { token.params[0], token.params[2] });
//            parent->addBody(call);
//            break;
//        } else if( is_equal("defina", token.cmd) ) {
//            std::shared_ptr<VarExpr> var( new VarExpr() );
//            var->parse(token.line, token.cmd, token.params);
//            parent->addBody(var);
//            break;
//        } else if( is_equal("se", token.cmd) ) {
//            std::shared_ptr<CompExpr> var( new CompExpr() );
//            var->parse(token.line, token.cmd, token.params);
//            parent->addBody(var);
//            break;
//        } else {
//            if( token.params.empty() ) {
//                throw asm_exception(token.line, "Funcao invalida: " + token.cmd);
//            }
//        }
//
//    }
//
//    return it;

}

void AstGenerator::finish()
{

}

std::string AstGenerator::getCode()
{
    return "";
}
