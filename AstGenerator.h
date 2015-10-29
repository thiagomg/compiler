//
// Created by Thiago on 8/19/15.
//

#ifndef COMPILER_ASTGENERATOR_H
#define COMPILER_ASTGENERATOR_H

#include <string>
#include <vector>
#include <memory>
#include "TokenProcessor.h"
#include "utils.h"
#include "asm_exception.h"

class TokenProcessor;
struct CmdToken;

namespace generator {

    struct Expr {
        using ExprPtr = std::shared_ptr<Expr>;
        virtual void parse(int line, const std::string &name, const std::vector<std::string> &params) = 0;
        
        virtual void setName(const std::string &name) { this->name = name; }
        virtual const std::string &getName() { return name; }

    private:
        std::string name;
        
    };

    struct FuncExpr : public Expr {

        std::vector<std::string> params;
        std::vector<ExprPtr> body;

        virtual void parse(int line, const std::string &name, const std::vector<std::string> &params) override {
            //this->name = name;
            setName(name);
            this->params = params;
        }

        void addBody(ExprPtr expr) {
            body.push_back(expr);
        }

        virtual ~FuncExpr() {}

    };
    using FuncExprPtr = std::shared_ptr<FuncExpr>;

    struct CallExpr : public generator::Expr {
        std::vector<std::string> params;
        virtual void parse(int line, const std::string &name, const std::vector<std::string> &params) override {
            //This is easy.
            setName(name);
            this->params = params;
        }
        
    };
    using FuncExprPtr = std::shared_ptr<FuncExpr>;
    
    class AstGenerator {
    public:

        AstGenerator(TokenProcessor &processor);
        void generate();
        void generate(FuncExprPtr parent, TokenProcessor::Range range);

        TokenProcessor::Iterator parseCmd(FuncExprPtr parent, TokenProcessor::Range &range, TokenProcessor::Iterator &it);

        void finish();

        std::string getCode();

        FuncExprPtr _mainExpr;
        TokenProcessor &_processor;

    };

    struct VarExpr : public Expr {
        
        std::string value;
        
        //FIXME: Por hora soh sera aceito +-*/, mas depois deve virar uma arvore
        
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


}
#endif //COMPILER_ASTGENERATOR_H
