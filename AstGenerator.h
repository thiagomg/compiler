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
        
        static FuncExpr *create_if(const std::string &name) {
            if( name == "rotina" )
                return new FuncExpr();
            return nullptr;
        }

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
        
        static CallExpr *create_if(const std::string &name) {
            if( name == "rotina" )
                return new CallExpr();
            return nullptr;
        }

        std::vector<std::string> params;
        virtual void parse(int line, const std::string &name, const std::vector<std::string> &params) override {
            //This is easy.
            setName(name);
            this->params = params;
        }
        
    };
    using FuncExprPtr = std::shared_ptr<FuncExpr>;
    
    struct BuiltInExpr : public CallExpr {
        static BuiltInExpr *create_if(const std::string &name) {
            if(Utils::is_equal("escreva", name) || Utils::is_equal("pergunta", name) ) {
                return new BuiltInExpr();
            }
            return nullptr;
        }
        
        virtual void parse(int line, const std::string &name, const std::vector<std::string> &params) override {
            //This is easy.
            setName(name);
            if( name == "escreva" )
                this->params = params;
            else if( name == "pergunta" )
                this->params = { params[0], params[2] };

        }

        
    };

    struct VarExpr : public Expr {
        
        std::string value;
        
        static VarExpr *create_if(const std::string &name) {
            if( name == "defina" )
                return new VarExpr();
            return nullptr;
        }
        
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
        
        FuncExprPtr true_body;
        FuncExprPtr false_body;
        std::vector<std::string> params;
        
        static CompExpr *create_if(const std::string &name) {
            if( name == "se" )
                return new CompExpr();
            return nullptr;
        }
        
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
            
            //varUtils.validateVar(line, params[0]);
            //varUtils.validateVar(line, params[1]);
        }
        virtual ~CompExpr() {};
    };

    
    class AstGenerator {
    public:
        
        AstGenerator(TokenProcessor &processor);
        void generate();
        void generate(FuncExprPtr parent, TokenProcessor::Range range);
        
        TokenProcessor::Iterator parseCmd(FuncExprPtr parent, TokenProcessor::Range &range, TokenProcessor::Iterator &it);
        
        template<typename T>
        TokenProcessor::Iterator check_and_parse(FuncExprPtr parent, TokenProcessor::Range &range, TokenProcessor::Iterator &ito, bool &found);
        
        TokenProcessor::Iterator count_brackets(TokenProcessor::Iterator &first, TokenProcessor::Range &range);
        FuncExprPtr add_cmds(CompExpr *expr, TokenProcessor::Range &range, TokenProcessor::Iterator &first, TokenProcessor::Iterator &last);
        
        
        void finish();
        
        std::string getCode();
        
        FuncExprPtr _mainExpr;
        TokenProcessor &_processor;
        
    };

}
#endif //COMPILER_ASTGENERATOR_H
