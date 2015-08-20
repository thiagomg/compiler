//
// Created by Thiago on 8/19/15.
//

#include "AstGenerator.h"
#include "VarUtils.h"
#include "utils.h"
#include "asm_exception.h"

VarUtils varUtils;


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