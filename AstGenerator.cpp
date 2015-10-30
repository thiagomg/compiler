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
TokenProcessor::Iterator AstGenerator::check_and_parse(FuncExprPtr parent, TokenProcessor::Range &range, TokenProcessor::Iterator &it, bool &found) {

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

template<>
TokenProcessor::Iterator AstGenerator::check_and_parse<CompExpr>(FuncExprPtr parent, TokenProcessor::Range &range, TokenProcessor::Iterator &ito, bool &found) {
    
    CmdToken &token = *ito;
    
    CompExpr *expr = CompExpr::create_if(token.cmd);
    if( expr != nullptr ) {
        expr->parse(token.line, token.cmd, token.params);
        generator::Expr::ExprPtr p(expr);
        parent->addBody(p);
        
        ++ito;

        //Vamos agora procurar o começo e final
        TokenProcessor::Iterator itf = ito;
        
        //O primeiro precisa ser um [
        if( ito->cmd != "[" )
            throw asm_exception("Apos um comando se, eh obrigatorio haver [");
        
        int brackets = 1;
        
        //vamos agora procurar onde fecha o parentese
        for(; itf != range.end(); ++itf) {
            CmdToken &c = *itf;
            if( c.cmd == "[" )
                brackets++;
            else if( c.cmd == "]" )
                brackets--;
            if( brackets == 0 )
                break;
        }
        
        if( brackets == 1 ) {
            throw asm_exception("] final nao encontrado");
        }
        
        auto its = ito+1;
        auto ite = itf-1;
        for(auto it=its; it != ite;) {
            expr->true_body.reset( new FuncExpr() );
            it = parseCmd(expr->true_body, range, it);
        }

        found = true;
        ++itf; //after last ]
        return itf;
    }
    found = false;
    return range.end();
}


TokenProcessor::Iterator AstGenerator::parseCmd(FuncExprPtr parent, TokenProcessor::Range &range, TokenProcessor::Iterator &it)
{
    using namespace Utils;
    bool found = false;

    TokenProcessor::Iterator itr = check_and_parse<BuiltInExpr>(parent, range, it, found);
    if( found ) return itr;

    itr = check_and_parse<VarExpr>(parent, range, it, found);
    if( found ) return itr;

    itr = check_and_parse<CompExpr>(parent, range, it, found);
    if( found ) return itr;

    itr = check_and_parse<CompExpr>(parent, range, it, found);
    if( found ) return itr;
    
    throw asm_exception(it->line, "Funcao invalida: " + it->cmd);
}

void AstGenerator::finish()
{

}

std::string AstGenerator::getCode()
{
    return "";
}
