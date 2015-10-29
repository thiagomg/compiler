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
    
    void escreva(VarsType &vars, CallExpr *expr) {
                
        for (auto t : expr->params) {
            if( Utils::is_value(t) ) {
                cout << Utils::get_value(t);
            } else {
                //t => variable
                auto it = vars.find(t);
                if( it != vars.end() )
                    cout << it->second;
                else {
                    //TODO: Gerar um runtime_exception
                    cerr << "Variavel " << t << " nao encontrada" << endl;
                }
            }
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

}

void Runner::run(generator::FuncExprPtr func)
{
    using namespace std;
    using namespace generator;
    using namespace Utils;

    VarsType vars;
    
    //built-in

    //std::string root = "@";

    //TODO: change this to call ->exec and run without this fake reflection
    for( auto e : func->body ) {
        //std::string level = "@" + root;

        //cout << level << " " << e->getName() << endl;

        if( is_equal(e->getName(), "escreva") ) {
            CallExpr *ce = dynamic_cast<CallExpr *>(e.get());
            if( ce == nullptr ) {
                //TODO: Gerar run_time exception
                cerr << "Expr nao eh uma CallExpr!" << endl;
            }
            builtin::escreva(vars, ce);
        }
        else if( is_equal(e->getName(), "pergunta") ) {
            CallExpr *ce = dynamic_cast<CallExpr *>(e.get());
            builtin::pergunta(vars, ce);
        } else {
            
            VarExpr *ve = dynamic_cast<VarExpr *>(e.get());
            if( ve != nullptr ) {
                builtin::defina(vars, ve);
            }
        }
        
    }


}
