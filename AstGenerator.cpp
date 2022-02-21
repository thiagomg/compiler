//
// Created by Thiago on 8/19/15.
//

#include "AstGenerator.h"
#include "VarUtils.h"
#include "utils.h"
#include "asm_exception.h"
#include "TokenProcessor.h"

#include <iostream>
#include <algorithm>

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
//        if( it != range.end() ) {
//            cout << "Parsed: " << it->cmd << endl;
//        }
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

TokenProcessor::Iterator AstGenerator::count_brackets(TokenProcessor::Iterator &first, TokenProcessor::Range &range) {
    
    TokenProcessor::Iterator last = first;

    int brackets = 1;
    ++last; //We've passed the first opened bracket already.
    
    //vamos agora procurar onde fecha o parentese
    for(; last != range.end(); ++last) {
        CmdToken &c = *last;
        if( c.cmd == "[" )
            brackets++;
        else if( c.cmd == "]" )
            brackets--;
        if( brackets == 0 )
            break;
    }
    
    return last;
}

FuncExprPtr AstGenerator::add_cmds(CompExpr *expr, TokenProcessor::Range &range, TokenProcessor::Iterator &first, TokenProcessor::Iterator &last) {
    
    FuncExprPtr f;
    
    //I don't need to decrement the end position. [b:e)
    ++first;
    if( first != last ) {
        f.reset( new FuncExpr() );
        while(first != last) {
            //parseCmd will increment.
            first = parseCmd(f, range, first);
        }
    }
    
    ++last; //after last
    
    return f;

}

template<>
TokenProcessor::Iterator AstGenerator::check_and_parse<CompExpr>(FuncExprPtr parent, TokenProcessor::Range &range, TokenProcessor::Iterator &ito, bool &found) {
    
    CmdToken &token = *ito;
    
    CompExpr *expr = CompExpr::create_if(token.cmd);
    if( expr != nullptr ) {
        expr->parse(token.line, token.cmd, token.params);
        generator::Expr::ExprPtr p(expr);
        parent->addBody(p);
        
        //Moving from _se_ to _[_
        ++ito;

        //O primeiro token apos se precisa ser um [
        if( ito->cmd != "[" )
            throw asm_exception("Apos um comando se, eh obrigatorio haver [");
        

        //Vamos agora procurar o começo e final
        TokenProcessor::Iterator itf = count_brackets(ito, range);
        
        int brackets = std::distance(ito, itf);
        if( brackets <= 1 ) {
            throw asm_exception("] final nao encontrado");
        }
        
        expr->true_body = add_cmds(expr, range, ito, itf);
        found = true;
        
        //Do we have else ?
        if( Utils::is_equal(itf->cmd, "senao" ) ) {
            
            ++itf;
            ito = itf;
            
            //Moving from _senao_ to _[_
            
            //O primeiro token apos se precisa ser um [
            if( ito->cmd != "[" )
                throw asm_exception("Apos um comando se, eh obrigatorio haver [");
            
            itf = count_brackets(ito, range);
            
            int brackets = std::distance(ito, itf);
            if( brackets <= 1 ) {
                throw asm_exception("] final nao encontrado");
            }
            
            expr->false_body = add_cmds(expr, range, ito, itf);
            found = true;
            
        }
        
        return itf;
    }
    found = false;
    return range.end();
}

template<>
TokenProcessor::Iterator AstGenerator::check_and_parse<CalcExpr>(FuncExprPtr parent, TokenProcessor::Range &range, TokenProcessor::Iterator &ito, bool &found) {
    
    CmdToken &token = *ito;
    
    CalcExpr *expr = CalcExpr::create_if(token.cmd);
    if( expr != nullptr ) {
        expr->parse(token.line, token.cmd, token.params);
        generator::Expr::ExprPtr p(expr);
        parent->addBody(p);
        
        //Moving from _se_ to _[_
        ++ito;
        
        //O primeiro token apos se precisa ser um [
        if( ito->cmd != "[" )
            throw asm_exception("Apos um comando se, eh obrigatorio haver [");
        
        
        //Vamos agora procurar o começo e final
        TokenProcessor::Iterator itf = count_brackets(ito, range);
        
        int brackets = std::distance(ito, itf);
        if( brackets <= 1 ) {
            throw asm_exception("] final nao encontrado");
        }
        
        ++ito; //calcula -> [
        std::string &buf = expr->calc_string;
        std::for_each(ito, itf, [&buf](CmdToken &s) {
            buf += s.cmd + " ";
        });
        
        ++itf; //] -> em
        if( !Utils::is_equal(itf->cmd,"em") ) {
            throw asm_exception("token 'em' nao encontrado");
        }
        
        ++itf; //em -> variable
        expr->var_name = itf->cmd;
        
        ++itf;

        found = true;
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
    
    itr = check_and_parse<CalcExpr>(parent, range, it, found);
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
