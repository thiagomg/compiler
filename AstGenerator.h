//
// Created by Thiago on 8/19/15.
//

#ifndef COMPILER_ASTGENERATOR_H
#define COMPILER_ASTGENERATOR_H

#include <string>
#include <vector>
#include <memory>
#include "TokenProcessor.h"

class TokenProcessor;
struct CmdToken;

namespace generator {

    struct Expr {
        using ExprPtr = std::shared_ptr<Expr>;
        virtual void parse(int line, const std::string &name, const std::vector<std::string> &params) = 0;
        
        virtual void setName(const std::string &name) = 0;
        virtual const std::string &getName() = 0;
        
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

        virtual void setName(const std::string &name) override { this->name = name; }
        virtual const std::string &getName() override { return name; }

        virtual ~FuncExpr() {}

    private:
        std::string name;
        

    };
    using FuncExprPtr = std::shared_ptr<FuncExpr>;

    struct CallExpr : public generator::Expr {
        std::vector<std::string> params;
        virtual void parse(int line, const std::string &name, const std::vector<std::string> &params) override {
            //This is easy.
            this->name = name;
            this->params = params;
        }
        
        virtual void setName(const std::string &name) override { this->name = name; }
        virtual const std::string &getName() override { return name; }
        
    private:
        std::string name;
        
    };
    
    class AstGenerator {
    public:

        AstGenerator(TokenProcessor &processor);
        void generate();
        void generate(FuncExprPtr parent, TokenProcessor::Range range);

        TokenProcessor::Iterator parseCmd(FuncExprPtr parent, TokenProcessor::Range &range, TokenProcessor::Iterator &it);

        //void addCmd(int line, const std::string &cmd, const std::vector<std::string> &params);

        void finish();

        std::string getCode();

        FuncExprPtr _mainExpr;
        TokenProcessor &_processor;

    };



}
#endif //COMPILER_ASTGENERATOR_H
