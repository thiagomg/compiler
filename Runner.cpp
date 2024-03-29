//
// Created by Thiago on 10/1/15.
//

#include "Runner.h"
#include "utils.h"
#include "infix_parser.h"

#include <iostream>
#include <algorithm>

//built-in
namespace builtin {
    
    using namespace generator;
    using namespace std;
    using namespace Utils;
    
    std::string *get_var(Runner::VarsType &vars, const std::string &name, bool add_if = false) {
        auto it = std::find_if(begin(vars), end(vars), [&name](Runner::VarType &p) {
            return( is_equal(p.first, name) );
        });
        
        if( it != vars.end() )
            return &(it->second);
        
        if( !add_if )
            return nullptr;
        
        vars.push_back( make_pair(name, string() ) );
        auto &pair = vars.back();
        return &(pair.second);
        
    }
    
    const std::string get_val(Runner::VarsType &vars, const std::string &name) {
        
        if( Utils::is_value(name) )
            return Utils::get_value(name);
        
        string *v = get_var(vars, name);
        if( v != nullptr )
            return *v;
        else {
            //TODO: Gerar um runtime_exception
            cerr << "Variavel " << name << " nao encontrada" << endl;
            return "";
        }
    }
    
    void set_val(Runner::VarsType &vars, const std::string &name, const std::string &val) {
        string *v = get_var(vars, name, true);
        *v = val;
    }
    
    void escreva(Runner::VarsType &vars, CallExpr *expr) {
                
        for (auto t : expr->params) {
            cout << get_val(vars, t);
        }
        cout << endl;
        
    }
    
    void pergunta(Runner::VarsType &vars, CallExpr *expr) {
        const string &text = expr->params[0];
        cout << Utils::get_value(text);
        cout.flush();
        for_each(begin(expr->params)+1, end(expr->params), [&vars, &text](auto &var_name) {
            string val;
            cin >> val;
            set_val(vars, var_name, val);
        });
    }

    void defina(Runner::VarsType &vars, VarExpr *expr) {
        const string &name = expr->getName();
        const string &value = expr->value;
        
        
        if( Utils::is_value(value) ) {
            set_val(vars, name, value);
        } else {
            //Setting one var to another
            string s = get_val(vars, value);
            set_val(vars, name, s);
            //const std::string &v = vars[value];
            //vars[name] = v;
        }
    }
    
    template<typename T>
    bool operation(char op, T &l, T &r) {
        if( op == '=' )
            return l == r;
        if( op == '>' )
            return l > r;
        if( op == '<' )
            return l < r;
        return false;
    }
    
    bool operation_s(char op, const std::string &l, const std::string &r) {
        if( op == '=' )
            is_equal(l, r);
        if( op == '>' )
            return l > r;
        if( op == '<' )
            return l < r;
        return false;
    }
    
    void se(Runner &runner, Runner::VarsType &vars, CompExpr *comp) {
        
        std::string l, r;
        l = get_val(vars, comp->params[0]);
        r = get_val(vars, comp->params[1]);
        
        bool ret = false;
        if( Utils::is_number(l) && Utils::is_number(r) ) {
            int64_t il = std::stoll(l);
            int64_t ir = std::stoll(r);
            ret = operation(comp->operation, il, ir);
        } else {
            ret = operation_s(comp->operation, l, r);
        }
        
        if( ret ) {
            //true
            if( comp->true_body )
                runner._run(vars, comp->true_body);
        } else {
            //false
            if( comp->false_body )
                runner._run(vars, comp->false_body);
        }
        
    }
    
    //TODO: Acertar um parser infix que funcione!!!
    void calcula(Runner::VarsType &vars, CalcExpr *expr) {
        cout << "Calcula: " << expr->var_name << " -> " << expr->calc_string << endl;
        std::string v = get_value(expr->calc_string);
        double ret = alien::parse(v);
        if( (ret / (int)ret) == 1 ) {
            set_val(vars, expr->var_name, std::to_string((int)ret));
        } else {
            set_val(vars, expr->var_name, std::to_string(ret));
        }
    }
    
    bool check(Runner::VarsType &vars, Expr::ExprPtr expr) {
        
        VarExpr *ve = dynamic_cast<VarExpr *>(expr.get());
        if( ve != nullptr ) {
            builtin::defina(vars, ve);
            return true;
        }
        
        CalcExpr *ce = dynamic_cast<CalcExpr *>(expr.get());
        if( ce != nullptr ) {
            calcula(vars, ce);
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

//TODO: Trocar chamadas recursivas para este
void Runner::_run(Runner::VarsType &parentVars, generator::FuncExprPtr func)
{
    
    using namespace std;
    using namespace generator;
    using namespace Utils;
    
    int size = parentVars.size();
    
    //built-in
    
    //std::string root = "@";
    
    for( auto e : func->body ) {
        
        if( builtin::check(parentVars, e) ) {
            continue;
        }
        
        CompExpr *ce = dynamic_cast<CompExpr *>(e.get());
        if( ce != nullptr ) {
            builtin::se(*this, parentVars, ce);
        }
        
    }
    
    parentVars.resize(size);
    
}

void Runner::run(generator::FuncExprPtr func)
{
    Runner::VarsType vars;
    _run(vars, func);
}
