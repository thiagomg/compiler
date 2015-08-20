/* 
 * File:   AsmGenerator.h
 * Author: thiago
 *
 * Created on 6 de Maio de 2015, 23:38
 */

#ifndef ASMGENERATOR_H
#define	ASMGENERATOR_H

#include <vector>
#include <list>
#include <string>
#include <sstream>
//#include <unordered_set>
#include <set>
#include <memory>
#include <utility>

namespace generator {

    struct CmdToken {
        CmdToken(int line, const std::string &cmd, std::vector< std::string > &params) {
            this->line = line;
            this->cmd = cmd;
            this->params = params;
        }
//        CmdToken(CmdToken &&src) {
//            this->line = src.line;
//            this->cmd = src.cmd;
//            this->params = src.params;
//        }
        CmdToken(const CmdToken &src) {
            this->line = src.line;
            this->cmd = src.cmd;
            this->params = src.params;
        }
        int line;
        std::string cmd;
        std::vector<std::string> params;
    };

    class CppGenerator {

        using string = std::string;
        using ParamVector = std::vector<string>;

        std::list<CmdToken> _tokenList;

        //Used to hold cpp template
        std::string _before;
        std::string _after;


    protected:

        CmdToken _getNext();
        void _putBack(CmdToken &&cmdToken);
        bool _hasToken();
        
        //Generators
        void _genEscreva(CmdToken &cmdToken);

    public:
        CppGenerator();
        void addCmd(int line, const string &cmd, std::vector<string> &params);
        void finish();
        string getCode();


    };


}

#endif	/* ASMGENERATOR_H */

