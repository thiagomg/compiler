//
// Created by Thiago on 10/1/15.
//

#include "Runner.h"
#include "utils.h"

#include <iostream>
#include <unordered_map>

using VarsType = std::unordered_map<std::string, std::string>;

//built-in
namespace builtin {
    
    using namespace generator;
    using namespace std;
    using namespace Utils;
    
    std::string get_val(VarsType &vars, const std::string &name) {
        
        if( Utils::is_value(name) )
            return Utils::get_value(name);
        
        auto it = vars.find(name);
        if( it != vars.end() )
            return it->second;
        else {
            //TODO: Gerar um runtime_exception
            cerr << "Variavel " << name << " nao encontrada" << endl;
            return "";
        }
    }
    
    void escreva(VarsType &vars, CallExpr *expr) {
                
        for (auto t : expr->params) {
            cout << get_val(vars, t);
        }
        cout << endl;
        
    }
    
    void pergunta(VarsType &vars, CallExpr *expr) {
        const string &text = expr->params[0];
        cout << Utils::get_value(text);
        cout.flush();
        for_each(begin(expr->params)+1, end(expr->params), [&vars, &text](auto &var_name) {
            string val;
            cin >> val;
            vars[var_name] = val;
        });
    }

    void defina(VarsType &vars, VarExpr *expr) {
        const string &name = expr->getName();
        const string &value = expr->value;
        
        if( Utils::is_value(value) ) {
            vars[name] = value;
        } else {
            //Setting one var to another
            const std::string &v = vars[value];
            vars[name] = v;
        }
    }
    
    void se(Runner &runner, VarsType &vars, CompExpr *comp) {
        
        std::string l, r;
        l = get_val(vars, comp->params[0]);
        r = get_val(vars, comp->params[1]);
        
        if( l == r ) {
            //true
            if( comp->true_body )
                runner.run(comp->true_body);
        } else {
            //false body
        }
        
        
        
    }
    
    bool check(VarsType &vars, Expr::ExprPtr expr) {
        
        VarExpr *ve = dynamic_cast<VarExpr *>(expr.get());
        if( ve != nullptr ) {
            builtin::defina(vars, ve);
            return true;
        }
        
        BuiltInExpr *e = dynamic_cast<BuiltInExpr *>(expr.get());
        if( e == nullptr ) {
            return false;
        }
        
        if( is_equal(e->getName(), "escreva") ) {
            builtin::escreva(vars, e);
        }
        else if( is_equal(e->getName(), "pergunta") ) {
            builtin::pergunta(vars, e);
        }
        
        return true;
        
    }
    
}

void Runner::run(generator::FuncExprPtr func)
{
    using namespace std;
    using namespace generator;
    using namespace Utils;

    VarsType vars;
    
    //built-in

    //std::string root = "@";

    for( auto e : func->body ) {
        
        if( builtin::check(vars, e) ) {
            continue;
        }
        
        CompExpr *ce = dynamic_cast<CompExpr *>(e.get());
        if( ce != nullptr ) {
            builtin::se(*this, vars, ce);
        }
        
    }


}
